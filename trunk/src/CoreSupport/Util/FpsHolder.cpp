#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ���
#include <windows.h>
#include "FpsHolder.h"

CFpsHolder::CFpsHolder()
{
	m_dwLastTime = GetTickCount();
	m_dwTime = 0;
	m_dwStart = 0;
	m_nLoopCount = 0;
	m_nFPS = 0;
}

void CFpsHolder::FrameStar()
{
	m_dwStart = GetTickCount();
}

void CFpsHolder::FrameEnd(unsigned int nBeat)
{
	m_dwTime = GetTickCount();
	Sleep(((m_dwTime-m_dwStart)<nBeat)?(nBeat-m_dwTime+m_dwStart):1);

	// ����ͳ��
	m_nLoopCount ++;
	if((m_dwTime - m_dwLastTime) > 1000)
	{
		InterlockedExchange((LONG*)&m_nFPS, m_nLoopCount);
		m_nLoopCount = 0;
		m_dwLastTime = m_dwTime;
	}
}