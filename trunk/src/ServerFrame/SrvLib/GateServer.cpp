#include "SrvLib.h"
#include "readerwriter.h"
#include "ModuleSupport.h"
#include "GateServer.h"

SERVER_COMMAND_EX_BEGIN(cGateServer)
	SERVER_COMMAND_EX(lg2gt_Packet,				OnRcvLogicPacket)		// 收到服务器信息
	SERVER_COMMAND_EX(lg2gt_Broadcast_ByList,	OnRcvBroadcastByList)	// 收到逻辑服务器的广播消息
	SERVER_COMMAND_EX(lg2gt_Broadcast_All,		OnRcvBroadcastAll)		// 全体客户端广撿

	SERVER_COMMAND_EX(lg2gt_Kick,				OnRcvKick)				// 踢掉网关上的连接	
	SERVER_COMMAND_EX(lg2gt_KickByServerID,		OnRcvKickByServerID)	// 踢掉某个特定服务器的链接
	SERVER_COMMAND_EX(lg2gt_KickAll,			OnRcvKickAll)			// 断开网关上所有连接
	
	SERVER_COMMAND_EX(lg2gt_Login,				OnRcvLogin)				// 登录完成
	SERVER_COMMAND_EX(lg2gt_Character,			OnRcvCharacter)			// 角色选择完成
	SERVER_COMMAND_EX(lg2gt_ChangeMap,			OnRcvChangeMap)			// 转跳到相应的地图
SERVER_COMMAND_EX_END(cOtherServer)

void cGateServer::OnNewClient(Session * pClient)
{
	GateClient * pPlayer = NULL;
	UINT nID = XLIST_NEW(m_PlayerList,&pPlayer);
	if(nID ==0) {
		LOGF_ERROR(DLL_LOGGER,_T("无法创建玩家对象\n"));
		return;
	}
	ZeroMemory(pPlayer,sizeof(GateClient));
	pClient->info.nServerID = nID;
	pPlayer->nSessionListID = m_lstClients.GetId(pClient);
}

void cGateServer::OnDieClient(Session * pClient)
{
	// 通知给登录服务器，通知给所在的地图服务器。从列表中删除玩家信息
	GateClient * pPlayer = m_PlayerList.Get(pClient->info.nServerID);
	if(pPlayer)
	{
		if(pPlayer->eState>=ClientState_Login && m_pLogin)
		{
			m_Writer.Begin(gt2lg_Dropped);
			m_Writer.UINT(pClient->info.nServerID | (m_myInfo.nServerID<<16));
			m_pSessiones->Write(m_pLogin->nSession,m_Writer.Buffer(),m_Writer.Length());
		}
		if(pPlayer->eState>=ClientState_Game && pPlayer->nGameMapID)
		{
			Session * pGame = GetGameSession(pPlayer->nGameMapID);
			if(pGame)
			{
				m_Writer.Begin(gt2lg_Dropped);
				m_Writer.UINT(pClient->info.nServerID | (m_myInfo.nServerID<<16));
				m_pSessiones->Write(pGame->nSession,m_Writer.Buffer(),m_Writer.Length());
			}
		}
		m_PlayerList.Del(pPlayer);
	}
}

void cGateServer::OnClientPacket(Session * pClient)
{
	GateClient * pPlayer = m_PlayerList.Get(pClient->info.nServerID);
	if(pPlayer)
	{
		// 如果消息已加密，在这里解密。
		int nCmd = m_Reader.INT();

		// 安全校验:消息必须与玩家当前的阶段符合。
		// 安全校验:消息长度必须是合法的。

		// 消息转发
		if(pPlayer->eState>=ClientState_Login)
		{
			Session * pServer = GetMessageTarget(pPlayer,nCmd);
			if(pServer)
			{
				m_Writer.Begin(gt2lg_Packet);														// 消息类型
				m_Writer.UINT(pClient->info.nServerID | (m_myInfo.nServerID<<16));					// 消息来源
				m_Writer.INT(nCmd);																	// 子命令
				m_Writer.Struct(m_Reader.Struct(m_Reader.GetLeft()),m_Reader.GetLeft());			// 参数
				m_pSessiones->Write(pServer->nSession,m_Writer.Buffer(),m_Writer.Length()); 
				return;
			}
		}
		// 自己处理
		else
		{
		}
	}
}
cGateServer::cGateServer(ServerParam * sp)
	:cOtherServer(sp)
	,m_pLogin(NULL)
{
}
cGateServer::~cGateServer()
{
}
BOOL cGateServer::Create()
{
	return TRUE;
}

