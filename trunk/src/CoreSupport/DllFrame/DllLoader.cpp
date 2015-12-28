#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <tchar.h>
#include <atlbase.h>
#include "DllLoader.h"
#include "support.h"

CDllLoader::CDllLoader()
	:m_hDll(NULL)
{
}
CDllLoader::~CDllLoader()
{
	FreeLibrary(m_hDll);
}
BOOL CDllLoader::Load(const TCHAR * lpszDllName)
{
	m_hDll = LoadLibrary(lpszDllName);
	return m_hDll != NULL;
}


