#pragma once

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>			// Windows 头文件:
#include <tchar.h>
#include "NetInterface.h"
#include "SrvMsg.h"

class iFrameCallBack
{
public:
	virtual bool  FrameCallBack()=0;
};

class iServerLogic : iModule
{
public:
	virtual BOOL Create() = 0;		// 初始化
	virtual void Destroy() = 0;		// 销毁
	virtual int  Run(iFrameCallBack *) = 0;
	virtual void Stop() = 0;
	virtual UINT GetFPS() = 0;
	virtual void OnResume(){}
};

struct ServerParam : public iInitParam
{
	iSocketIO * pSessiones;
	int nServers;
	int nClients;
	int nByteSize;
	char * pData;

	ServerParam(iSocketIO * _pSessiones,int _nServers,int _nClients,int _nByteSize,char * _pData)
		:pSessiones(_pSessiones)
		,nServers(_nServers)
		,nClients(_nClients)
		,nByteSize(_nByteSize)
		,pData(_pData)
	{
	}
};

//typedef iServerLogic * (*CREATESRVLOGIC_PROC)(iSocketIO *pSessiones, ServerLogicConfig * config, int nServers,int nClients);
//typedef void (*DESTROYSRVLOGIC_PROC)(iServerLogic *pServerLogic);
//#define CREATESRVLOGIC_PROC_NAME "CreateServerLogic"
//#define DESTROYSRVLOGIC_PROC_NAME "DestroyServerLogic"

#define CLIENT_COMMAND_BEGIN(the_class)											\
	void the_class :: OnClientPacket(Session * pClient, size_t nSize)			\
	{																			\
		ServerCommand eCommand = (ServerCommand)m_Reader.Begin();				\
		switch(eCommand)														\
		{
#define CLIENT_COMMAND(eCommand,func)											\
		case eCommand:															\
			func(pClient,nSize);												\
			break;
#define CLIENT_COMMAND_END														\
		default:																\
			break;																\
		}																		\
	}

#define SERVER_COMMAND_BEGIN(the_class)											\
	void the_class :: OnServerPacket(Session * pServer, size_t nSize)			\
	{																			\
		ServerCommand eCommand = (ServerCommand)m_Reader.Begin();				\
		switch(eCommand)														\
		{
#define SERVER_COMMAND(eCommand,func)											\
		case eCommand:															\
			func(pServer,nSize);												\
			break;
#define SERVER_COMMAND_END														\
		default:																\
			break;																\
		}																		\
	}


#define CLIENT_COMMAND_DECLARE													\
	void OnClientPacket(Session * pClient, size_t nSize)
#define SERVER_COMMAND_DECLARE													\
	void OnServerPacket(Session * pServer, size_t nSize)

#define INHERIT_SUPER_DECLARE													\
	virtual BOOL	Create();													\
	virtual void	Destroy();													\
	virtual int		Run();														\
	virtual void	Stop();														


////////////////////////////////////////////////////////////////
//	具体游戏无关的服务器组构架
////////////////////////////////////////////////////////////////
struct PACKET_TIMER_MARK
{
	float client_logic_mark;		//客户端逻辑层发送时间
	float client_iocp_send_mark;	//客户端iocp层发送时间
	float gate_iocp_recv_mark;		//Gate iocp层接收时间
	float gate_logic_mark;			//Gate端逻辑层发送时间
};

struct SessionInfo
{
	UINT nServerID;			// 服务器的ID
	int nServerType;		// 服务器的类型
	UINT iServer;			// 当前类型的几号服务器
	TCHAR szIP[MAX_IP_LEN];	// 地址
	TCHAR szName[64];		// 服务器名字

	SessionInfo()
	{
		ZeroMemory(this,sizeof(SessionInfo));
		nServerType = -1;
	}
};

struct Session		// 所有的服务器都用到这个结构
{
	SessionInfo		info;				// 连接信息
	UINT			nSession;			// 局部 Session
};

/*
cBaseServer
	cManagerServer				
	cOtherServer				
		cGateServer				
		cLogicServer
			cLoginServer
			cGameServer
		cCalcServer
		cAIServer
*/

/*
MG(管理者)
OT(被管理者)※	
	GT(网关)
	LG(逻辑服务器)※	
		LI(登录)
		GM(游戏逻辑)
	CL(计算)
	AI(AI)
*/
