#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>
#include "support.h"
#include "supportLoader.h"

#define SUPPORT_DLL	_T("Support.dll")

BOOL CSupportLoader::Load(LPCTSTR lpszLogFile, LPCTSTR lpszBasePath,LPCTSTR lpszExtraPath,LPCTSTR lpszEffcFile)
{
	if(CDllLoader::Load(SUPPORT_DLL))
	{
		FnSysInit * pFnSysInit = (FnSysInit *)GetProcAddress( m_hDll, GetSysInit_FN);
		FnSysShut * pFnSysShut = (FnSysShut *)GetProcAddress( m_hDll, GetSysShut_FN);
		if(pFnSysInit && pFnSysShut)
		{
			m_pSystem = pFnSysInit(lpszLogFile,lpszBasePath,lpszExtraPath,lpszEffcFile);
			m_pFnSysShut = pFnSysShut;
			return m_pSystem && m_pFnSysShut;
		}
	}
	return FALSE;
}

CSupportLoader::CSupportLoader()
	:m_pSystem(NULL)
	,m_pFnSysShut(NULL)
{
}
CSupportLoader::~CSupportLoader()
{
	if(m_pFnSysShut)
		m_pFnSysShut(m_pSystem);
	m_pSystem = NULL;
}