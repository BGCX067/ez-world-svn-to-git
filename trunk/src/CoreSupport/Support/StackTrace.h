// StackTrace.H

// Author: Dr. Carlo Pescio
// Eptacom Consulting
// Via Bernardo Forte 2-3
// 17100 Savona - ITALY
// Fax +39-19-854761
// email pescio@programmers.net

#ifndef STACKTRACE_

#define STACKTRACE_


#include <windows.h>


#ifdef __BCPLUSPLUS__
  #define DLLEXPORT _export
#else
  #define DLLEXPORT __declspec( dllexport )
#endif


class IStackTraceCallback
{
public:
	virtual ~IStackTraceCallback() {};
	virtual void Output(LPEXCEPTION_POINTERS e,LPCTSTR lpszLogFile,LPCTSTR lpszDumpFile)const = 0;
};

// 必须经过有效配置才能正确提交崩溃报告.
extern "C" DLLEXPORT void InitStackTraceLibrary(const IStackTraceCallback * pCallBack) ;
extern "C" DLLEXPORT const IStackTraceCallback* GetStackTraceConfig();


#endif // #ifndef STACKTRACE_
