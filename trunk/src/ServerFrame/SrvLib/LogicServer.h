#pragma once
#include "OtherServer.h"

// 与 DB 的沟通
// 与网关的沟通

class cLogicPlayer
{
protected:
	UINT m_nClientID;		// 在Gate上的容器ID(唯一的Gate服务器)
	UINT m_nListID;			// 在本地容器的ID

	stPlayerSession m_Session;// 玩家的连接信息
public:
	cLogicPlayer(UINT nClientID,UINT nListID,GUID session,const char * lpszIP);
	virtual ~cLogicPlayer(){}

	UINT GetClientID(){return m_nClientID;}
	UINT GetListID(){return m_nListID;}
	const stPlayerSession * GetPlayerSession(){return &m_Session;}
	void SetUserInfo(UINT nUserID,const TCHAR * lpszUserName);
	void SetCharInfo(UINT nCharID,const TCHAR * lpszCharName);
};

class cPlayerList : public cXList<cLogicPlayer *>
{
};

class cLogicServer: public cOtherServer
{
protected:
	cPlayerList		m_lstPlayers;
	
	CLIENT_COMMAND_EX_DECLARE;

	void OnRcvGatePacket(Session * pSession);
	void OnRcvNewPlayer(Session * pSession);
	void OnRcvDelPlayer(Session * pSession);
	
	// 具体游戏的逻辑服务器从这个接口来继承。
	virtual UINT OnNewPlayer(){return 0;}
	virtual void OnDelPlayer(UINT nPlayerID){}
	
	void OnPlayerPacket(int nCmd,UINT nPlayerID,size_t nSize);

public:
	
	cLogicServer(ServerParam * sp);
	virtual ~cLogicServer(){}
};


//-------------------------------------------------------------------------------------//
#define CLIENT_REQEST_COMMAND_BEGIN_EX(the_class)								\
	void the_class :: OnClientReqest(UINT cmd,cLogicPlayer * player)			\
	{																			\
		switch(cmd)																\
		{
#define CLIENT_REQEST_COMMAND_EX(cmd,func)										\
		case cmd:																\
			func(player);												\
			break;
#define CLIENT_REQEST_COMMAND_END_EX											\
		default:																\
			_error("unprocessed server inquiry %d!\n",cmd);						\
			assert(0);															\
			break;																\
		}																		\
	}

#define CLIENT_REQEST_COMMAND_DECLARE_EX										\
	void OnClientReqest(UINT cmd,cLogicPlayer * player)