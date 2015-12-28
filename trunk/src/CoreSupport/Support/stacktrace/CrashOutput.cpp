#include <Windows.h>
#include <time.h>
#include <shlwapi.h>
#include <DbgHelp.h>
#include "../CrashOutput.h"

#pragma comment(lib, "shlwapi")
#pragma comment(lib, "dbghelp")

//StackTraceInitializer stackTraceInitializer;
StackTraceInitializer *Singleton<StackTraceInitializer>::ms_Singleton=NULL;

SUPPORT_DLL_CLASS StackTraceInitializer::StackTraceInitializer(MINIDUMP_TYPE mdt):m_mdt(mdt)
{ 
	InitializeCriticalSection( &cs );
	m_pStackTraceCallBack = NULL;
	stackTraceSync = &cs;
	//g_psti=this;

#ifndef _DEBUG
	// this can be safely done multiple times
	SetUnhandledExceptionFilter( StackTraceExceptionFilter ) ;
	DisableSetUnhandledExceptionFilter();
#endif
}


void StackTraceInitializer::DisableSetUnhandledExceptionFilter()
{
	void *addr = (void*)GetProcAddress(LoadLibrary(L"kernel32.dll"),"SetUnhandledExceptionFilter");
	if (addr) 
	{
		unsigned char code[16];
		int size = 0;
		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC2;
		code[size++] = 0x04;
		code[size++] = 0x00;

		BOOL bRet = FALSE;
		DWORD dwOldFlag, dwTempFlag;
		bRet = VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
		bRet = WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
		bRet = VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
	}
}

LONG StackTraceInitializer::StackTraceExceptionFilter( LPEXCEPTION_POINTERS e )
{
	StackTraceInitializer *psti=StackTraceInitializer::getSingletonPtr();
	LONG r;
	EnterCriticalSection( &psti->cs);
	
	// _asm int 3 的错误继续由别的错误模块处理
	if(e->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		r = EXCEPTION_CONTINUE_SEARCH;
	}
	else
	{
		r = EXCEPTION_EXECUTE_HANDLER;
	}
	psti->KeDumpDebugger(e);
	LeaveCriticalSection(&psti->cs);
	return r;
}

//==============================================================================
//    定义常用的宏
#define ERR_FAILURE 0xFFFFFFFF
#define ERR_SUCCESS 0x00000000

//==============================================================================
//    定义默认的输出文件名称
static const TCHAR szMiniDump[] = { TEXT("Crash.DMP") };

LONG StackTraceInitializer::KeDumpDebugger(PEXCEPTION_POINTERS ExceptionInfo)
{
	DWORD dwAttr = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH;
	HANDLE hFile = CreateFile(szMiniDump,GENERIC_WRITE,0,0,CREATE_ALWAYS,dwAttr,0);
	if(hFile == INVALID_HANDLE_VALUE) 
		return 0;
	
	MINIDUMP_EXCEPTION_INFORMATION eInfo;
	eInfo.ThreadId = GetCurrentThreadId();
	eInfo.ExceptionPointers = ExceptionInfo;
	eInfo.ClientPointers = FALSE;

	// 准备进程相关参数
	HANDLE hProc = GetCurrentProcess();
	DWORD nProc = GetCurrentProcessId();
	PMINIDUMP_EXCEPTION_INFORMATION pMDI = ExceptionInfo ? &eInfo : NULL;
	BOOL bDump = MiniDumpWriteDump(hProc,nProc,hFile,m_mdt,pMDI,NULL,NULL);
	
	CloseHandle( hFile );

	if(m_pStackTraceCallBack)
	{
		m_pStackTraceCallBack->Output(ExceptionInfo,NULL,szMiniDump);
	}
	else
	{
		TCHAR szMessage[MAX_PATH] = {0};
		_stprintf(szMessage,_T("不是崩溃是寂寞,异常地址 = 0x%08x.赶紧召唤程序,有火锅请."),ExceptionInfo->ExceptionRecord->ExceptionAddress);
		MessageBox(NULL,szMessage,_T("哥来教你C语言"),MB_OK|MB_ICONERROR);
	}
	return bDump ? ERR_SUCCESS : ERR_FAILURE ;
}

