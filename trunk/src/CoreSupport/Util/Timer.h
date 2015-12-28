#pragma once
namespace UTIL
{

	class CTimer
	{
		unsigned __int64	m_start;			// performance timer start
		double	m_resolution;			// timer resolution
	public:
		CTimer(void);
		double GetRelativeTime();
		double GetSystemTime();
		void Reset();
	};

}