void cGateServer::OnRcvLogicPacket(Session * pSession)
{
	UINT nClientID = m_Reader.UINT();
	UINT nClientListID = nClientID & 0x0000FFFF;
	GateClient * pPlayer = m_PlayerList.Get(nClientListID);
	if(pPlayer)
	{
		Session * pClient = m_lstClients.Get(pPlayer->nSessionListID);

		size_t nSendLen = m_Reader.GetLeft();
		m_Writer<<CBEGIN();
		char *pBuffer = (char *)m_Reader.Struct(nSendLen);

		// todo: 处理Server_Ping的情况

		m_Writer.Struct(pBuffer,nSendLen);
		m_pSessiones->Write( pClient->nSession,m_Writer.Buffer(),m_Writer.Length());
	}
}
void cGateServer::OnRcvBroadcastByList(Session * pSession)
{
	int nCnt = m_Reader.INT();
	if (nCnt < 0 || nCnt >= 65535)
	{
		LOGF_ERROR(DLL_LOGGER,_T("广播出现错误，广播人数%d\n"),nCnt);
		return;
	}

	UINT *pClientIDs = (UINT *)m_Reader.Struct(sizeof(UINT)*nCnt);

	size_t nSendLen =m_Reader.GetLeft(); 
	char * pBuffer = (char *)m_Reader.Struct(nSendLen);
	int nRealCnt = 0;
	for(int n=0;n<nCnt;n++)
	{
		//可能在其他网关上
		int nMyId = (pClientIDs[n]>>16)&0x0000FFFF;
		if (nMyId != m_myInfo.nServerID)
			continue;

		UINT nClientListID = pClientIDs[n] & 0x0000FFFF;
		GateClient * pPlayer = m_PlayerList.Get(nClientListID);
		if(pPlayer)
		{
			Session * pClient = m_lstClients.Get(pPlayer->nSessionListID);
			if(pClient)
			{
				m_pSessiones->Write(pClient->nSession,pBuffer,nSendLen);
				nRealCnt ++;
			}
		}
	}
}
void cGateServer::OnRcvBroadcastAll(Session * pSession)
{
	size_t nSendLen = m_Reader.GetLeft();
	char * pBuffer = (char *)m_Reader.Struct(nSendLen);

	int nRealCnt = 0;
	Session * pClient = m_lstClients.NextData(); 
	for(;pClient;pClient = m_lstClients.NextData(pClient))
	{
		m_pSessiones->Write( pClient->nSession,pBuffer,nSendLen); 
		nRealCnt ++;
	}
}

void cGateServer::OnRcvKick(Session * pSession)
{
	// 只需要知道目的,直接转发
	UINT nClientID = m_Reader.UINT();
	UINT nClientListID = nClientID & 0x0000FFFF;
	GateClient * pPlayer = m_PlayerList.Get(nClientListID);
	if(pPlayer)
	{
		Session * pSession = m_lstClients.Get(pPlayer->nSessionListID);
		LOGF_ERROR(DLL_LOGGER,_T("收到kick指令,断开[%s]的连接,username=%s\n"),pSession->info.szIP,pPlayer->session.szUserName);
		KickPlayer(pPlayer);
	}
}
void cGateServer::OnRcvKickByServerID(Session * pSession)
{
	UINT nServerID = m_Reader.UINT();
	Session * pClient = m_lstClients.NextData(); 
	for(;pClient;pClient = m_lstClients.NextData(pClient))
	{
		GateClient * pPlayer = m_PlayerList.Get(pClient->info.nServerID);
		if(pPlayer && pPlayer->eState>=ClientState_Game && pPlayer->nGameMapID)
		{
			Session * pGame = GetGameSession(pPlayer->nGameMapID);
			if(pGame && pGame->info.nServerID == nServerID)
			{
				KickPlayer(pPlayer);
			}
		}
	}
}
void cGateServer::OnRcvKickAll(Session * pSession)
{
	UINT nServerID = m_Reader.UINT();
	Session * pClient = m_lstClients.NextData(); 
	for(;pClient;pClient = m_lstClients.NextData(pClient))
	{
		GateClient * pPlayer = m_PlayerList.Get(pClient->info.nServerID);
		if(pPlayer)
			KickPlayer(pPlayer);
	}
}


void cGateServer::OnRcvLogin(Session * pSession)
{
	// 带回用户ID和用户名
	size_t len = 0;
	UINT nClientID = m_Reader.UINT();
	UINT nUserID = m_Reader.UINT();
	char * lpszUserName = m_Reader.String(len);

	// 当前状态必须是ClientState_Version
	UINT nClientListID = nClientID & 0x0000FFFF;
	GateClient * pPlayer = m_PlayerList.Get(nClientListID);
	if(pPlayer)
	{
		assert(pPlayer->eState == ClientState_Version);
		_tcscpy_s(pPlayer->session.szUserName,(TCHAR*)lpszUserName);
		pPlayer->session.nUserID = nUserID;
		pPlayer->eState = ClientState_Login;
	}
}
void cGateServer::OnRcvCharacter(Session * pSession)
{
	// 带回角色名和地图号
	size_t len = 0;
	UINT nClientID = m_Reader.UINT();
	char * lpszNickName = m_Reader.String(len);
	UINT nMapID = m_Reader.UINT();
	
	// 当前状态必须大于ClientState_Login
	UINT nClientListID = nClientID & 0x0000FFFF;
	GateClient * pPlayer = m_PlayerList.Get(nClientListID);
	if(pPlayer)
	{
		assert(pPlayer->eState >= ClientState_Login);
		_tcscpy_s(pPlayer->session.szCharName,(TCHAR*)lpszNickName);
		pPlayer->eState = ClientState_Game;
		pPlayer->nGameMapID = nMapID;
	}
}
void cGateServer::OnRcvChangeMap(Session * pSession)
{
	// 带回地图号
	UINT nClientID = m_Reader.UINT();
	UINT nMapID = m_Reader.UINT();

	// 当前状态必须大于ClientState_Game
	UINT nClientListID = nClientID & 0x0000FFFF;
	GateClient * pPlayer = m_PlayerList.Get(nClientListID);
	if(pPlayer)
	{
		assert(pPlayer->eState >= ClientState_Game);
		pPlayer->nGameMapID = nMapID;
	}
}

void cGateServer::KickPlayer(GateClient * pClient)
{
	assert(pClient->nSessionListID!=-1);
	m_pSessiones->Disconnect(pClient->nSessionListID);
	pClient->eState = ClientState_Kicked;
}