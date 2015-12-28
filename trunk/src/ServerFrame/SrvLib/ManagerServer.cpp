#include <Shlwapi.h>
#include <tchar.h>
#include <stdio.h>
#include "SrvLib.h"
#include "readerwriter.h"
#include "ModuleSupport.h"
#include "ManagerServer.h"


CLIENT_COMMAND_EX_BEGIN(cManagerServer)
	CLIENT_COMMAND_EX(ot2mg_MyInfo,		OnRcvClientInfo)		//收到服务器信息
	CLIENT_COMMAND_EX(ot2mg_Transmit,	OnRcvTransmit)			//收到转发命令
CLIENT_COMMAND_EX_END(cBaseServer)

cManagerServer::cManagerServer(ServerParam * sp)
	:cBaseServer(sp)
{
	memset(m_ServerStyler,0,sizeof(ServerStyler)*Server_MaxType);
}

cManagerServer::~cManagerServer()
{
}

// 首先开始监听
BOOL cManagerServer::OnStart()
{
	return BeginListen();
}

void cManagerServer::OnNewClient(Session * pClient)
{
	pClient->info.nServerID = m_lstClients.GetId(pClient);
	// 告知服务器类型 ID
	m_Writer.Begin(mg2ot_ServerID);
	m_Writer.UINT(pClient->info.nServerID);
	m_pSessiones->Write(pClient->nSession,m_Writer.Buffer(),m_Writer.Length());
}

void cManagerServer::OnRcvClientInfo(Session * pClient)
{
	pClient->info = *(SessionInfo *)m_Reader.Struct(sizeof(SessionInfo));
	LOGF_INFO(DLL_LOGGER,_T("接到服务器信息:%s,类型%d,id %d\n"),
		pClient->info.szIP,
		pClient->info.nServerType,
		pClient->info.nServerID);

	// 告知服务器是否需要汇报负载
	m_Writer.Begin(mg2ot_ReportLoad);
	m_Writer.INT(!m_ServerStyler[pClient->info.nServerType].bUnique);
	m_pSessiones->Write(pClient->nSession,m_Writer.Buffer(),m_Writer.Length()); 

	Session * pSession = m_lstServers.First();
	while(pSession)
	{
		if(pClient != pSession)
		{
			// 发给所有关心这个服务的连接
			if(IfCarefulThis((ServerType)pSession->info.nServerType,(ServerType)pClient->info.nServerType))
			{
				m_Writer.Begin(mg2ot_SessionInfo);
				m_Writer.Struct(&pClient->info,sizeof(SessionInfo));
				m_pSessiones->Write(pSession->nSession,m_Writer.Buffer(),m_Writer.Length());
			}
			// 把已经有的服务连接发给当前的服务器
			if(IfCarefulThis((ServerType)pClient->info.nServerType,(ServerType)pSession->info.nServerType))
			{
				m_Writer.Begin(mg2ot_SessionInfo);
				m_Writer.Struct(&pSession->info,sizeof(SessionInfo));
				m_pSessiones->Write(pClient->nSession,m_Writer.Buffer(),m_Writer.Length()); 
			}
		}
		pSession = m_lstServers.Next(); 
	}
}
void cManagerServer::OnRcvTransmit(Session * pClient)
{
	UINT nTargetID = m_Reader.UINT(); 
	UINT nLen = m_Reader.UINT();
	void * pBuffer = m_Reader.Struct(nLen);

	// 在管理器这里，Server 的 ID 其实就是在 List 里的 ID
	Session * pSession = m_lstServers.Get(nTargetID);
	if(pSession)
	{
		m_Writer.Begin(mg2ot_Transmit);
		m_Writer.Struct(pBuffer,nLen);
		m_pSessiones->Write(pSession->nSession,m_Writer.Buffer(),m_Writer.Length()); 
	}
}

BOOL cManagerServer::IfCarefulThis(ServerType myType, ServerType targetType)
{
	for(int i=0; i<m_ServerStyler[myType].nDependNums;i++)
	{
		if(m_ServerStyler[myType].nDependTypes[i]==targetType)
			return TRUE;
	}
	return FALSE;
}

BOOL cManagerServer::Create()
{
	if(!m_Config.has_manager())
	{
		LOGF_ERROR(DLL_LOGGER,_T("配置文件中不存在Manager节点\n"));
		return FALSE;
	}
	
	const server_config::Logic::ManagerLogic& manager = m_Config.manager();
	int nStyler = manager.stylers_size();
	for(int i=0;i<nStyler;i++)
	{
		const ::server_config::Logic_ManagerLogic_Styler& stylers = manager.stylers(i);
		
		m_ServerStyler[i].bUnique = stylers.unique()?true:false;
		m_ServerStyler[i].nDependNums = stylers.depends_size();
		for(int j=0;j<m_ServerStyler[i].nDependNums;j++)
		{
			m_ServerStyler[i].nDependTypes[j] = stylers.depends(j);
		}
		for(int j=m_ServerStyler[i].nDependNums;j<Server_MaxType;j++)
		{
			m_ServerStyler[i].nDependTypes[j] = (ServerType)-1;
		}
	}

	return TRUE;
}
