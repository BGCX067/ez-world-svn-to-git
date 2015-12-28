#pragma once

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>			// Windows 头文件:
#include "NetInterface.h"
#include "SrvLib.h"
#include "EzModule.h"

class CSupportLoader;
class cServerNet
{
	CSupportLoader&				m_Support;
	CModuleLoader<iSocketIO>	m_Netlib;
	CModuleLoader<iServerLogic> m_Logic;
	TCHAR m_szName[64];

	void Clear();
public:
	cServerNet(CSupportLoader& support);
	~cServerNet();

	BOOL CreateConfigTemplate(const TCHAR * lpszConfig);
	BOOL Create(const TCHAR * lpszConfig);
	void Destroy();
	BOOL Run(iFrameCallBack *);
	void Stop();
	UINT GetFPS();
	const TCHAR * GetName();

	virtual iSocketIO*	GetSocketIO() {
		return m_Netlib.GetInterface();
	}
	virtual iServerLogic* GetLogic() {
		return m_Logic.GetInterface();
	}
};
