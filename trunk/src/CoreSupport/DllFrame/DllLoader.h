#ifndef DLL_LOADER_H
#define DLL_LOADER_H
#include "EzModule.h"

class CDllLoader
{
protected:
	HMODULE	m_hDll;
public:
	CDllLoader();
	virtual ~CDllLoader();
	BOOL Load(const TCHAR * lpszDllName);
};

class ISystem;
template<typename T> class CModuleLoader :public CDllLoader
{
	T * m_pInterface;
	FnCreateModule * m_fnCreator;
	FnDestroyModule * m_fnDestroyer;

public:
	CModuleLoader()
		:m_pInterface(NULL)
		,m_fnCreator(NULL)
		,m_fnDestroyer(NULL)
	{
	}
	~CModuleLoader()
	{
		if(m_fnDestroyer)
		{
			iModule * pModule = (iModule *)m_pInterface;
			m_fnDestroyer(pModule);
		}
	}
	BOOL Load(const TCHAR * lpszDllName,const iInitParam * pInitParam,ISystem * pSystem)
	{
		if(CDllLoader::Load(lpszDllName))
		{
			m_fnCreator = (FnCreateModule *)GetProcAddress( m_hDll, CreateModule_FN);
			m_fnDestroyer = (FnDestroyModule *)GetProcAddress( m_hDll, DestroyModule_FN);
			if(m_fnCreator && m_fnDestroyer)
			{
				m_pInterface = (T*)m_fnCreator(pInitParam,pSystem);
				return m_pInterface != NULL;
			}
		}
		return FALSE;
	}
	T * GetInterface(){return m_pInterface;}
};


#endif