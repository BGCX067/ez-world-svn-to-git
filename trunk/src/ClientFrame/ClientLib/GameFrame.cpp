#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>			// Windows 头文件:
#include <fstream>
#include <shlobj.h>
#include <shlwapi.h>
#include "strcode.h"
#include "CmdLineParser.h"
#include "crashoutput.h"
#include "BugReport.h"
#include "VersionInfo.h"
#include "GameFrame.h"
#include "ClientCore.h"

#define NETLIB_DLL	_T("NetLib.dll")

IMemManager * pMemManager = NULL;

CGameFrame * Singleton<CGameFrame>::ms_Singleton = NULL;

CGameFrame::CGameFrame(CSupportLoader& support)
	:m_Support(support)
	,m_pClientNet(NULL)
	,m_GameProto(support)
{
	m_szAppDataPath[0] = 0;
}

BOOL CGameFrame::CreateConfigTemplate(const TCHAR * lpszConfig)
{
	client_config::client config;
	config.set_clients(1);
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

BOOL CGameFrame::Initialize(const TCHAR * lpszConfig,const TCHAR * lpszCmdLine,CVersionInfo * pVersion)
{
	// 取得执行程序所在的目录
	::GetCurrentDirectory(MAX_PATH,m_szAppDataPath);
	
	// 命令行解析
	CCmdLineParser parser(lpszCmdLine, true);
	if(parser.HasVal(_T("from")))
		SetAppDataPath(parser.GetVal(_T("install")));
	if(parser.HasVal(_T("area")))
		m_sAera = parser.GetVal(_T("area"));
	if(parser.HasVal(_T("server")))
		m_sIpAddr = parser.GetVal(_T("server"));

	// 从 Json 中读
	{
		std::ifstream jsonis( lpszConfig );
		if(!jsonis)
			return FALSE;
		Json::Reader reader;
		Json::Value root(Json::objectValue);
		if(!reader.parse(jsonis,root))
			return FALSE;
		google::protobuf::GameProto::Json2Pb(&m_Config,root);
	}

	// 崩溃报告的地址
#ifndef _DEBUG
	EmailConfig config = 
	{
		m_Config.report().smtp().c_str(),25,		// smtp server
		m_Config.report().usename().c_str(),		// username
		m_Config.report().password().c_str(),		// password
		m_Config.report().reciver().c_str(),	    // 接收者
		m_Config.report().emailaddr().c_str()		// 发送者
	};

	COutputCallbackEmail callbackEmail(&config,pVersion->GetVersionInfo());
	InitStackTraceLibrary(&callbackEmail);
#endif

	// 支持库
	const client_config::Path & path = m_Config.path();
	if(!m_Support.Load(CUtf8ToTchar(path.log_file().c_str()).GetString(),
				CUtf8ToTchar(path.data_path().c_str()).GetString(),
				CUtf8ToTchar(path.update_pak().c_str()).GetString(),
				CUtf8ToTchar(path.efficiency_file().c_str()).GetString()))
		return FALSE;
	pMemManager = m_Support.GetSystem()->GetSysMem();

	// 创建网络模块
	SocketIOConfig sio[2];
	ZeroMemory(sio,sizeof(SocketIOConfig)*2);

	const client_config::SocketIO & sockio = m_Config.socket_io();
	sio[1].dwPacketBegin = *(DWORD *)sockio.packet_begin().c_str();
	sio[1].dwPacketEnd = *(DWORD *)sockio.packet_end().c_str();
	sio[1].nRecvBufLen = sockio.recv_buf_len();
	sio[1].nSendPool = sockio.send_pool();
	sio[1].bPacketFlags = sockio.packet_flag()?TRUE:FALSE;
	sio[1].nDiscardThreshold=sockio.discard_threshold();
	sio[1].nPendLimit=sockio.pend_limit();
	sio[1].nMaxSocket = m_Config.has_clients()?m_Config.clients():1;
		
	if(!m_Netlib.Load(NETLIB_DLL,sio,m_Support.GetSystem()))
		return FALSE;
	m_pClientNet = new cClientNet(m_Netlib.GetInterface());

	const client_config::GameProto & proto = m_Config.proto();
	// 游戏原型定义
	if(!m_GameProto.Init(CUtf8ToTchar(proto.desc_file().c_str()).GetString()))
		return FALSE;
	// 游戏数据装载
	
	// 分配客户端
	m_nClients = m_Config.has_clients()?m_Config.clients():1;
	m_pClients = new CClientCore[m_nClients];
	for(UINT n=0;n<m_nClients;n++)
	{
		m_pClients[n].Initialize(m_pClientNet,&m_Reader,&m_Writer);
	}

	return TRUE;
}
void CGameFrame::Shutdown()
{
	delete []m_pClients;
	delete m_pClientNet;
}
void CGameFrame::Run(iFrameCallback * pFrameCallback)
{
	while(1)
	{
		m_FPS.FrameStar();
		for(UINT n=0;n<m_nClients;n++)
		{
			m_pClients[n].Run();
		}		
		m_pClientNet->SendAll();
		m_FPS.FrameEnd(m_Config.beats());
		if(pFrameCallback->FrameCallback())
			break;
	}
}

void CGameFrame::SetAppDataPath(LPCTSTR lpCmdLine)
{
	if(lpCmdLine)
	{
		::SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, m_szAppDataPath); 
		_tcscat_s(m_szAppDataPath,MAX_PATH,_T("\\"));
		_tcscat_s(m_szAppDataPath,MAX_PATH,lpCmdLine);

		if(!::PathFileExists(m_szAppDataPath))
			::CreateDirectory(m_szAppDataPath,NULL);
	}
}