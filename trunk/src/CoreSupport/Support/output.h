//--------------------------------------------------------------------
//  Name:			output.h
//  Copyright:		unknown
//	Author:			Lighter.W
//  Date:			9:38 2003-5-29
//  Description:	输出字符串到文件
//--------------------------------------------------------------------

#ifndef _OUTPUTDUMP_H
#define _OUTPUTDUMP_H

#include <map>
#include <log4cplus/logger.h>
#include "tstring.h"
#include "Singleton.h"
#include "support.h"

using namespace log4cplus;

//
// 插入一个字符串到文件
//
#define MAXCONLINELEN	1024		// maximum length for console string

class CLogInstance : public ILogInstance
{
	Logger m_Logger;
	LogLevel GetLevel(LogType eType)const;
public:
	CLogInstance(Logger logger);
	~CLogInstance(){}
	bool IsEnabledFor(LogType eType)const;
	void Write(LogType eType,const TCHAR *str,const char* file, int line);
	void WriteF(LogType eType,const char* file, int line,const TCHAR * _Format, ...);
};


class CLog : public Singleton<CLog>, public ILog 
{
	std::map<t_string,CLogInstance*> m_mapInstances;
	CLogInstance * m_pRoot;
public:
	CLog(const TCHAR * lpszConfig);
	~CLog();
	
	ILogInstance * GetInstance(const TCHAR * lpszName);
	ILogInstance * GetRoot()const{return m_pRoot;}
};

#endif