#pragma once
#include "BaseServer.h"

// 玩家的连接信息
#define MAX_USERNAME	    16		// 用户名的最大长度
#define MAX_NICKNAME		16		// 昵称的最大长度
struct stPlayerSession
{
	UINT nUserID;						// 用户数据库ID
	TCHAR szUserName[MAX_USERNAME+1];	// 帐号
	UINT nCharID;						// 角色数据库ID
	TCHAR szCharName[MAX_NICKNAME+1];	// 角色名
	GUID session;						// 一个唯一的SessionID
	char szIP[MAX_IP_LEN];				// 玩家在Gate上的IP和端口
};

class cOtherServer: public cBaseServer
{
protected:
	Session *	m_pManager;

	virtual void OnConnectFailed(Session * pServer);
	virtual void OnNewServer(Session * pServer);
	virtual void OnDieServer(Session * pServer);
	virtual BOOL OnStart();

	CLIENT_COMMAND_EX_DECLARE;
	SERVER_COMMAND_EX_DECLARE;

	void OnRcvMyInfo(Session * pSession);
	void OnRcvServerID(Session * pSession);
	void OnRcvSessionInfo(Session * pSession);

public:
	cOtherServer(ServerParam * sp);
	virtual ~cOtherServer();
};