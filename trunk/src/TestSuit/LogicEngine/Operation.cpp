#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <fstream>

#include "LogicEngine.h"
#include "Operation.h"
// #define _CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>
// #ifdef _DEBUG
// #define   new   new(_NORMAL_BLOCK,   __FILE__,   __LINE__)
// #endif
cOperationManager::cOperationManager()
	:m_pOperations(NULL)
	,m_nOperations(0)
{
}
cOperationManager::~cOperationManager()
{
	delete []m_pOperations;
	m_pOperations = NULL;
	m_nOperations = 0;
}

BOOL cOperationManager::Load(const char * lpctstrFile)
{
	return FALSE;
}
BOOL cOperationManager::Create(const char * lpcstrFile)
{
	strcpy(m_szFile,lpcstrFile);
	return TRUE;
}
BOOL cOperationManager::Op(UINT nID,cUnitObject * u1,cUnitObject ** u2,UINT n2,cUnitObject ** u3,UINT n3)
{
	return FALSE;
}
Operation * cOperationManager::GetOperation(UINT nID)
{
	return NULL;
}
Operation * cOperationManager::GetOperation(const char * lpcstrOperation)
{
	return NULL;
}
