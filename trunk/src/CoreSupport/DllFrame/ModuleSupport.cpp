#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
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
