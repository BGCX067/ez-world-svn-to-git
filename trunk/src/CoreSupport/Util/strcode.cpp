#include  <stdlib.h> 
#include "strcode.h"
#include <windows.h>

CMbsToWcs::CMbsToWcs()
	:m_p(NULL)
{
}

CMbsToWcs::CMbsToWcs(const char * p)
	:m_p(NULL)
{
	Conver(p);   
}
CMbsToWcs::~CMbsToWcs()
{
	free(m_p);
}
const wchar_t * CMbsToWcs::Conver(const char * p)
{
 	if(!p)
 		return NULL;

	int numWChar = MultiByteToWideChar(CP_ACP,0,p,-1,NULL,0);
	m_p = (wchar_t *)malloc(numWChar * sizeof(wchar_t));
	if(m_p)
		MultiByteToWideChar(CP_ACP,0,p,-1,m_p,numWChar);

	return m_p;
}

CWcsToMbs::CWcsToMbs()
	:m_p(NULL)
{
}
CWcsToMbs::CWcsToMbs(const wchar_t * p)
	:m_p(NULL)
{
	Conver(p);
}
CWcsToMbs::~CWcsToMbs()
{
	free(m_p);
}
const char * CWcsToMbs::Conver(const wchar_t * p)
{
 	if(!p)
 		return NULL;

	int numChar = WideCharToMultiByte(CP_ACP,0,p,-1,NULL,0,NULL,NULL);
	m_p = (char *)malloc(numChar * sizeof(char));
	if(m_p)
		WideCharToMultiByte(CP_ACP,0,p,-1,m_p,numChar,NULL,NULL);

	return m_p;
}


CUtf8ToTchar::CUtf8ToTchar(const char * lpszStr)
	:m_pUstr(NULL)
	,m_pAsin(NULL)
{
	int size = strlen(lpszStr);
	int uSize = ::MultiByteToWideChar(CP_UTF8,0,lpszStr,size,NULL,0);
	m_pUstr = new wchar_t[uSize+1];
	size = ::MultiByteToWideChar(CP_UTF8,0,lpszStr,size,m_pUstr,uSize);
	m_pUstr[uSize] = 0;
#ifndef  UNICODE
	size = ::WideCharToMultiByte(CP_ACP,0,m_pUstr,-1,NULL,0,NULL,NULL);
	m_pAsin = new char[size+1];
	size = ::WideCharToMultiByte(CP_ACP,0,m_pUstr,-1,m_pAsin,size,NULL,NULL);
	m_pAsin[size+1] = 0;
#endif
}
CUtf8ToTchar::~CUtf8ToTchar()
{
	delete []m_pUstr;
	delete []m_pAsin;
}
const TCHAR * CUtf8ToTchar::GetString()const
{
#ifndef  UNICODE
	return m_pAsin;
#else
	return m_pUstr;
#endif
}

CTcharToChar::CTcharToChar()
	:m_p(NULL)
{
}
CTcharToChar::CTcharToChar(const TCHAR * p)
	:m_p(NULL)
{
	Conver(p);
}
CTcharToChar::~CTcharToChar()
{
#ifdef  UNICODE
	free(m_p);
#endif
}
const char * CTcharToChar::Conver(const TCHAR * p)
{
 	if(!p)
 		return NULL;
#ifdef  UNICODE
	int numChar = WideCharToMultiByte(CP_ACP,0,p,-1,NULL,0,NULL,NULL);
	m_p = (char *)malloc(numChar * sizeof(char));
	if(m_p)
		WideCharToMultiByte(CP_ACP,0,p,-1,m_p,numChar,NULL,NULL);
#else
	m_p = p;
#endif
	return m_p;
}

CCharToTchar::CCharToTchar(const char * p)
	:m_p(NULL)
{
	Conver(p);
}
CCharToTchar::CCharToTchar()
	:m_p(NULL)
{
}
CCharToTchar::~CCharToTchar()
{
#ifdef  UNICODE
	free(m_p);
#endif
}
const TCHAR * CCharToTchar::Conver(const char * p)
{
 	if(!p)
 		return NULL;
#ifdef  UNICODE
	int numWChar = MultiByteToWideChar(CP_ACP,0,p,-1,NULL,0);
	m_p = (wchar_t *)malloc(numWChar * sizeof(wchar_t));
	if(m_p)
		MultiByteToWideChar(CP_ACP,0,p,-1,m_p,numWChar);
#else
	m_p = p;
#endif
	return m_p;
}