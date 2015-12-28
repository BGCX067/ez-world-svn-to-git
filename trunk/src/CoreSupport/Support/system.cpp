/*===========================================================================
	Title: TSystem.cpp
	Author: Ignacio Casta駉 Aguado
	Description: 基本系统类
	Classes:
		TSystem
===========================================================================*/
#include <io.h>
#include <shlwapi.h>
#include <locale.h>

#define USING_STRSTREAM
#include "tstring.h"

#include "support.h"

#include "output.h"
#include "memmgr.h"
#include "vfile.h"
#include "efficiency.h"

#include "system.h"
#define OVERLOAD
#include "overload.h"

/*---------------------------------------------------------------------------
	Definitions
---------------------------------------------------------------------------*/
CSystem * Singleton<CSystem>::ms_Singleton = NULL;
IMemManager * pMemManager = NULL;;
/*---------------------------------------------------------------------------
	Methods
---------------------------------------------------------------------------*/
bool CSystem::GetDataPath(LPCTSTR lpszData,TCHAR * lpszOut)
{
	// 取得当前工作路径
	TCHAR szWorkPath[MAX_PATH];
	DWORD dwRet = GetEnvironmentVariable(_T("DATA_PATH"),szWorkPath,MAX_PATH);
	if(dwRet == 0)
		::GetCurrentDirectory(MAX_PATH,szWorkPath);

	TCHAR * lpszNext = szWorkPath;
	while(lpszNext)
	{
		// 检查所在的目录是否存在数据目录,存在则直接返回
		TCHAR szDataPack[MAX_PATH] = {0};
		_stprintf(szDataPack,_T("%s\\%s.pk3"),szWorkPath,lpszData);
		if(PathFileExists(szDataPack) && !PathIsDirectory(szDataPack))
		{
			_stprintf(lpszOut,_T("%s\\%s\\"),szWorkPath,lpszData);
			return true;
		}

		_stprintf(szDataPack,_T("%s\\%s\\"),szWorkPath,lpszData);
		if(PathFileExists(szDataPack) && PathIsDirectory(szDataPack))
		{
			TCHAR szDataFlag[MAX_PATH] = {0};
			_stprintf(szDataFlag,_T("%s\\%s\\_Is_DataPacket"),szWorkPath,lpszData);
			if(PathFileExists(szDataFlag) && !PathIsDirectory(szDataFlag))
			{
				_tcscpy(lpszOut,szDataPack);
				return true;
			}
		}

		lpszNext = _tcsrchr(szWorkPath,_T('\\'));
		if(lpszNext)
			szWorkPath[_tcslen(szWorkPath)-_tcslen(lpszNext)] = 0;
	}
	return false;
}

CSystem::CSystem(LPCTSTR lpszLogFile,LPCTSTR lpszBasePath,LPCTSTR lpszExtraPath,LPCTSTR lpszEffcFile)
	:m_pLogSys(NULL)
	,m_pFiles(NULL)
	,m_pEfficiency(NULL)
	,m_pLog(NULL)
{
	pMemManager = &m_Mem;
	try
	{
		if(lpszLogFile)
		{
			m_pLogSys = new CLog(lpszLogFile);
			m_pLog = m_pLogSys->GetRoot();
			VersionString();
		}
		if(lpszBasePath)
		{
			LOG_INFO(m_pLog,_T("CSystem::Init():Setting up System Env..."));
			TCHAR szDataPath[MAX_PATH]={0},szExtraPath[MAX_PATH]={0};
			if(!GetDataPath(lpszBasePath,szDataPath))
			{
				LOG_ERROR(m_pLog,_T("CSystem::Init():Cannot Found Path [")<<lpszBasePath<<_T("],Setting up System Env Failed."));
				throw(std::exception("CSystem::CSystem",2));
			}

			GetDataPath(lpszExtraPath,szExtraPath);
			m_pFiles = new CSysFiles(szDataPath,szExtraPath);
			LOG_INFO(m_pLog,_T("CSystem::Init():Setting up System Env Succeed."));
		}
		if(lpszEffcFile)
		{
			m_pEfficiency = new CEfficiency(lpszEffcFile);
			//LOG_ERROR(m_pLog,_T("CSystem::Init():efficiency.OpenFile")<<lpszEffcFile<<_T("error"));
		}
	}
	catch(std::exception e)
	{
		LOG_ERROR(m_pLog,e.what());
		LOG_ERROR(m_pLog,_T("CSystem::Init():Setting up System Env failed."));
		throw(std::exception("CSystem::CSystem",1)); 
	}
}


