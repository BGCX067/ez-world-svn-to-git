#ifndef _WIN32_WINNT            // 指定要求的最低平台是 Windows Vista。
#define _WIN32_WINNT 0x0600     // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include "FiberPool.h"
#include "Fiber.h"

CFiberPool * Singleton <CFiberPool>::ms_Singleton = 0;

CFiberPool::CFiberPool(UINT nMaxFibers)
	:m_nMaxFibers(nMaxFibers)
	,m_pMainFiber(NULL)
	,m_ppFibers(NULL)
{
}
CFiberPool::~CFiberPool()
{
}

BOOL CFiberPool::Create(CFiberFactory * pFactor,SIZE_T stack)
{
	m_pMainFiber = ::ConvertThreadToFiber(NULL);
	if(!m_pMainFiber)
		return FALSE;
	m_ppFibers = new CFiber * [m_nMaxFibers];
	for(UINT n=0;n<m_nMaxFibers;n++)
	{
		m_ppFibers[n] = pFactor->Create(n,m_pMainFiber,stack);
		if(m_ppFibers[n]->GetFiber()==NULL)
			return FALSE;
		m_FreeFibers.push(n);
	}
	return TRUE;
}
void CFiberPool::Destroy()
{
	for(UINT n=0;n<m_nMaxFibers;n++)
	{
		delete m_ppFibers[n];
	}
	::ConvertFiberToThread();
	m_pMainFiber = NULL;
}

CFiber * CFiberPool::AllocFiber()
{
	if(!m_FreeFibers.empty())
	{
		UINT nIndex = m_FreeFibers.front();
		m_FreeFibers.pop();
		m_setRunning.insert(nIndex);
		return m_ppFibers[nIndex];
	}
	return NULL;
}
void CFiberPool::FreeFiber(UINT nID)
{
	m_FreeFibers.push(nID);
	std::set<UINT>::iterator i = m_setRunning.find(nID);
	if(i != m_setRunning.end())
		m_setRunning.erase(i);
}
CFiber * CFiberPool::GetFiber(UINT nID)
{
	return m_ppFibers[nID];
}
CFiber * CFiberPool::GetRunningFiber(UINT nID)
{
	std::set<UINT>::iterator i = m_setRunning.find(nID);
	if(i != m_setRunning.end())
		return m_ppFibers[nID];
	return FALSE;
}
