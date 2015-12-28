#pragma once

#include <dbghelp.h>
#include "StackTrace.h"
#include "Singleton.h"
#include "support.h"

// uses a static object to guarantee that
// InitStackTraceLibrary is called at the
// beginning of the program, but after the
// RTL has installed its own filter
//class StackTraceInitializer;
//extern StackTraceInitializer *g_psti;
class StackTraceInitializer :public Singleton<StackTraceInitializer>
{
	const IStackTraceCallback * m_pStackTraceCallBack;

	// Needed for thread-safe access to DumpBuffer and
	// other static stuff in other parts of the code
	CRITICAL_SECTION cs ;
	CRITICAL_SECTION* stackTraceSync;

	// implemented in VerbExcept.CPP
	static LONG WINAPI StackTraceExceptionFilter( LPEXCEPTION_POINTERS e ) ;
	LONG KeDumpDebugger(PEXCEPTION_POINTERS ExceptionInfo);
	void DisableSetUnhandledExceptionFilter();

	MINIDUMP_TYPE	m_mdt;

	StackTraceInitializer()
	{
	}

public :
	SUPPORT_DLL_CLASS StackTraceInitializer(MINIDUMP_TYPE mdt);

	SUPPORT_DLL_CLASS ~StackTraceInitializer()
	{
	    DeleteCriticalSection(&cs );
	}

	CRITICAL_SECTION * GetStackTraceSync(){return stackTraceSync;}
	void SetStackTraceCallBack(const IStackTraceCallback * pCallBack)
	{
		m_pStackTraceCallBack = pCallBack;		
	}
	const IStackTraceCallback* GetStackTraceConfig()
	{
		return m_pStackTraceCallBack;
	}
};
