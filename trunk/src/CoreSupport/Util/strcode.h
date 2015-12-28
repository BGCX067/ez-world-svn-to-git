#pragma once
#include <tchar.h>

class CMbsToWcs
{
	wchar_t * m_p;
public:
	CMbsToWcs(const char * p);
	CMbsToWcs();
	~CMbsToWcs();
	const wchar_t * Get()const{return m_p;} 
	const wchar_t * Conver(const char * p);
};

class CWcsToMbs
{
	char * m_p;
public:
	CWcsToMbs(const wchar_t * p);
	CWcsToMbs();
	~CWcsToMbs();
	const char * Get()const{return m_p;}
	const char * Conver(const wchar_t * p);
};

class CUtf8ToTchar
{
	wchar_t * m_pUstr;
	char * m_pAsin;
public:
	CUtf8ToTchar(const char * lpszStr);
	~CUtf8ToTchar();
	const TCHAR * GetString()const;
};

class CTcharToChar
{
	char * m_p;
public:
	CTcharToChar(const TCHAR * p);
	CTcharToChar();
	~CTcharToChar();
	const char * Get()const{return m_p;}
	const char * Conver(const TCHAR * p);
};
class CCharToTchar
{
	TCHAR * m_p;
public:
	CCharToTchar(const char * p);
	CCharToTchar();
	~CCharToTchar();
	const TCHAR * Get()const{return m_p;} 
	const TCHAR * Conver(const char * p);
};