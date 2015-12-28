#pragma once
#include "LogicServer.h"

class cLoginPlayer : public cLogicPlayer
{
};

class cLoginServer : public cLogicServer
{
protected:
	cXList<cLoginPlayer> m_lstLoginPlayer;

	// 具体游戏的逻辑服务器从这个接口来继承。
	UINT OnNewPlayer();
	void OnDelPlayer(UINT nPlayerID);

	CLIENT_REQEST_COMMAND_DECLARE_EX;

public:
	
	cLoginServer(ServerParam * sp);
	virtual ~cLoginServer(){}
};