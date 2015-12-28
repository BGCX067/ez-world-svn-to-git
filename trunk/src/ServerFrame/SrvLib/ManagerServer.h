#pragma once
#include "BaseServer.h"

/* 
	基类负责决策同类型多个服务器的挑选问题。
		如果自己是需要管理器进行决策的服务器，一旦在连接数发生改变的时候，就要向管理器汇报。
		
		通知被选种的逻辑服务器开始服务？开始参数每个游戏都不确定，
*/

typedef server_config::Logic::type ServerType;
const int Server_MaxType = server_config::Logic::type_MAX + 1;

struct ServerStyler
{
	BOOL bUnique;							// 是否唯一
	int	nDependNums;						// 关心的依赖类型数量
	ServerType nDependTypes[Server_MaxType];	// 关心的依赖类型列表
};

class cManagerServer: public cBaseServer
{
	ServerStyler m_ServerStyler[Server_MaxType];

	BOOL IfCarefulThis(ServerType myType, ServerType targetType);

protected:
	virtual BOOL OnStart();
	virtual void OnNewClient(Session * pClient);
	
	CLIENT_COMMAND_EX_DECLARE;

	void OnRcvClientInfo(Session * pSession);
	void OnRcvTransmit(Session * pSession);

public:
	cManagerServer(ServerParam * sp);
	virtual ~cManagerServer();
	virtual BOOL Create();
};
