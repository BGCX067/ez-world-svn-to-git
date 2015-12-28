#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件：
#include <windows.h>

#include "support.h"
#include "NetInterface.h"
#include "readerwriter.h"
#include "ModuleSupport.h"

#include "ClientNet.h"
#include "state.h"

CClientModule::CClientModule(cClientNet * pClientNet,cReader * pReader,cWriter * pWriter,int nSession)
	:m_pClientNet(pClientNet)
	,m_pReader(pReader)
	,m_pWriter(pWriter)
	,m_nSession(nSession)
{
}

Result CClientModule::OnServerMessageDefault(int cmd,cReader* pReader,size_t len)
{
	LOGF_ERROR(DLL_LOGGER,_T("No ClientModule use message %d"),cmd);
	return Result(Ra_NOTHING,Rc_Successed);
}

size_t CClientModule::Send()
{
	return m_pClientNet->Send(m_pWriter->Buffer(),m_pWriter->Length(),m_nSession); 
}

CClientState::CClientState(cClientNet * pClientNet,cReader * pReader,cWriter * pWriter,int nSession)
	:CClientModule(pClientNet,pReader,pWriter,nSession)
{
	m_nState = -1;
	m_nStateTo = -1;
}

CClientState::~CClientState()
{
}
bool CClientState::OnInit()
{
	return true;
}
void CClientState::RegisterModule(CClientModule * pModule,int nMinSrvCmd,int nMaxSrvCmd,int nMinGuiDlg,int nMaxGuiDlg)
{
	Module m = {pModule,{nMinSrvCmd,nMaxSrvCmd},{nMinGuiDlg,nMaxGuiDlg}};
	m_vecModules.push_back(m);
}
Result CClientState::OnServerMessageDefault(int cmd,cReader* pReader,size_t len)
{
	UINT nModules = m_vecModules.size();
	BOOL bHandle = FALSE;
	for (UINT n=0;n<nModules;n++)
	{
		Module &m = m_vecModules[n];
		if(cmd>=m.nSrvCmdRange[0] && cmd<=m.nSrvCmdRange[1])
		{
			m.pModule->OnServerMessage(cmd,pReader,len);
			bHandle = TRUE;
			break;
		}
	}
	if(!bHandle)
		return CClientModule::OnServerMessage(cmd,pReader,len);
	return Result(Ra_NOTHING,Rc_Successed);
}

int CClientState::OnLoop(DWORD dwTime)
{
	UINT nModules = m_vecModules.size();
	for (UINT n=0;n<nModules;n++)
	{
		Module &m = m_vecModules[n];
		m.pModule->OnLoop(dwTime);
	}
	return m_nStateTo;
}
