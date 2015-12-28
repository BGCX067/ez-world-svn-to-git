#pragma once

#include "xlist.h"
#include "FpsHolder.h"
#include "server_config.pb.h"

typedef cXList<Session> cSessionList;

class cBaseServer: public iServerLogic
{
protected:
	iSocketIO*			m_pSessiones;	
	cSessionList		m_lstServers;	// 自己作为客户端连上去的连接
	cSessionList		m_lstClients;	// 作为自己的客户端连上来的连接
	server_config::Logic m_Config;
	SessionInfo			m_myInfo;
	cReader 			m_Reader;
	cWriter				m_Writer;
	BOOL				m_bRun;
	CFpsHolder			m_FPS;

	// for children_class
	virtual void OnConnectFailed(Session * pServer){}
	virtual void OnNewClient(Session * pClient){}
	virtual void OnDieClient(Session * pClient){}
	virtual void OnClientPacket(Session * pClient){}
	virtual void OnNewServer(Session * pServer){}
	virtual void OnDieServer(Session * pServer){}
	virtual void OnServerPacket(Session * pServer){}
	virtual BOOL OnStart(){return TRUE;}
	virtual void OnLoop(){}

	BOOL BeginListen();

private:
	
	void Clear();
	Session * OnClientConnected(UINT nSession,LPVOID lparam);
	void OnClientDropped(UINT nSession,LPVOID lparam);
	Session * OnServerConnected(UINT nSession,LPVOID lparam);
	void OnServerDropped(UINT nSession,LPVOID lparam);

public:
	cBaseServer(ServerParam * sp);
	virtual ~cBaseServer();
	
	// for iServerLogic
	virtual BOOL Create(){return TRUE;}
	virtual void Destroy(){}
	virtual int  Run(iFrameCallBack * fb);
	virtual void Stop();
	virtual UINT GetFPS();
};

//-------------------------------------------------------------------------------------//
#define CLIENT_COMMAND_EX_BEGIN(the_class)										\
	void the_class :: OnClientPacket(Session * pClient)							\
	{																			\
		ServerCommand eCommand = (ServerCommand)m_Reader.Begin();				\
		switch(eCommand)														\
		{
#define CLIENT_COMMAND_EX(eCommand,func)										\
		case eCommand:															\
			func(pClient);														\
			break;
#define CLIENT_COMMAND_EX_END(base_class)										\
		default:																\
			base_class::OnClientPacket(pClient);								\
			break;																\
		}																		\
	}

#define CLIENT_COMMAND_EX_DECLARE												\
	void OnClientPacket(Session * pClient)

//-------------------------------------------------------------------------------------//
#define SERVER_COMMAND_EX_BEGIN(the_class)										\
	void the_class :: OnServerPacket(Session * pServer)							\
	{																			\
		ServerCommand eCommand = (ServerCommand)m_Reader.Begin();				\
		switch(eCommand)														\
		{
#define SERVER_COMMAND_EX(eCommand,func)										\
		case eCommand:															\
			func(pServer);														\
			break;
#define SERVER_COMMAND_EX_END(base_class)										\
		default:																\
			base_class::OnServerPacket(pServer);								\
			break;																\
		}																		\
	}

#define SERVER_COMMAND_EX_DECLARE												\
	void OnServerPacket(Session * pClient)
