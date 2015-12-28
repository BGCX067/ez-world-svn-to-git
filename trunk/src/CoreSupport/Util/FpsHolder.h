#pragma once

class CFpsHolder
{
	unsigned int m_dwLastTime;
	unsigned int m_dwTime;
	unsigned int m_dwStart;
	unsigned int m_nLoopCount;
	unsigned int m_nFPS;
public:
	CFpsHolder();
	void FrameStar();
	void FrameEnd(unsigned int nBeat);
	unsigned int GetFPS(){return m_nFPS;}
};