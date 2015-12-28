#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>			// Windows 头文件:
#include <stdio.h>
#include <string.h>
#include <fstream>

#include "netlib.h"
#include "socket.h"
#include "thread.h"
#include "support.h"
#include "SupportLoader.h"
#include "strcode.h"

#include "server_config.pb.h"
#include "GameProto.h"
#include "servernet.h"

#define OVERLOAD
#include "overload.h"

#define NETLIB_DLL	_T("NetLib.dll")

IMemManager * pMemManager = NULL;

cServerNet::cServerNet(CSupportLoader& support)
	:m_Support(support)
{
	m_szName[0] = 0;
}

cServerNet::~cServerNet()
{
	Destroy();
	pMemManager = NULL;
}

BOOL cServerNet::CreateConfigTemplate(const TCHAR * lpszConfig)
{
	// todo:一个通用的 json 配置模板生成器。把所有的 可重复的域 和 可选域 都设置成有，输出json.
	server_config::Server config;
	server_config::Logic * pLogic = config.mutable_logic();
	server_config::Logic::ManagerLogic * pManager = pLogic->mutable_manager();
	pManager->add_stylers();
	pLogic->mutable_db();
	pLogic->mutable_game();
	config.add_socket_io();
	config.add_socket_io();

	// 写一个 Json 模板
	{
		Json::Value root(Json::objectValue);
		google::protobuf::GameProto::Pb2Json(root,&config);
		Json::StyledStreamWriter writer;
		std::ofstream jsonos( lpszConfig );
		if(!jsonos)
			return FALSE;
		writer.write( jsonos,root );
		jsonos.close();
	}
	return TRUE;
}

BOOL cServerNet::Create(const TCHAR * lpszConfig)
{
	server_config::Server config;
	// 从 Json 中读
	{
		std::ifstream jsonis( lpszConfig );
		if(!jsonis)
			return FALSE;
		Json::Reader reader;
		Json::Value root(Json::objectValue);
		if(!reader.parse(jsonis,root))
			return FALSE;
		google::protobuf::GameProto::Json2Pb(&config,root);
	}

	// 支持库
	const server_config::Path & path = config.path();
	if(!m_Support.Load(CUtf8ToTchar(path.log_file().c_str()).GetString(),
				CUtf8ToTchar(path.data_path().c_str()).GetString(),
				CUtf8ToTchar(path.update_pak().c_str()).GetString(),
				CUtf8ToTchar(path.efficiency_file().c_str()).GetString()))
		return FALSE;
	pMemManager = m_Support.GetSystem()->GetSysMem();

	// 创建网络模块
	SocketIOConfig sio[2];
	ZeroMemory(sio,sizeof(SocketIOConfig)*2);
	int nSocketIOs = config.socket_io_size();
	nSocketIOs = min(2,nSocketIOs);
	for(int i=0;i<nSocketIOs;i++)
	{
		const server_config::SocketIO & sockio = config.socket_io(i);
		sio[i].nMaxSocket = sockio.max_socket();
		sio[i].dwPacketBegin = *(DWORD *)"TSTB";
		sio[i].dwPacketEnd = *(DWORD *)"TSTE";
		sio[i].nRecvBufLen = sockio.recv_buf_len();
		sio[i].nSendPool = sockio.send_pool();
		sio[i].bPacketFlags = sockio.packet_flag()?TRUE:FALSE;
		sio[i].nDiscardThreshold=sockio.discard_threshold();
		sio[i].nPendLimit=sockio.pend_limit();
	}
	if(!m_Netlib.Load(NETLIB_DLL,sio,m_Support.GetSystem()))
		return FALSE;


	// 装载逻辑动态连接库
	const server_config::Logic& logic = config.logic();
	int nByteSize = logic.ByteSize();
	char * pLogicData = new char[nByteSize];
	logic.SerializeToArray(pLogicData,nByteSize);
	ServerParam sp(m_Netlib.GetInterface(),sio[0].nMaxSocket,sio[1].nMaxSocket,nByteSize,pLogicData);

	if(!m_Logic.Load(CUtf8ToTchar(logic.logic_dll().c_str()).GetString(),&sp,m_Support.GetSystem()))
		return FALSE;
	_tcscpy_s(m_szName,sizeof(m_szName)/sizeof(TCHAR),CUtf8ToTchar(logic.server_name().c_str()).GetString());
	if(!m_Logic.GetInterface()->Create())
		return FALSE;

	return TRUE;
}

void cServerNet::Destroy()
{
}

BOOL cServerNet::Run(iFrameCallBack *pframecallback)
{
	// 启动逻辑线程对象  启动 Client session 和 Server session 都由逻辑层控制
	m_Logic.GetInterface()->Run(pframecallback);
	return TRUE;
}

void cServerNet::Stop()
{
	// 销毁线程
	m_Logic.GetInterface()->Stop(); 
	m_Netlib.GetInterface()->Close();
	Destroy();
}

UINT cServerNet::GetFPS()
{
	return m_Logic.GetInterface()->GetFPS();
}

const TCHAR * cServerNet::GetName()
{
	return m_szName;
}