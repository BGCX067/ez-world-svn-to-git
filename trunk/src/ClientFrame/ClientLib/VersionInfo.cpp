#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件：
#include <windows.h>
#include <fstream>
#include "json\json.h"
#include "versioninfo.h"

void CVersionInfo::BuildString()
{
	TCHAR bufVersionTemp[MAX_PATH] = {0};
	_stprintf_s(bufVersionTemp,MAX_PATH,_T("%d.%d.%d.%d.%d"), m_nMajor, m_nMinor, m_nPatch, m_nProgram,m_nResource);
	m_strInfo = bufVersionTemp;
}

bool CJsonVersionInfo::Init()
{
	std::ifstream jsonis( m_lpszFile );
	if(!jsonis)
		return false;
	Json::Reader reader;
	Json::Value root(Json::objectValue);
	if(!root.isMember("Major") || !root.isMember("Minor") 
		|| !root.isMember("Patch") || !root.isMember("Program") 
		|| !root.isMember("Resource"))
		return false;

	m_nMajor = root["Major"].asInt();
	m_nMinor = root["Minor"].asInt();
	m_nPatch = root["Patch"].asInt();
	m_nProgram = root["Program"].asInt();
	m_nResource = root["Resource"].asInt();
	BuildString();
	return true;
}

bool CExeVersionInfo::Init()
{
	// 读取本EXE的版本属性。
	int nVersionLen = ::GetFileVersionInfoSize(m_lpszFile,NULL);
	if (nVersionLen <= 0)
	{
		return false;
	}

	TCHAR * pBuffer = new TCHAR[nVersionLen];
	if (!pBuffer)
	{
		return false;
	}

	if ( !::GetFileVersionInfo( m_lpszFile,  NULL,   nVersionLen,   pBuffer   ) )
	{
		return false;
	}

	struct LANGANDCODEPAGE {   
		WORD   wLanguage;   
		WORD   wCodePage;   
	} *lpTranslate;   

	//   Read   the   list   of   languages   and   code   pages.   
	UINT   cbTranslate;   
	::VerQueryValue( pBuffer, _T("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate);   

	//   dwBytes最大值应该是301.   
	//   应用仅仅允许输入最多300个字符：）   
	UINT   dwBytes;   
	TCHAR*   lpBuffer;   
	TCHAR   szSubBlock[256];
	TCHAR szVersion[20] = {0};

	_stprintf_s(szSubBlock,256,_T("\\StringFileInfo\\%04x%04x\\FileVersion"),lpTranslate[0].wLanguage,lpTranslate[0].wCodePage   );
	if(::VerQueryValue(pBuffer,szSubBlock,(LPVOID*)&lpBuffer,&dwBytes))   
	{   
		_tcscpy_s(szVersion,20, lpBuffer);
		_stscanf_s(szVersion,_T("%d.%d.%d.%d.%d"),20,&m_nMajor, &m_nMinor, &m_nPatch, &m_nProgram,&m_nResource);
		BuildString();
		return true;
	}
	delete[] pBuffer;
	return false;
}