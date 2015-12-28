#ifndef SUPPORT_LOADER_H
#define SUPPORT_LOADER_H

#include "dllLoader.h"

class ISystem;
class CSupportLoader : public CDllLoader
{
	FnSysShut * m_pFnSysShut;
	ISystem * m_pSystem;
public:
	CSupportLoader();
	~CSupportLoader();
	ISystem * GetSystem()const{return m_pSystem;}
	BOOL Load(LPCTSTR lpszLogFile, LPCTSTR lpszBasePath,LPCTSTR lpszExtraPath,LPCTSTR lpszEffcFile);
};

#endif