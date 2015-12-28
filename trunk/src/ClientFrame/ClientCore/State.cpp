#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件：
#include <windows.h>

#include "support.h"
#include "SupportLoader.h"
#include "NetInterface.h"
#include "readerwriter.h"

#include "state.h"

CClientModule::CClientModule()
{
	m_pEngineSDK = CClientApp::getSingletonPtr()->GetEngineSDK();
	m_pClientNet = CClientApp::getSingletonPtr()->GetClientNet();
	m_pReader = CClientApp::getSingletonPtr()->GetReader();
	m_pWriter = CClientApp::getSingletonPtr()->GetWriter();
	//m_nSession = CClientApp::getSingletonPtr()->GetSession();
	m_pWindow = CClientApp::getSingletonPtr()->GetWindow();
	m_pWorld = CClientApp::getSingletonPtr()->GetWorld(); 
	m_pUiManager = CClientApp::getSingletonPtr()->GetUiManager();
}

void CClientModule::OnServerMessageDefault(int cmd,cReader* pReader,size_t len)
{
	_error("No ClientModule use message %d\n",cmd);
}

size_t CClientModule::Send()
{
	int nSession = CClientApp::getSingletonPtr()->GetSession();
	return m_pClientNet->Send(m_pWriter->Buffer(),m_pWriter->Length(),nSession); 
}
bool CClientModule::OnWndMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_UICOMMAND/*WM_COMMAND*/:
		return OnGuiMessage(message, wParam,lParam);
		break;
	}
	return false;
}

CClientState::CClientState()
{
	m_nState = -1;
	m_nStateTo = -1;

	//加载消息框
	m_pUiManager->dialog_load(DLG_MESSAGE_BOX2_FILENAME,ui::kDialogModel);
	m_pUiManager->dialog_set_visible(DLG_MESSAGE_BOX2,false);
	m_pUiManager->dialog_load(DLG_MESSAGE_BOX_FILENAME,ui::kDialogModel);
	m_pUiManager->dialog_set_visible(DLG_MESSAGE_BOX,false);
}

CClientState::~CClientState()
{
	m_pUiManager->dialog_remove_all();

	if (CClientApp::getSingletonPtr())
		CClientApp::getSingletonPtr()->SetPlayUISoundEnable(false);
}
bool CClientState::OnInit()
{
	if (CClientApp::getSingletonPtr())
		CClientApp::getSingletonPtr()->SetPlayUISoundEnable(true);
	return true;
}
void CClientState::RegisterModule(CClientModule * pModule,int nMinSrvCmd,int nMaxSrvCmd,int nMinGuiDlg,int nMaxGuiDlg)
{
	Module m = {pModule,{nMinSrvCmd,nMaxSrvCmd},{nMinGuiDlg,nMaxGuiDlg}};
	m_vecModules.push_back(m);
}
void CClientState::OnServerMessageDefault(int cmd,cReader* pReader,size_t len)
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
		CClientModule::OnServerMessage(cmd,pReader,len);
}
bool CClientState::OnGuiMessageDefault(UINT nMsg,WPARAM wParam, LPARAM lParam)
{
	UINT nModules = m_vecModules.size();
	bool bHandle = false;
	UINT DlgID = (UINT)((wParam>>14)&0x1ff);
	UINT CtrlID=(UINT)(wParam&0x3fff);
	//UINT CtrlMsg=(UINT)((wParam>>23)&0x1ff);
	UINT CtrlMsg = (UINT)((wParam>>23)&0xff);
	UINT nFlag = (UINT)((wParam>>31)&0x1);

	for (UINT n=0;n<nModules;n++)
	{
		Module &m = m_vecModules[n];
		if((int)DlgID>=m.nGuiDlgRange[0] && (int)DlgID<=m.nGuiDlgRange[1])
		{
			m.pModule->OnGuiMessage(nMsg,wParam,lParam);
			bHandle = TRUE;
			break;
		}
	}
	if(!bHandle)
		CClientModule::OnGuiMessageDefault(nMsg,wParam,lParam);
	return bHandle;
}
bool CClientState::OnWndMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(m_nStateTo != -1)
	{
		return true;
	}
	
	if (m_pUiManager->OnMessage(message, wParam, lParam))
	{
		return true;
	}

	switch(message)
	{
	case WM_UICOMMAND/*WM_COMMAND*/:
		return OnGuiMessage(message, wParam,lParam);
		break;
	}

	UINT nModules = m_vecModules.size();
	for (UINT n=0;n<nModules;n++)
	{
		Module &m = m_vecModules[n];
		if(m.pModule->OnWndMessage(message,wParam,lParam))
			return true;
	}
	
	return false;
}
bool CClientState::OnMessageBox(UINT nCode,UINT nClickBt)
{
	UINT nModules = m_vecModules.size();
	for (UINT n=0;n<nModules;n++)
	{
		Module &m = m_vecModules[n];
		if(m.pModule->OnMessageBox(nCode,nClickBt))
			return true;
	}
	return false;
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

void CClientModule::FormDialog(int nDialogLeft,int nDialogRight,bool bCenter)
{
	//RECT rcLeft = {0};
	//RECT rcRight = {0};
	//int xLeft,xRight;
	//int yLeft,yRight;
	////获得主窗口的大小
	//int nHeight = CClientApp::getSingletonPtr()->GetConfig()->nHeight;
	//int nWidth = CClientApp::getSingletonPtr()->GetConfig()->nWidth;
	//m_pUiManager->dialog_get_rect(nDialogLeft,&rcLeft);
	//m_pUiManager->dialog_get_rect(nDialogRight,&rcRight);
	//if (bCenter)
	//{
	//	xLeft = (nWidth-(rcLeft.right-rcLeft.left)-(rcRight.right-rcRight.left))/2;
	//	xRight = xLeft+(rcLeft.right-rcLeft.left);
	//}
	//else
	//{
	//	xLeft = 0;	
	//	xRight = (nWidth-(rcRight.right-rcLeft.left))/2;
	//}
	//yLeft = (nHeight-(rcLeft.bottom-rcLeft.top))/2;
	//yRight = (nHeight-(rcRight.bottom-rcRight.top))/2;
	//m_pUiManager->dialog_set_location(nDialogLeft,xLeft,yLeft);
	//m_pUiManager->dialog_set_location(nDialogRight,xRight,yRight);
	if (!m_pUiManager->dialog_is_visible(nDialogLeft))
	{
		m_pUiManager->dialog_set_visible(nDialogLeft,true);
	}
	if (!m_pUiManager->dialog_is_visible(nDialogRight))
	{
		m_pUiManager->dialog_set_visible(nDialogRight,true);
	}	
}
