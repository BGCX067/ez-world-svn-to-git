#pragma once
#include "OtherServer.h"

/*
	连接上来一定时间不发包踢掉
	发包过于频繁踢掉
	加速器校验不符合踢掉
	命令找不到逻辑服务器目标的踢掉
	黑名单IP列表
	
	消息包加密解密
	消息转发
	消息群发
	
	统计消息发送流量
	消息发送顺序校验

	维护与客户端连接相关联的具体玩家信息
	玩家连接或者断线通知到相应逻辑服务器
	决定某客户端的具体消息该传给哪个逻辑服务器
*/

enum ClientState
{
	ClientState_Kicked = -1,
	ClientState_None,
	ClientState_Version,		// 版本校验
	ClientState_Login,			// 帐号登录
	ClientState_Character,		// 角色选择
	ClientState_Game,			// 游戏
	ClientState_ChangeMap,		// 正在切换地图...
};

struct GateClient		// 只有网关用到这个结构
{
	stPlayerSession session;					// 玩家的连接信息
	
	ClientState		eState;						// 在游戏中所处的状态
	UINT			nGameMapID;					// 当前所在的地图号(可能在不同地图,决定消息转发的目的)
	UINT			nSessionListID;				// 所代表的Session在容器中的ID
	
	DWORD			dwOtherwiseTime;			// disconnect request issured time
	DWORD			dwLastPacketReceived;		// 上次接到的消息时间。太长时间未收到消息的踢掉。
	DWORD			dwLastSpeedCheck;			// 上次发送速度检测的时间(同时用为客户端断线检测)。
	DWORD			dwSetIntervalTm;			// 应该再多少时间后收到速度检测的回馈。
};

class cGateServer : public cOtherServer
{
protected:
	cXList<GateClient> m_PlayerList;
	Session *	m_pLogin;

	virtual void OnNewClient(Session * pClient);
	virtual void OnDieClient(Session * pClient);
	virtual void OnClientPacket(Session * pClient);

	SERVER_COMMAND_EX_DECLARE;

	void OnRcvLogicPacket(Session * pSession);
	void OnRcvBroadcastByList(Session * pSession);
	void OnRcvBroadcastAll(Session * pSession);
	void OnRcvKick(Session * pSession);
	void OnRcvKickByServerID(Session * pSession);
	void OnRcvKickAll(Session * pSession);
	void OnRcvLogin(Session * pSession);
	void OnRcvCharacter(Session * pSession);
	void OnRcvChangeMap(Session * pSession);

	// 具体游戏的网关服务器从这个接口来继承。
	Session * GetGameSession(UINT nGameMapID){return NULL;}
	Session * GetMessageTarget(GateClient * pPlayer,int nCmd){return NULL;}
	void KickPlayer(GateClient * pClient);

public:
	cGateServer(ServerParam * sp);
	virtual ~cGateServer();
	virtual BOOL Create();
};