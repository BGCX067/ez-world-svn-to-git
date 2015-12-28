#pragma once

#include "Singleton.H"
#include <vector>
#include <string>

class CClientState;
class iClientNet;
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

struct stShowVersion 
{
	DWORD	dwMajor;
	DWORD	dwMrior;
	DWORD	dwVersion;
	DWORD	dwSVN;
};

//客户端程序主体
class CClientCore : public Singleton<CClientCore>
{
protected:
	
	// 网络设备
	SocketIOConfig	*	m_pSocketConfig;
	iClientNet		*	m_pClientNet;
	cReader			*	m_pReader;
	cWriter			*	m_pWriter;
	EClientState		m_connectstate;
	BOOL				m_bOtherLogin;
	int					m_nSession;
	bool				m_bshowdisconnect;
	bool				m_alterDisconnectBox;

	// 状态
	CClientState *	m_pClientState;
	CreateFn	**	m_ppCreateFns;
	UINT			m_nStates;

	// 客户端版本号
	int m_nVerMajor;
	int m_nVersion;
	int m_nVerPatch;
	std::string m_strVersionInfo;

	// check speed .
	DWORD	m_dwRecvCheckSpeedTm;
	bool	m_bRecvedCheckSpeed;
	void	SendCheckSpeedMsg();
	void	RecvCheckSpeedMsg();
	
	void Clear();
	void NetLoop();
	void IgnoreGateMsg(int nHeader);

	bool OnMessageBox(UINT nMsg,WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitialize(){return TRUE;}
	virtual void OnShutdown(){}

public:

	CClientCore();
	virtual ~CClientCore();

	inline iClientNet * GetClientNet(){return m_pClientNet;}
	inline cReader * GetReader(){return m_pReader;}
	inline cWriter * GetWriter(){return m_pWriter;}
	inline int GetSession(){return m_nSession;}

	inline void GetVersion(int &nVerMajor,int &nVersion,int &nVerPatch){nVerMajor=m_nVerMajor;nVersion=m_nVersion;nVerPatch=m_nVerPatch;}
	inline const char * GetVersionInfo() const {return m_strVersionInfo.c_str();}
	const char * ReadClientVersion();

	BOOL Initialize(iClientNet * pClientNet);
	void Shutdown();
	void Run();

	void InitState(CreateFn ** ppCreateFn, int nStates);	// 第一个被添加的做为缺省开始状态
	inline CClientState * GetClientState(){return m_pClientState;}
	BOOL ChangeState(UINT nState);

	BOOL OnResultError(int eCmd);

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

