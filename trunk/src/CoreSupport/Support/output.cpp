//--------------------------------------------------------------------
//  Name:			output.cpp
//  Copyright:		unknown
//	Author:			Lighter.W
//  Date:			9:38 2003-5-29
//  Description:	输出字符串到文件
//--------------------------------------------------------------------

#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/sleep.h>
#include "Output.h"
#define OVERLOAD
#include "overload.h"

using namespace log4cplus::helpers;

CLog * Singleton<CLog>::ms_Singleton = NULL;

LogLevel CLogInstance::GetLevel(LogType eType)const
{
	LogLevel ll[LT_MAX] = {FATAL_LOG_LEVEL,ERROR_LOG_LEVEL,WARN_LOG_LEVEL,INFO_LOG_LEVEL,DEBUG_LOG_LEVEL};
	if(eType>=0 && eType<LT_MAX)
		return ll[eType];
	return INFO_LOG_LEVEL;
}
CLogInstance::CLogInstance(Logger logger)
	:m_Logger(logger)
{
}
bool CLogInstance::IsEnabledFor(LogType eType)const
{
	return m_Logger.isEnabledFor(GetLevel(eType));
}
void CLogInstance::Write(LogType eType,const TCHAR *str,const char* file, int line)
{
	m_Logger.forcedLog(GetLevel(eType),tstring(str),file,line);
}
void CLogInstance::WriteF(LogType eType,const char* file, int line,const TCHAR * _Format, ...)
{
	TCHAR buff[1024] = {_T('\0')};
	va_list ap;
	va_start(ap,_Format);
	vswprintf(buff,_Format,ap);
	va_end(ap);
	m_Logger.forcedLog(GetLevel(eType),tstring(buff),file,line);
}



CLog::CLog(const TCHAR * lpszConfig)
	:m_pRoot(NULL)
{
	if(lpszConfig)
	{
		LogLog::getLogLog()->setInternalDebugging(true);
		m_pRoot = new CLogInstance(Logger::getRoot());	

		PropertyConfigurator configure(lpszConfig);
		configure.configure();
	}
}

CLog::~CLog()
{
	std::map<t_string,CLogInstance*>::iterator i = m_mapInstances.begin();
	while(i != m_mapInstances.end())
	{
		delete i->second;
		i ++;
	}
	delete m_pRoot;
}

ILogInstance * CLog::GetInstance(const TCHAR * lpszName)
{
	std::map<t_string,CLogInstance*>::iterator i = m_mapInstances.find(lpszName);
	if(i != m_mapInstances.end())
	{
		return i->second;
	}
	else
	{
		CLogInstance * pLogger = new CLogInstance(Logger::getInstance(lpszName));
		std::pair<t_string,CLogInstance*> p(lpszName,pLogger);
		m_mapInstances.insert(p);
		return pLogger;
	}
	return NULL;
}
