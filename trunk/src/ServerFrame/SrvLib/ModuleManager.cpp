#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include "srvlib.h"
#include "modulemanager.h"

iSocketIO * IFuncModel::m_pSessiones = NULL;
cReader * IFuncModel::m_pReader = NULL;
cWriter * IFuncModel::m_pWriter = NULL;

void CModuleManager::AddModule(const Module &m)
{
	m_vecModules.push_back(m);
}

bool CModuleManager::OnClientRequest(int nCmd,LPVOID param)
{
	UINT nModules = m_vecModules.size();
	for (UINT n=0;n<nModules;n++)
	{
		Module &m = m_vecModules[n];
		if(nCmd>=m.nClientCmdRange[0] && nCmd<=m.nClientCmdRange[1])
		{
			m.pModule->OnClientReqest(nCmd,param);
			return true;
		}
	}
	return false;
}

bool CModuleManager::OnRcvDBResult(DbCommand eDbCommand,UINT nSession,LPVOID pPlayer)
{
	UINT nModules = m_vecModules.size();
	for(UINT n=0;n<nModules;n++)
	{
		Module &m = m_vecModules[n];
		if(eDbCommand>=m.nDBCmdRange[0] && eDbCommand<=m.nDBCmdRange[1])
		{
			m.pModule->OnRcvDBResponse(eDbCommand,nSession,pPlayer);
			return true;
		}
	}
	return false;
}
