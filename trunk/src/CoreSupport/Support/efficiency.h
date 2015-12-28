//efficiency.h
#pragma once

#include <map>
#include "tstring.h"


class CEffcRecord : public IEffcRecord
{
	LARGE_INTEGER m_BeginTimeMs;// 统计的开始时间
	LARGE_INTEGER m_EndTimeMs;	// 统计的结束时间
	LARGE_INTEGER m_TotelTime;	// 从开始统计运行到现在总共时间
	LARGE_INTEGER m_FrameTime;	// 本桢的时间统计
	int m_TotelCount;			// 从开始统计运行到现在,节点被调用的次数
	int m_FrameCount;			// 在一桢内,节点被调用的次数

public:
	CEffcRecord();
	CEffcRecord(CEffcRecord * e);
	void Update();
	void DumpFrame(FILE * fp,int nID,const TCHAR * pString,SYSTEMTIME& systemTime,LARGE_INTEGER& m_CpuFreq);
	void DumpAll(FILE * fp,int nID,const TCHAR * pString,SYSTEMTIME& systemTime,LARGE_INTEGER& m_CpuFreq);
	
	void Begin();
	void End();
};

/** \ingroup Engine
  * \brief 效率评估设备的实现类
  *
  * Describe:这个类实现了效率评估设备的接口
  */
class CEfficiency : public IEfficiency
{
	typedef std::pair<t_string,CEffcRecord> RecordUnit;
	typedef std::map<t_string,CEffcRecord> RecordMap;
	RecordMap m_RecordMap;

	LARGE_INTEGER m_CpuFreq;

	bool m_bEnable;

	FILE * m_fp;
	int m_nID;

public:
	CEfficiency(LPCTSTR lpszFileName);
	~CEfficiency();


	void Enable(bool bEnable) { m_bEnable = bEnable; }
	bool IsEnable() const { return m_bEnable; }
	void update();
	void DumpFrame();
	void DumpAll();
	IEffcRecord * GetRecord(const TCHAR * Name);
};