CSystem::~CSystem()
{
	delete m_pEfficiency;
	delete m_pFiles;
	delete m_pLogSys;
	pMemManager = NULL;
}
void CSystem::VersionString()
{
	TCHAR LocaleName[LOCALE_NAME_MAX_LENGTH] = {0};
	LCID nlcid = GetSystemDefaultLCID();
	int nRet = GetLocaleInfo(nlcid,LOCALE_SABBREVLANGNAME,LocaleName,LOCALE_NAME_MAX_LENGTH);
	_tsetlocale( LC_CTYPE, _T("chs") );

	t_stringstream sVersion,sLanguage;
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if(!(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO*) &osvi)))
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) )
		{
			LOG_ERROR(m_pLog,_T("CSystem::VersionString():GetVersionEx failed."));
			return;
		}
	}

	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		if (osvi.dwMajorVersion <= 4)
		{
			sVersion << _T("Microsoft Windows NT ");
		}
		else if (osvi.dwMajorVersion == 5)
		{
			switch(osvi.dwMinorVersion)
			{
			case 0:
				sVersion << _T("Microsoft Windows 2000 ");
				break;
			case 1:
				sVersion << _T("Microsoft Windows XP ");
				break;
			case 2:
				sVersion << _T("Microsoft Windows Server 2003 ");
				break;
			}
		}
		else if (osvi.dwMajorVersion == 6)
		{
			if( osvi.wProductType == VER_NT_WORKSTATION )
				sVersion << _T("Windows Vista ");
			else 
				sVersion << _T("Windows Server 2008 ");
		}


		if( bOsVersionInfoEx )
		{
#ifdef VER_SUITE_ENTERPRISE
			if (osvi.wProductType == VER_NT_WORKSTATION)
			{
				if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
					sVersion << _T("Personal ");
				else
					sVersion << _T("Professional ");
			}
			else if (osvi.wProductType == VER_NT_SERVER)
			{
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					sVersion << _T("DataCenter Server ");
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					sVersion << _T("Advanced Server ");
				else
					sVersion << _T("Server ");
			}
#endif
		}
		else
		{
			HKEY hKey;
			TCHAR szProductType[80];
			DWORD dwBufLen;

			RegOpenKeyEx( HKEY_LOCAL_MACHINE,_T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),0, KEY_QUERY_VALUE, &hKey );
			RegQueryValueEx( hKey, _T("ProductType"), NULL, NULL,(LPBYTE) szProductType, &dwBufLen);
			RegCloseKey( hKey );

			if (lstrcmpi( _T("WINNT"), szProductType) == 0 )
				sVersion << _T("Professional ");
			if ( lstrcmpi( _T("LANMANNT"), szProductType) == 0 )
				sVersion << _T("Server ");
			if ( lstrcmpi( _T("SERVERNT"), szProductType) == 0 )
				sVersion << _T("Advanced Server ");
		}

		// Display version, service pack (if any), and build number.

		if (osvi.dwMajorVersion <= 4 )
		{
			sVersion << _T("version ") 
				<< osvi.dwMajorVersion
				<< _T(".")
				<< osvi.dwMinorVersion
				<< _T(".")
				<< osvi.szCSDVersion
				<< _T(" (Build ")
				<< (DWORD)(osvi.dwBuildNumber & 0xFFFF)
				<< _T(")");
		}
		else
		{
			sVersion << osvi.szCSDVersion
				<< _T(" (Build ")
				<< (DWORD)(osvi.dwBuildNumber & 0xFFFF)
				<< _T(")");
		}
		break;

	case VER_PLATFORM_WIN32_WINDOWS:

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			sVersion << _T("Microsoft Windows 95 ");
			if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
				sVersion << _T("OSR2 " );
		}

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			sVersion << _T("Microsoft Windows 98 ");
			if ( osvi.szCSDVersion[1] == 'A' )
				sVersion << _T("SE ");
		}

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
			sVersion << _T("Microsoft Windows Me ");

		break;

	case VER_PLATFORM_WIN32s:
		sVersion << _T("Microsoft Win32s ");
		break;
	}
	LOG_INFO(m_pLog,sVersion.str().c_str());

	sLanguage << _T("Local=0x");
	sLanguage << _itot(nlcid,LocaleName,16);
	sLanguage << _T(";");
	sLanguage << LocaleName;
	LOG_INFO(m_pLog,sLanguage.str().c_str());
}

ISystem * SysInit(LPCTSTR lpszLogFile, LPCTSTR lpszBasePath,LPCTSTR lpszExtraPath,LPCTSTR lpszEffcFile)
{
	try
	{
		static CSystem sys(lpszLogFile,lpszBasePath,lpszExtraPath,lpszEffcFile);
		return &sys;
	}
	catch(std::exception e)
	{
		return NULL;
	}
}
void SysShut(ISystem * pSystem)
{
}
