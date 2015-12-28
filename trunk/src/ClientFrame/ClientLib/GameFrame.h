#pragma once
#include "Singleton.H"
#include "FpsHolder.h"
#include "tstring.h"

#include "support.h"
#include "SupportLoader.h"
#include "DllLoader.h"
#include "readerwriter.h"
#include "NetInterface.h"

#include "GameProto.h"
#include "ClientNet.h"
#include "client_config.pb.h"

class CClientCore;
class CSupportLoader;
class CVersionInfo;

class iFrameCallback
{
public:
	virtual bool  FrameCallback()=0;
};

class CGameFrame : public Singleton<CGameFrame>
{
	CFpsHolder					m_FPS;
	client_config::client		m_Config;
	CSupportLoader&				m_Support;
	CVersionInfo *				m_pVersion;
	t_string					m_sAera;
	t_string					m_sIpAddr;
	TCHAR						m_szAppDataPath[MAX_PATH];

	// 网络设备
	SocketIOConfig				m_SocketConfig;
	CModuleLoader<iSocketIO>	m_Netlib;
	cClientNet	*				m_pClientNet;
	cReader						m_Reader;
	cWriter						m_Writer;

	// GameProto
	google::protobuf::GameProto m_GameProto;

	// Client容器？
	CClientCore	* m_pClients;
	UINT m_nClients;

	void SetAppDataPath(LPCTSTR lpCmdLine);
	BOOL CreateConfigTemplate(const TCHAR * lpszConfig);

public:
	CGameFrame(CSupportLoader& support);
	~CGameFrame(){}

	BOOL Initialize(const TCHAR * lpszConfig,const TCHAR * lpszCmdLine,CVersionInfo * pVersion);
	void Shutdown();
	void Run(iFrameCallback * pFrameCallback);
	unsigned int GetFps(){return m_FPS.GetFPS();}
};
