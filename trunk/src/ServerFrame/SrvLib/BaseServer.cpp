#include "SrvLib.h"
#include "readerwriter.h"
#include "ModuleSupport.h"
#include "strcode.h"
#include "BaseServer.h"

void cBaseServer::Clear()
{
	m_pSessiones = NULL;
	m_myInfo.nServerType = -1;
	m_myInfo.szIP[0] = 0;
	m_myInfo.nServerID = 0;
	m_bRun = FALSE;
}
cBaseServer::cBaseServer(ServerParam * sp)
{
	Clear();
	m_Config.ParseFromArray(sp->pData,sp->nByteSize);

	m_pSessiones = sp->pSessiones;
	m_myInfo.nServerType = m_Config.server_type();
	if(sp->nClients>0)
		m_lstClients.Create(sp->nClients);
	if(sp->nServers>0)
		m_lstServers.Create(sp->nServers);
}
cBaseServer::~cBaseServer()
{
	m_lstServers.Destroy(); 
	m_lstClients.Destroy(); 
	Clear();
}

int cBaseServer::Run(iFrameCallBack * fb)
{
	LOGF_INFO(DLL_LOGGER,_T("%s 开始运行"),
		CUtf8ToTchar(m_Config.server_name().c_str()).GetString());

	if(!OnStart())
		return 0;

	// 进入循环
	size_t nLen = 0;
	UINT nSession = 0;
	LPVOID pparam = NULL;
	m_bRun = TRUE;

	while(m_bRun)
	{
		m_FPS.FrameStar();
		
		while((nSession=m_pSessiones->GetFailedSession(pparam))!=-1)
			OnConnectFailed((Session *)pparam);
		
		// 作为 Server,处理 ClientSession
		while((nSession = m_pSessiones->GetNewSession(SessionStyler_Provider,pparam))!=-1)
		{
			Session * pClient = OnClientConnected(nSession,pparam);
			if(pClient)
				OnNewClient(pClient);
		}
		while((nSession = m_pSessiones->GetDieSession(SessionStyler_Provider,pparam))!=-1)
		{
			assert(pparam != NULL);
			OnDieClient((Session *)pparam);
			OnClientDropped(nSession,pparam);
		}
		while((nSession=m_pSessiones->GetNextSession(pparam,SessionStyler_Provider,nSession))!=-1)
		{
			while(nLen = m_pSessiones->Read(nSession,m_Reader))
				OnClientPacket((Session *)pparam);
		}

		// 作为 Client,处理 Session
		while((nSession = m_pSessiones->GetNewSession(SessionStyler_Consumer,pparam))!=-1)
		{
			Session * pServer = OnServerConnected(nSession,pparam);
			if(pServer)
				OnNewServer(pServer);
		}
		while((nSession = m_pSessiones->GetDieSession(SessionStyler_Consumer,pparam))!=-1)
		{
			assert(pparam != NULL);
			OnDieServer((Session *)pparam);
			OnServerDropped(nSession,pparam);
		}
		while((nSession=m_pSessiones->GetNextSession(pparam,SessionStyler_Consumer,nSession))!=-1)
		{
			while(nLen = m_pSessiones->Read(nSession,m_Reader))
				OnServerPacket((Session *)pparam);
		}

		
		OnLoop();
		m_pSessiones->SendAll();
		
		// 速率统计
		m_FPS.FrameEnd(m_Config.beat());

		if(fb->FrameCallBack())
			break;
	}
	return 0;
}

void cBaseServer::Stop()
{
	InterlockedExchange((LONG*)&m_bRun, FALSE);
}
UINT cBaseServer::GetFPS()
{
	return m_FPS.GetFPS();
}

BOOL cBaseServer::BeginListen()
{
	if(!m_pSessiones->Listen(IpAddr_Any,
		CUtf8ToTchar(m_Config.listen_ip().c_str()).GetString(),
		(LPVOID)m_Config.server_type()))
	{
		LOGF_ERROR(DLL_LOGGER,_T("启动监听失败"));
		return FALSE;
	}
	LOGF_INFO(DLL_LOGGER,_T("开始监听在 %s"),CUtf8ToTchar(m_Config.listen_ip().c_str()).GetString());
	return TRUE;
}


Session * cBaseServer::OnClientConnected(UINT nSession,LPVOID lparam)
{
	Session *pClient = NULL;
	UINT nID = XLIST_NEW(m_lstClients,&pClient);
	if(nID ==0) {
		LOGF_ERROR(DLL_LOGGER,_T("无法创建客户端对象"));
		return NULL;
	}
	pClient->info.nServerType = -1;
	pClient->info.nServerID = 0;
	_tcscpy_s(pClient->info.szIP,sizeof(pClient->info.szIP)/sizeof(TCHAR), m_pSessiones->GetConnInfo(nSession) ) ;
	pClient->nSession = nSession;
	m_pSessiones->SetParam(nSession,pClient);
	LOGF_INFO(DLL_LOGGER,_T("客户端连接成功, 地址=%s, 共 %d 个连接. "), pClient->info.szIP, m_lstClients.GetCount()) ;
	return pClient;
}
void cBaseServer::OnClientDropped(UINT nSession,LPVOID lparam)
{
	Session * pClient = (Session *)lparam;
	LOGF_INFO(DLL_LOGGER,_T("客户端断开:%s,类型%s,id %d"),
				pClient->info.szIP,
				server_config::Logic::type_Name((server_config::Logic::type)pClient->info.nServerType),
				pClient->info.nServerID);
	m_lstClients.Del(pClient);
}
Session * cBaseServer::OnServerConnected(UINT nSession,LPVOID lparam)
{
	if(lparam == NULL)
	{
		Session * pSession = NULL;
		UINT nID = XLIST_NEW(m_lstServers,&pSession);
		if(nID == 0)
		{
			LOGF_ERROR(DLL_LOGGER,_T("无法创建服务器对象"));
			return NULL;
		}
		pSession->info.nServerType = server_config::Logic::Manager;
		pSession->info.nServerID = 0;	// 管理者的 ServerID 固定是 0
		_tcscpy_s(pSession->info.szIP,sizeof(pSession->info.szIP)/sizeof(TCHAR),CUtf8ToTchar(m_Config.manager_ip().c_str()).GetString());
		pSession->nSession = nSession;
		m_pSessiones->SetParam(nSession,pSession);
		return pSession;
	}
	return (Session *)lparam;
}
void cBaseServer::OnServerDropped(UINT nSession,LPVOID lparam)
{
	Session * pServer = (Session *)lparam;
	if(!pServer)
		return ;
	LOGF_INFO(DLL_LOGGER,_T("服务器断开:%s,类型%s,id %d"),
		pServer->info.szIP,
		server_config::Logic::type_Name((server_config::Logic::type)pServer->info.nServerType),
		pServer->info.nServerID);
	m_lstServers.Del(pServer);
}
