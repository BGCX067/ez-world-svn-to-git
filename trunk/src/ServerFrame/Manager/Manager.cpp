#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>			// Windows 头文件:
#include <winsock2.h>
#include <assert.h>
#include "ModuleSupport.h"
#include "SrvLib.h"
#include "readerwriter.h"
#include "ManagerServer.h"

#define OVERLOAD
#include "overload.h"


#if defined( MANAGER_EXPORTS )
#	define MANAGER_EXPORTS_API	extern "C" __declspec( dllexport )
#	define MANAGER_EXPORTS_CLASS	__declspec( dllexport )
#else
#	define MANAGER_EXPORTS_API	extern "C" __declspec( dllimport )
#	define MANAGER_EXPORTS_CLASS	__declspec( dllexport )
#endif

IMemManager * pMemManager = NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


MANAGER_EXPORTS_API iModule * CreateModule(const iInitParam * param, ISystem * pSystem)
{
	pMemManager = pSystem->GetSysMem();
	static CModuleSupport g_ms(pSystem,_T("NetLib"));
	return (iModule *)new cManagerServer((ServerParam*)param);
}

MANAGER_EXPORTS_API void DestroyModule(iModule *& pModule)
{
	delete pModule;
	pModule = NULL;
	pMemManager = NULL;
}

