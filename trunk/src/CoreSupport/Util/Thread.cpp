#define	WIN32_LEAN_AND_MEAN
#ifndef _WIN32_WINNT
	#define _WIN32_WINNT	0x0501
#endif
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <process.h>
#include "thread.h"

int cThread::m_nTreadCreated = 0;

void cThread::Clear()
{
	m_hThread = NULL;
	m_dwThreadId = 0;
	m_pfnHandleProc = NULL;
	m_pThreadHandle = NULL;
}

cThread::cThread(void)
{
	Clear();
}

cThread::~cThread(void)
{
//	if(m_hThread)
//		Destroy(); 
}

DWORD cThread::ThreadFunc(LPVOID lpParameter)
{
	return 0;
}


DWORD _stdcall cThread::StaticThreadProc(LPVOID lpParam)
{
	DWORD	dwRet;
	cThread * pThread = (cThread *)lpParam;
	if(pThread->m_pfnHandleProc)
		dwRet = (pThread->m_pThreadHandle->*(pThread->m_pfnHandleProc))(pThread->m_lpParam); 
	else
		return pThread->ThreadFunc(pThread->m_lpParam);
	return dwRet;
}

HANDLE cThread::Create(DWORD dwStackSize,LPVOID lpParam )
{
	m_hThread = (HANDLE)_beginthreadex( NULL, dwStackSize, ( unsigned (_stdcall *)(void *) )StaticThreadProc, this, 0 , (unsigned *)&m_dwThreadId );
	if( m_hThread == NULL )
		return NULL;
	//ResumeThread(m_hThread);
	m_nTreadCreated ++;
	return m_hThread;
}

HANDLE cThread::Create(DWORD dwStackSize, cThreadHandle * pHandle, ThreadProc pfnHandleProc, LPVOID lpParam)
{
	m_hThread = (HANDLE)_beginthreadex( NULL, dwStackSize, ( unsigned (_stdcall *)(void *) )StaticThreadProc, this, CREATE_SUSPENDED , (unsigned *)&m_dwThreadId );
	if( m_hThread == NULL )
		return NULL;

	//if( dwThreadPriorityLevel != TL_NORMAL )
	//{
	//	int	iClass = (dwThreadPriorityLevel & 0xffff0000)>>16;
	//	int	iLevel = dwThreadPriorityLevel & 0xffff;
	//	if( iClass != 0 )
	//	{
			//SetPriorityClass(m_hThread, (DWORD)iClass);		//commaneted by qiuxinyu why use thread handle?
	//	}
	//	SetThreadPriority( m_hThread, iLevel );
	//}

	m_pThreadHandle = pHandle;
	m_pfnHandleProc = pfnHandleProc;
	m_lpParam = lpParam;

	ResumeThread(m_hThread);
	m_nTreadCreated ++;
	return m_hThread;
}

// 暴力结束线程
BOOL cThread::Destroy(void)
{
	if( WaitForSingleObject( m_hThread, 100000 )==WAIT_TIMEOUT)
	{
		assert(0);
		TerminateThread( m_hThread, 0 );
	}
	BOOL bRet = CloseHandle( m_hThread );
	Clear();
	m_nTreadCreated --;
	return TRUE;
}

BOOL cThread::IsRunning()
{
	if(m_hThread)
	{
		if( WaitForSingleObject( m_hThread, 0 )==WAIT_TIMEOUT)
			return TRUE;
	}
	return  FALSE;
}


DWORD cThread::Suspend()
{
	if(m_hThread)
		return ::SuspendThread(m_hThread);
	return -1;
}
DWORD cThread::Resume()
{
	if(m_hThread)
		return ::ResumeThread(m_hThread);
	return -1;
}

/*
BOOL cThread::QueueUserAPC( PAPCFUNC pfnAPC,ULONG_PTR dwData)
{
	m_lpParam=(LPVOID)dwData;
	return ::QueueUserAPC(pfnAPC,m_hThread,(ULONG_PTR)this);
}
*/
