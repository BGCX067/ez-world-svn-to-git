/*===========================================================================
	Title:			TSystem.h
	Author:			Ignacio Casta駉 Aguado
	Description:	系统基本类
	Classes:
		TSystem
===========================================================================*/
#ifndef _SYSTEM_INCLUDE
#define _SYSTEM_INCLUDE
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <Windows.h>
#include "Singleton.h"

/*---------------------------------------------------------------------------
	Framework
---------------------------------------------------------------------------*/
/*
	The framework consist of a memory allocator, an output device, an error manager,
	a timer, and variable system.
*/

class CSystem : public Singleton<CSystem>,public ISystem
{
private:
	CSysMemory		m_Mem;			// 内存管理
	CLog		*	m_pLogSys;		// LOG输出
	CSysFiles	*	m_pFiles;		// 文件系统
	CEfficiency	*	m_pEfficiency;	// 效率评估工具
	ILogInstance *	m_pLog;

	bool GetDataPath(LPCTSTR lpszBasePath,TCHAR* lpszOut);
	void VersionString();
public:
	CSystem(LPCTSTR lpszLogFile,LPCTSTR lpszBasePath,LPCTSTR lpszExtraPath,LPCTSTR lpszEffcFile);
	~CSystem();

	ILog			* GetSysLog(){return m_pLogSys;}
	IMemManager		* GetSysMem(){return &m_Mem;}
	IVirtualFile	* GetSysVfs(){return m_pFiles;}
	IEfficiency		* GetSysEfficiency(){return m_pEfficiency;}
};


#endif // _FRAMEWORK_INCLUDE