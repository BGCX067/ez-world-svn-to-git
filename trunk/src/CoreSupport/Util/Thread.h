#pragma once

//enum	_threadlevel_
//{
//	TL_IDLE = THREAD_PRIORITY_IDLE,
//	TL_REALTIME = (REALTIME_PRIORITY_CLASS<<16) | THREAD_PRIORITY_NORMAL,
//	TL_TIME_CRITICAL	=	THREAD_PRIORITY_TIME_CRITICAL,
//	TL_HIGHEST = (HIGH_PRIORITY_CLASS << 16)|THREAD_PRIORITY_HIGHEST,
//	TL_HIGH = (HIGH_PRIORITY_CLASS << 16 ) | THREAD_PRIORITY_NORMAL,
//	TL_NORMAL = (NORMAL_PRIORITY_CLASS << 16) | THREAD_PRIORITY_NORMAL,
//	TL_LOW = (NORMAL_PRIORITY_CLASS << 16 ) | THREAD_PRIORITY_LOWEST,
//	TL_LOWEST = (IDLE_PRIORITY_CLASS << 16) | THREAD_PRIORITY_NORMAL,
//};


class cThreadHandle
{
};
typedef DWORD (cThreadHandle::*ThreadProc)(LPVOID lpParameter);


class cThread
{
protected:
	LPVOID	m_lpParam;				// 线程参数

private:
	HANDLE	m_hThread;				// 线程句柄
	DWORD	m_dwThreadId;			// 线程ID

	// 线程处理类对象函数指针
	ThreadProc	m_pfnHandleProc;
	cThreadHandle * m_pThreadHandle;

	// 线程处理函数
	static DWORD _stdcall StaticThreadProc(LPVOID lpParam);

	void Clear();
	virtual DWORD ThreadFunc(LPVOID lpParameter);

public:
	static int m_nTreadCreated;
	inline LPVOID GetParam()
	{
		return m_lpParam;
	}
	cThread(void);
	virtual ~cThread(void);

	// 创建线程
	HANDLE Create(DWORD dwStackSize, cThreadHandle * pHandle, ThreadProc pfnHandleProc,LPVOID lpParam = NULL);

	HANDLE Create(DWORD dwStackSize=65536,LPVOID lpParam = NULL);

	BOOL	SetThreadPriority(int nPriority)
	{
		return::SetThreadPriority(m_hThread,nPriority);
	}

	// 销毁线程
	BOOL Destroy(void);

	// 暂停/继续线程
	DWORD Suspend();
	DWORD Resume();
	
	inline HANDLE GetHandle(){return m_hThread;}
	BOOL IsRunning();
};
