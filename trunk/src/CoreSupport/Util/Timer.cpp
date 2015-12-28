#include <windows.h>
#include ".\timer.h"

namespace UTIL
{

CTimer::CTimer(void)
{
	__int64 frequency;	// The m_resolution>1000.01 counter frequency

	// check for m_resolution>1000.01 counter
	if(QueryPerformanceFrequency((LARGE_INTEGER *) &frequency))
	{
		// Use the m_resolution>1000.01 counter to get the present time
		QueryPerformanceCounter((LARGE_INTEGER *) &m_start);
		// calculate the m_resolution from the frequency
		m_resolution=(double)frequency;
	}else{	
		// Get the present time
		m_start=(unsigned __int64)timeGetTime();
		// calculate the m_resolution
		m_resolution=1000.0;
	}
}

double CTimer::GetRelativeTime()
{
	// get the time from the available counter
	if(m_resolution>1000.01)
	{
		__int64 time;			// the current time

		// get the time
		QueryPerformanceCounter((LARGE_INTEGER *) &time);
		// calculate it in milliseconds
		return ( (time-m_start)/m_resolution );
	}else
		return ( (timeGetTime()-m_start)/m_resolution );
}

double CTimer::GetSystemTime()
{
	if(m_resolution>1000.01)
	{
		unsigned __int64 time;			// the current time

		// get the time
		QueryPerformanceCounter((LARGE_INTEGER *) &time);
		return ( (time)/m_resolution );
	}else
		return ( timeGetTime()/m_resolution );
}

void CTimer::Reset()
{
	if(m_resolution>1000.01)
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &m_start);
	}else{	
		// Get the present time
		m_start=(unsigned __int64)timeGetTime();
	}
}

}