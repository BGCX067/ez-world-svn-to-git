//efficiency.cpp
#include "Support.h"
#include "efficiency.h"
#include <shlwapi.h>
#include <assert.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////
CEffcRecord::CEffcRecord()
	:m_TotelCount(0)
	,m_FrameCount(0)
{
	m_TotelTime.QuadPart = 0;
	m_FrameTime.QuadPart = 0;
}
CEffcRecord::CEffcRecord(CEffcRecord * e)
{
}
void CEffcRecord::Update()
{
	m_TotelCount += m_FrameCount;
	m_TotelTime.QuadPart += m_FrameTime.QuadPart;
	m_FrameCount = 0;
	m_FrameTime.QuadPart = 0;
}
void CEffcRecord::DumpFrame(FILE * fp,int nID,const TCHAR * pString,SYSTEMTIME& systemTime,LARGE_INTEGER& m_CpuFreq)
{
	if(m_FrameCount)
	{
		_ftprintf(fp,_T("%d\t[%02d:%02d:%02d:%03d]\tFrame\t%s\t%d\t%I64u\t%d\n"),
			nID,
			systemTime.wHour,systemTime.wMinute,systemTime.wSecond,systemTime.wMilliseconds,
			pString,
			m_FrameCount,
			m_FrameTime,
			m_FrameTime.QuadPart * 1000/m_CpuFreq.QuadPart
			);
	}
}
void CEffcRecord::DumpAll(FILE * fp,int nID,const TCHAR * pString,SYSTEMTIME& systemTime,LARGE_INTEGER& m_CpuFreq)
{
	if(m_TotelCount)
	{
		fprintf(fp,"%d\t[%02d:%02d:%02d:%03d]\tTotal\t%s\t%d\t%I64u\t%d\n",
			nID,
			systemTime.wHour,systemTime.wMinute,systemTime.wSecond,systemTime.wMilliseconds,
			pString,
			m_TotelCount,
			m_TotelTime,
			m_TotelTime.QuadPart * 1000/m_CpuFreq.QuadPart
			);
	}
}
void CEffcRecord::Begin()
{
	QueryPerformanceCounter(&m_BeginTimeMs);
	m_FrameCount ++;
}
void CEffcRecord::End()
{
	QueryPerformanceCounter(&m_EndTimeMs);
	m_FrameTime.QuadPart += (m_EndTimeMs.QuadPart - m_BeginTimeMs.QuadPart);
}


CEfficiency::CEfficiency(LPCTSTR lpszFileName)
	:m_fp(NULL)
	,m_nID(0)
	,m_bEnable(true)
{
	QueryPerformanceFrequency(&m_CpuFreq);

	// 把文件
	TCHAR szFileName[MAX_PATH] = {0};
	_tcscpy(szFileName,lpszFileName);
	PathRenameExtension(szFileName,_T(".txt"));
	if((m_fp = _tfopen(szFileName, _T("w+")))==NULL)
	{
		throw(std::exception("CEfficiency::CEfficiency",1));
	}
	_ftprintf(m_fp,_T("ID\t时间\t类型\t节点\t次数\tCPU周期\t毫秒\n"));
}

CEfficiency::~CEfficiency()
{
	m_RecordMap.clear();
	if(m_fp)
		fclose(m_fp);
}

void CEfficiency::update()
{
	for (RecordMap::iterator it = m_RecordMap.begin(); it != m_RecordMap.end(); ++it) {
		CEffcRecord& node = it->second;
		node.Update();
	}
}

IEffcRecord * CEfficiency::GetRecord(const TCHAR * Name)
{
	if (Name)
	{
		RecordMap::iterator it = m_RecordMap.find(Name);
		if (it == m_RecordMap.end())
		{
			m_RecordMap.insert(RecordMap::value_type(Name, CEffcRecord()));
			it = m_RecordMap.find(Name);
		}
		CEffcRecord& node = it->second;
		return &node;
	}
	return NULL;
}
void CEfficiency::DumpFrame()
{
	m_nID ++;

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	for (RecordMap::iterator it = m_RecordMap.begin(); it != m_RecordMap.end(); ++it) 
	{
		CEffcRecord& node = it->second;
		const TCHAR * pString = it->first.c_str();
		node.DumpFrame(m_fp,m_nID,pString,systemTime,m_CpuFreq);
	}
	fflush(m_fp);
}
void CEfficiency::DumpAll()
{
	m_nID ++;

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	for (RecordMap::iterator it = m_RecordMap.begin(); it != m_RecordMap.end(); ++it) 
	{
		CEffcRecord& node = it->second;
		const TCHAR * pString = it->first.c_str();
		node.DumpAll(m_fp,m_nID,pString,systemTime,m_CpuFreq);
	}
	fflush(m_fp);
}