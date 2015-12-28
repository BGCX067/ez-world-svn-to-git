#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include "support.h"
#include "ModuleSupport.h"
#include <wchar.h>

CModuleSupport * Singleton<CModuleSupport>::ms_Singleton = NULL;

CModuleSupport::CModuleSupport(	ISystem * pSystem, const TCHAR * lpszModuleName )
	:m_pSystem(pSystem)
	,m_pLog(pSystem->GetSysLog()->GetInstance(lpszModuleName))
{
}
