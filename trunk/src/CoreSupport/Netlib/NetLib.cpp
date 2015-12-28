#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>			// Windows 头文件:
#include <winsock2.h>
#include <assert.h>

#include "support.h"
#include "ModuleSupport.h"

#include "NetLib.h"
#include "socket.h"
#include "iocp.h"

#define OVERLOAD
#include "overload.h"

IMemManager * pMemManager = NULL;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

int m_bWSAStartup = FALSE;

BOOL OpenWinSockDll()
{
	if(!m_bWSAStartup)
	{
		WSADATA m_wsaData;
		if(WSAStartup(MAKEWORD(2,2), &m_wsaData) == SOCKET_ERROR)
			return FALSE;
		m_bWSAStartup = TRUE;
	}
	return TRUE;
}

void CloseWinSockDll()
{
	if(m_bWSAStartup)
	{
		WSACleanup();
		m_bWSAStartup = FALSE;
	}
}



iModule * CreateModule(const iInitParam * param, ISystem * pSystem)
{
	pMemManager = pSystem->GetSysMem();
	OpenWinSockDll();
	static CModuleSupport g_ms(pSystem,_T("NetLib"));

	return new cIocpIO((const SocketIOConfig*)param);
}

void DestroyModule(iModule *& pModule)
{
	delete pModule;
	pModule = NULL;
	CloseWinSockDll();
	pMemManager = NULL;
}

