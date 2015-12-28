#ifndef __CS_H__
#define __CS_H__

#include <windows.h>

class CriticalSection
{
public:
	CriticalSection()
	{
		::InitializeCriticalSection(&m_cs);
	}

	virtual ~CriticalSection()
	{
		::DeleteCriticalSection(&m_cs);
	}

	CRITICAL_SECTION& GetCS()
	{
		return m_cs;
	}

	void Enter()
	{
		::EnterCriticalSection(&m_cs);
	}

	void Leave()
	{
		::LeaveCriticalSection(&m_cs);
	}

protected:
	CRITICAL_SECTION m_cs;
};

class CSLocker
{
public:
	CSLocker(const CRITICAL_SECTION& cs)
	{
		m_pcs = (LPCRITICAL_SECTION)&cs;
		::EnterCriticalSection(m_pcs);
		//_printf("锁定临界区[ %d ]\n", (int)m_pcs);
	}

	virtual ~CSLocker()
	{
		::LeaveCriticalSection(m_pcs);
		//_printf("释放临界区[ %d ]\n", (int)m_pcs);
	}

private:
	CSLocker(){}
	CSLocker(const CSLocker& locker){}
	CSLocker& operator=(const CSLocker& locker) { return *this; }
private:
	LPCRITICAL_SECTION m_pcs;

};

#define CSGUARD  CSLocker lock(this->GetCS())

#endif//__CS_H__