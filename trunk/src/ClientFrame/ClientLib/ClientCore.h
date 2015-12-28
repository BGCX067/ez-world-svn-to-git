#pragma once

#include <vector>
#include <string>
#include "result.h"

class CClientState;
class cClientNet;
struct SocketIOConfig;
class cReader;
class cWriter;


typedef CClientState* CreateFn();

template<class TYPE>
CClientState * CreateState()
{
	return (CClientState *)new TYPE;
}

enum EClientState
{
	enotconnect,
	econnecting,
	econnected,
	edisconnecting
};

//客户端程序主体
class CClientCore
{
protected:
	
	// 网络设备
	cClientNet		*	m_pClientNet;
	cReader			*	m_pReader;
	cWriter			*	m_pWriter;
	EClientState		m_connectstate;
	BOOL				m_bOtherLogin;
	int					m_nSession;

	// 状态
	CClientState *	m_pClientState;
	CreateFn	**	m_ppCreateFns;
	UINT			m_nStates;

	void Clear();
	Result NetLoop();

	virtual BOOL OnInitialize(){return TRUE;}
	virtual void OnShutdown(){}
	virtual void OnConnected(){}

public:

	CClientCore();
	virtual ~CClientCore();

	inline cClientNet * GetClientNet(){return m_pClientNet;}
	inline cReader * GetReader(){return m_pReader;}
	inline cWriter * GetWriter(){return m_pWriter;}
	inline int GetSession(){return m_nSession;}

	BOOL Initialize(cClientNet * pClientNet,cReader * pReader,cWriter * pWriter);
	void Shutdown();
	Result Run();	// 出错返回非零值。

	void InitState(CreateFn ** ppCreateFn, int nStates);	// 第一个被添加的做为缺省开始状态
	inline CClientState * GetClientState(){return m_pClientState;}
	BOOL ChangeState(UINT nState);

	void SetConnected(EClientState e)
	{
		m_connectstate= e;
		m_nSession = INVALID_SESSION;
	}
	EClientState GetConnectState()
	{
		return m_connectstate;
	}
	void SetSession(int nSession)
	{
		m_nSession = nSession;
	}

	unsigned AsyncConnect(LPCTSTR pszServer);
	void	Disconnect();
};

