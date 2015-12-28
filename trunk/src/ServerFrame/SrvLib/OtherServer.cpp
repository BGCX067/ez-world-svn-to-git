#include "SrvLib.h"
#include "readerwriter.h"
#include "ModuleSupport.h"
#include "strcode.h"
#include "OtherServer.h"

CLIENT_COMMAND_EX_BEGIN(cOtherServer)
	CLIENT_COMMAND_EX(ot2ot_MyInfo,		OnRcvMyInfo)			//收到服务器信息
CLIENT_COMMAND_EX_END(cBaseServer)
SERVER_COMMAND_EX_BEGIN(cOtherServer)
	SERVER_COMMAND_EX(mg2ot_ServerID,	OnRcvServerID)			//收到服务器信息
	SERVER_COMMAND_EX(mg2ot_SessionInfo,OnRcvSessionInfo)		//收到服务器信息
SERVER_COMMAND_EX_END(cBaseServer)

void cOtherServer::OnNewServer(Session * pServer)
{
	// 管理服务器
	if(pServer->info.nServerType == server_config::Logic::Manager)
	{
		m_pManager = pServer;
	}
	else
	{
		// 发送自己的信息给管理者
		m_Writer.Begin(ot2ot_MyInfo);
		m_Writer.Struct(&m_myInfo,sizeof(SessionInfo));
		m_pSessiones->Write(pServer->nSession,m_Writer.Buffer(),m_Writer.Length());
	}
}
void cOtherServer::OnDieServer(Session * pServer)
{
	if(pServer->info.nServerType == server_config::Logic::Manager)
	{
		LOGF_INFO(DLL_LOGGER,_T("管理服务器 断开,服务器必须终止\n"));
		m_bRun = FALSE; 
	}
}

void cOtherServer::OnRcvMyInfo(Session * pSession)
{
	pSession->info = *(SessionInfo*)m_Reader.Struct(sizeof(SessionInfo)); 
	LOGF_INFO(DLL_LOGGER,_T("接到服务器信息:%s,类型%d,id %d\n"),
		pSession->info.szIP,
		pSession->info.nServerType,
		pSession->info.nServerID);  
}

void cOtherServer::OnRcvServerID(Session * pSession)
{
	m_myInfo.nServerID = m_Reader.UINT(); 
	LOGF_INFO(DLL_LOGGER,_T("得到自己的服务器ID %d\n"),m_myInfo.nServerID);

	// 连到管理者后,就可以开始听了.
	if(!BeginListen())
		return;

	// 发送自己的信息给管理者
	m_Writer.Begin(ot2mg_MyInfo);
	m_Writer.Struct(&m_myInfo,sizeof(SessionInfo));
	m_pSessiones->Write(pSession->nSession,m_Writer.Buffer(),m_Writer.Length());
}
void cOtherServer::OnRcvSessionInfo(Session * pSession)
{
	SessionInfo info = *(SessionInfo*)m_Reader.Struct(sizeof(SessionInfo));
	LOGF_INFO(DLL_LOGGER,_T("接到新服务器信息.地址 %s 端口 %d 类型 %d\n"),info.szIP,info.nServerType);

	Session * pServer = NULL;
	UINT nID = XLIST_NEW(m_lstServers,&pServer);
	if(nID == 0)
	{
		LOGF_INFO(DLL_LOGGER,_T("无法创建服务器对象\n"));
		return;
	}
	pServer->info = info;
	pServer->nSession = pSession->nSession;

	if(0 == m_pSessiones->AsyncConnect(info.szIP,pServer))
		LOGF_INFO(DLL_LOGGER,_T("无法连接服务器\n"));
}

BOOL cOtherServer::OnStart()
{
	// 首先连接管理者
	LOGF_INFO(DLL_LOGGER,_T("连接服务器管理器%s\n"),CUtf8ToTchar(m_Config.manager_ip().c_str()).GetString());
	return m_pSessiones->AsyncConnect(CUtf8ToTchar(m_Config.manager_ip().c_str()).GetString(),NULL) != 0;
}
void cOtherServer::OnConnectFailed(Session * pServer)
{
	if(pServer)
	{
		LOGF_INFO(DLL_LOGGER,_T("连接失败%s[%s],再次连接...\n"),pServer->info.szIP,
			server_config::Logic::type_Name((server_config::Logic::type)pServer->info.nServerType));
		m_pSessiones->AsyncConnect(pServer->info.szIP,pServer);
	}else{
		LOGF_INFO(DLL_LOGGER,_T("连接管理服务器失败,重新连接...\n"));
		m_pSessiones->AsyncConnect(CUtf8ToTchar(m_Config.manager_ip().c_str()).GetString(),NULL);
	}
}

cOtherServer::cOtherServer(ServerParam * sp)
	:cBaseServer(sp)
	,m_pManager(NULL)
{
}
cOtherServer::~cOtherServer()
{
	m_pManager = NULL;
}

