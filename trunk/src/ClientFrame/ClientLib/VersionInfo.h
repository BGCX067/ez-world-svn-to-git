#pragma once

#include "tstring.h"

class CVersionInfo
{
protected:
	int m_nMajor;
	int m_nMinor;
	int m_nPatch;
	int m_nProgram;
	int m_nResource;
	t_string m_strInfo;

	void BuildString();
public:
	inline void GetVersion(int &nMajor,int &nMinor,int &nPatch) const{nMajor=m_nMajor;nMinor=m_nMinor;nPatch=m_nPatch;}
	inline const TCHAR * GetVersionInfo() const {return m_strInfo.c_str();}
	inline int GetProgramVersion()const {return m_nProgram;}
	virtual bool Init() = 0;
};

class CJsonVersionInfo : public CVersionInfo
{
	const TCHAR * m_lpszFile;
public:
	CJsonVersionInfo(const TCHAR * lpszFile)
		:m_lpszFile(lpszFile)
	{
	}
	bool Init();
};

class CExeVersionInfo : public CVersionInfo
{
	const TCHAR * m_lpszFile;
public:
	CExeVersionInfo(const TCHAR * lpszFile)
		:m_lpszFile(lpszFile)
	{
	}
	bool Init();
};