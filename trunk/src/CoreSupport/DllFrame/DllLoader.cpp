#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
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


