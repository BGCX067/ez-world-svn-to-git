#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件：
#include <windows.h>
#include <iostream>
#include <tlhelp32.h>  
#include <tchar.h>  

#include "support.h"
#include "SupportLoader.h"
#include "NetInterface.h"
#include "readerwriter.h"

#include "clientcore.h"

#define _error
#define _printf

CClientCore * Singleton<CClientCore>::ms_Singleton = NULL;

void CClientCore::Clear()
{
	m_pClientState = NULL;
	m_nStates = 0;
	m_ppCreateFns = NULL;
	m_pSocketConfig = NULL;
	m_pClientNet = NULL;
	m_pReader = NULL;
	m_pWriter = NULL;
	m_connectstate = enotconnect;
	m_bOtherLogin = FALSE;

	m_strVersionInfo.clear();

	m_bshowdisconnect=false;

	m_dwRecvCheckSpeedTm = 0;
	m_bRecvedCheckSpeed = false;
	m_nSession=INVALID_SESSION;
	m_alterDisconnectBox = true;
}

CClientCore::CClientCore()
{
	Clear();
}

CClientCore::~CClientCore()
{
	delete []m_ppCreateFns;
	Clear();
}

void CClientCore::InitState(CreateFn ** ppCreateFn, int nStates)
{
	m_ppCreateFns = new CreateFn * [nStates];
	CopyMemory(m_ppCreateFns,ppCreateFn,sizeof(CreateFn *)*nStates);
	m_nStates = nStates;
}

//初始化客户端
BOOL CClientCore::Initialize(iClientNet * pClientNet)
{
	m_pClientNet = pClientNet;
	m_pReader = new cReader;
	m_pWriter = new cWriter;

	// 7 回调
	if(!OnInitialize())
	{
		_error(_T("Init OnInitialize function failed\n"));
		return FALSE;
	}

	// 8.开始第一个状态
	int nStartState = 0;

	if(m_ppCreateFns[nStartState])
	{
		m_pClientState = m_ppCreateFns[nStartState]();
		_printf("Current state is %d.\n", nStartState);
		if(!m_pClientState ->OnInit())
		{
			_printf("Init state %d failed\n",nStartState);
			return FALSE;
		}
		_printf("Init state %d success\n",nStartState);
	}

	return true;
}

//关闭客户端
void CClientCore::Shutdown()
{
	// 释放最后一个状态
	if(m_pClientState)
	{
		delete m_pClientState;
		m_pClientState = NULL;
	}

	m_pClientNet = NULL;

	// 销毁 Socket
	delete m_pReader;
	delete m_pWriter;
	delete m_pSocketConfig;

	m_pReader = NULL;
	m_pWriter = NULL;
	m_pSocketConfig = NULL;

	OnShutdown();
}

BOOL CClientCore::ChangeState(UINT nState)
{
	if(m_pClientState)
	{
		delete m_pClientState;
		_printf("Unload current state\n");
	}
	m_pClientState = NULL;

	//m_pUiManager->hide_all_dialog();

	if(nState >=0 && nState<m_nStates && m_ppCreateFns[nState])
	{
		_printf("Load state %d。\n",nState);
		m_pClientState = m_ppCreateFns[nState]();
		if(m_pClientState ->OnInit())
			return TRUE;
	}

	_printf("Invalid state %d\n",nState);

	return FALSE;
}

void CClientCore::Run()
{
	if(m_pClientState)
	{
		DWORD dwBegin = GetTickCount();

		static DWORD dwFirstTime = 0;
		BOOL bEnterGameWorld = m_pClientState->GetState() == 4; //Game world state
		BOOL bStartRecord = FALSE;
		if (bEnterGameWorld && dwFirstTime == 0)
			dwFirstTime = dwBegin;

		//只有进入游戏世界10秒后才进行统计
		if (dwFirstTime != 0 && (dwBegin - dwFirstTime) > 10 * 1000)
			bStartRecord = TRUE;	

		GetSysEfficiency()->update();
		if (bStartRecord)
			GetSysEfficiency()->beginRecord("ClientLoop");

		// 收取网络消息
		if (bStartRecord)
		{
			GetSysEfficiency()->beginRecord("NetLoop");
			NetLoop();
			GetSysEfficiency()->endRecord("NetLoop");
		}
		else
			NetLoop();
		
		// 逻辑循环
		if (bStartRecord)
		{
			GetSysEfficiency()->beginRecord("LogicLoop");
			DWORD dwTime = m_pEngineSDK->getTime();
			int nRet = m_pClientState->OnLoop(dwTime);
			if(nRet >=0)
			{
				if(!ChangeState(nRet))
				{
					_printf("Change %d state failed\n",nRet);
					MessageBox(CLIENT_MESSAGE_BOX_ERROR,CLIENT_MESSAGE_QUIT,MESSAGE_STATUS_FAIL);
				}
			}
			GetSysEfficiency()->endRecord("LogicLoop");
		}
		else
		{
			DWORD dwTime = m_pEngineSDK->getTime();
			int nRet = m_pClientState->OnLoop(dwTime);
			if(nRet >=0)
			{
				if(!ChangeState(nRet))
				{
					_printf("Change %d state failed\n",nRet);
					MessageBox(CLIENT_MESSAGE_BOX_ERROR,CLIENT_MESSAGE_QUIT,MESSAGE_STATUS_FAIL);
				}
			}
		}

		// 引擎渲染
		if (bStartRecord)
		{
			GetSysEfficiency()->beginRecord("EngineLoop");
			EngineLoop();

			// add for control Cursor weed Windows Cursor
			if (m_CurCursor != m_NewCursor)
			{
				m_CurCursor = m_NewCursor;
				m_Window.SetCurCursor(m_CurCursor);
			}
			m_pUiManager->clear_text_downmost();
			GetSysEfficiency()->endRecord("EngineLoop");
		}
		else
		{
			EngineLoop();

			// add for control Cursor weed Windows Cursor
			if (m_CurCursor != m_NewCursor)
			{
				m_CurCursor = m_NewCursor;
				m_Window.SetCurCursor(m_CurCursor);
			}
			m_pUiManager->clear_text_downmost();
		}

		if (bStartRecord)
			GetSysEfficiency()->endRecord("ClientLoop");
		// end

		// 如果单桢时间过长,看看发生了什么问题
		DWORD dwCurrTime = ::GetTickCount();
		if((dwCurrTime - dwBegin) >= 100 && bStartRecord)
		{
			GetSysEfficiency()->Dump(TRUE);
			_printf("In the loop use much time：%d\n", dwCurrTime - dwBegin);
		}

		// 加速器检测
		DWORD dwCurTm = GetTickCount();
		if ( m_connectstate==econnected 
			&& m_bRecvedCheckSpeed 
			&& (dwCurTm-m_dwRecvCheckSpeedTm > CHECK_SPEED_INTERVAL_TM)
			)
		{
			SendCheckSpeedMsg();
		}
	}
}


void CClientCore::EngineLoop()
{
	//if(m_Window.IfActive())
	m_pEngineSDK->NextFrame(0xff000000); 
}

void CClientCore::IgnoreGateMsg(int nHeader)
{
	assert(nHeader == 27 || nHeader == 42);
	if(nHeader == 27)
	{
		int nGate = m_pReader->INT();
	}
	else
	{
		UINT nCount = m_pReader->UINT();
		if(nCount)
		{
			UINT * pnIds = (UINT *)m_pReader->Struct(sizeof(UINT)*nCount);
		}
	}
}

void CClientCore::NetLoop()
{
	UINT nSession = 0;
	LPVOID pparam;
	if(m_connectstate==econnecting)
	{
		if(m_nSession==m_pClientNet->GetFailedSession(pparam))
		{
			m_connectstate=enotconnect;
			m_pUiManager->control_enable(DLG_LOGIN_DIALOG,DLG_LOGIN_BUTTONOK,true);
			//m_pUiManager->control_dsbutton_set_accept_user_input()
			MessageBox(CLIENT_MESSAGE_BOX_ERROR,CLIENT_MESSAGE_NOTHING,MESSAGE_CONNECT_FAIL);
		}
		unsigned nSession=m_pClientNet->GetNewSession(pparam);
		if(nSession==m_nSession)
		{
			int nVerMajor = 0;
			int nVersion = 0;
			int nVerPatch = 0;
			m_connectstate=econnected;
			GetVersion(nVerMajor,nVersion,nVerPatch);
			_printf("Login %d:%d:%d \n", nVerMajor, nVersion, nVerPatch);
			//			//向服务器发消息，校验版本号 
			//#ifdef NO_BALANCE
			//			SendMd5Code();
			//
			VersionInfo info;
			info.nVerMajor = nVerMajor;
			info.nVersion  = nVersion;
			info.nVerPatch = nVerPatch;

			(*m_pWriter)<<CBEGIN()<<(int)c2s_Version_Verify<<info;
			m_pClientNet->Send(m_pWriter->Buffer(),m_pWriter->Length(),m_nSession);
			//#endif
		}
	}else if(m_connectstate==edisconnecting)
	{
		if(m_nSession==m_pClientNet->GetFailedSession(pparam)||m_nSession==m_pClientNet->GetNewSession(pparam))
		{
			m_connectstate=enotconnect;
		}
	}else	if(m_connectstate==econnected)
	{
		/*if (m_pEngineSDK->GetStage()->IsLoading())
			return;*/

		LPVOID param;
		unsigned nsession=m_pClientNet->GetDieSession(param);

		if(nsession!=m_nSession)
		{
			m_pClientState->OnNetLoopBegin();
			DWORD dwUsed = 0;
			DWORD dwStart = ::GetTickCount(); //m_pEngineSDK->getTime()
			int nRoleInfo = 0;
			//_printf("net loop start %d\n",dwStart);

			// 如果30毫秒内没有处理完消息，跳出循环，下次循环再处理
			while (true)	
			{
				size_t len=0;
				if(m_nSession!=-1)
					len = m_pClientNet->Recive(*m_pReader,m_nSession);
				if(len == 0)
					break;

				// 处理消息
				int nHeader = m_pReader->Begin();

				Msg_s2c_Command eCmd = (Msg_s2c_Command)nHeader;
				if (s2c_Role_Info == eCmd)
					++nRoleInfo;

				// 帐号从另外的地方登陆了。
				if(eCmd == s2c_Gate_LogonOtherwise)
				{
					MessageBox(CLIENT_MESSAGE_BOX_WARNING,CLIENT_MESSAGE_QUIT/*CLIENT_MESSAGE_RESTART*/,MESSAGE_LOGIN_OTHERWISE);
					m_pClientState->OnNetLoopEnd();
					continue;
				}else if(eCmd==s2c_Login_GatePing)
				{
					m_pClientState->Begin(c2s_Login_GatePing);
					m_pClientState->Send();
					continue;
				}else if (eCmd == s2c_Account_Freezed)
				{
					MessageBox(CLIENT_MESSAGE_BOX_ERROR,CLIENT_MESSAGE_NOTHING,MESSAGE_ACCOUNT_FREEZED);
					m_pClientState->OnNetLoopEnd();
					m_alterDisconnectBox = false;
					continue;
				}
				// todo:如果状态因为等待某个服务器消息而被禁止,得到消息解除禁止.
				// 其他消息
				Msg_s2c_Result Result = (Msg_s2c_Result)m_pReader->INT();
				if(Result != Result_Successed)
				{
					CLIENT_MESSAGE_BOX eBox = CLIENT_MESSAGE_BOX_ERROR;
					CLIENT_MESSAGE_ACT eAct = CLIENT_MESSAGE_NOTHING;

					if(eCmd == s2c_Version_Verify)
					{// 版本校验出错要退出
						eAct = CLIENT_MESSAGE_QUIT;
					}else	if(eCmd == s2c_Login_Logon)
					{// 登陆出错要回到登陆状态
						eAct = CLIENT_MESSAGE_NOTHING;
						m_pClientNet->Disconnect(m_nSession);
						m_nSession=INVALID_SESSION;
						m_connectstate = edisconnecting;
						//Result = Result_Login_Register_UserNameExist;
					}else if(eCmd==s2c_Mail_SendMail)
					{
						m_pUiManager->control_enable(DLG_ROLE_MAIL, DLG_MAIL_SEND, true);
					}
					else if (s2c_Map_NewbieSelect==eCmd)
					{
						m_pUiManager->control_enable(DLG_NEWBIE_LINE,DLG_NEWBIE_LINE_OK,TRUE);
					}
					else if (eCmd == s2c_Mail_ReceiveAdjunct)
					{
						//.
						for (UINT nItemIndex = 0; nItemIndex < 6; ++nItemIndex)
						{
							//m_pUiManager->control_cellbox_cell_enable(DLG_ROLE_MAIL,DLG_MAIL_RCVITEM+nItemIndex, 0, 0, TRUE);
							m_pUiManager->control_cellbox_set_status(DLG_ROLE_MAIL,DLG_MAIL_RCVITEM+nItemIndex, 0, 0, 0);
						}
					}else if(eCmd==s2c_Role_Create)
					{
						m_pUiManager->control_set_enable(DLG_CREATE_ROLE_5_DLG,CTRL_CREATE_ROLE_CREATE,true);
					}else if(eCmd==s2c_Role_Delete)
					{
						//m_pUiManager->control_set_enable(DLG_PASSWORD_DIALOG,DLG_PASSWORD_OK,true);
						m_pUiManager->control_set_enable(DLG_ENTER_GAME,DLG_ENTER_GAME_OK,true);
					}

					if(OnResultError(eCmd))
					{
						m_pClientState->OnServerMessage(eCmd,m_pReader,len); 
					}

					TCHAR szMessage[256];
					const TCHAR *perrordes;
					perrordes=m_pErrDesc->GetErrDes(GetStringMsg_s2c_Result(Result));
					if (perrordes)
					{
						sprintf(szMessage,"%s",perrordes);
					}

					MessageBox(eBox,eAct,szMessage);
				}
				else
				{
					if (s2c_CheckSpeed == eCmd)
					{
						RecvCheckSpeedMsg();
					}
					else
					{
						m_pClientState->OnServerMessage(eCmd,m_pReader,len); 
					}
				}

				dwUsed = ::GetTickCount() - dwStart; //m_pEngineSDK->getTime() - dwStart;
			}

			m_pClientNet->SendAll(); 
			m_pClientState->OnNetLoopEnd();
		}
		else 
		{//连接断开
			m_connectstate=enotconnect;
			if(!m_bshowdisconnect)
			{
				ChangeState(0);
				if (m_alterDisconnectBox)
					MessageBox(CLIENT_MESSAGE_BOX_ERROR,CLIENT_MESSAGE_RESTART,MESSAGE_DISCONNECT); 
			}
			//if(m_bConnected==enotconnected)
			//{
				// todo:显示断开连接的对话框

			//	ChangeState(0);
			//	MessageBox(CLIENT_MESSAGE_BOX_ERROR,CLIENT_MESSAGE_RESTART,MESSAGE_DISCONNECT); 
			//}
		}
	}
}

BOOL CClientCore::OnResultError(int eCmd)
{
	switch(eCmd)
	{
	case s2c_GemInset_Perforate:
		m_pReader->Rebegin(m_pReader->GetOffset()-4);
		return true;
	case s2c_GemInset_Upgrade:
		m_pReader->Rebegin(m_pReader->GetOffset()-4);
		return true;
	case s2c_GemInset_Inlay:
	case s2c_Login_Logon:
	case s2c_Role_List:
	case s2c_MakeEquip:
	case s2c_Equip_SoulLV:
	case s2c_Equip_SoulQL:
	case s2c_Consortia_Create:
	case s2c_Role_Ready:
	case s2c_Equip_Binding:
	case s2c_UpdateGem:
		return true;
	default:
		return FALSE;
	}
}


const cLeafRow * CClientCore::GetMap(int nMapID)
{
	cLeafNode * pLeafNode = (cLeafNode*)m_pWorldRoot->GetNodeByPath(_T("Root/UnitTree/map/UnitTemplate"));
	if(!pLeafNode)
		return NULL;
	UINT row = pLeafNode->FindRow(_T("ID"),nMapID);
	if(-1 ==row )
		return NULL;
	return pLeafNode->GetRow(row); 
}

void CClientCore::MessageBox(CLIENT_MESSAGE_BOX eBox,CLIENT_MESSAGE_ACT eAct,const TCHAR * lpszMessage)
{
	// 显示提示框
	if(eBox == CLIENT_MESSAGE_BOX_WARNING)
	{
		m_pUiManager->dialog_set_visible(DLG_MESSAGE_BOX2,true);
		m_pUiManager->control_set_text(DLG_MESSAGE_BOX2,DLG_MESSAFE_BOX2TEXT,lpszMessage);
		m_pUiManager->dialog_center_window(DLG_MESSAGE_BOX2);
	}
	else if(eBox == CLIENT_MESSAGE_BOX_ERROR)
	{
		m_pUiManager->dialog_set_visible(DLG_MESSAGE_BOX,true);
		m_pUiManager->control_set_text(DLG_MESSAGE_BOX,DLG_MESSAGE_TEXT,lpszMessage);
		m_pUiManager->dialog_center_window(DLG_MESSAGE_BOX);
	}
	m_eMessageBox = eBox;
	m_eMessageAct = eAct;
}
void CClientCore::MessageBox(CLIENT_MESSAGE_BOX eBox,CLIENT_MESSAGE_ACT eAct,int MsgRes)
{
	// 确认提示 从gameresult.xml装入
	if(eBox == CLIENT_MESSAGE_BOX_WARNING)
	{
		m_pUiManager->dialog_set_visible(DLG_MESSAGE_BOX2,true);
		m_pUiManager->control_set_text(DLG_MESSAGE_BOX2,DLG_MESSAFE_BOX2TEXT,CErrDes::getSingletonPtr()->GetErrDes(GetStringMsg_s2c_Result(MsgRes)));
		m_pUiManager->dialog_center_window(DLG_MESSAGE_BOX2);
	}
	// 错误信息
	else if(eBox == CLIENT_MESSAGE_BOX_ERROR)
	{
		m_pUiManager->dialog_set_visible(DLG_MESSAGE_BOX,true);
		m_pUiManager->control_set_text(DLG_MESSAGE_BOX,DLG_MESSAGE_TEXT,CErrDes::getSingletonPtr()->GetErrDes(GetStringMsg_s2c_Result(MsgRes)));
		m_pUiManager->dialog_center_window(DLG_MESSAGE_BOX);
	}
	m_eMessageBox = eBox;
	m_eMessageAct = eAct;
}
void CClientCore::CloseMessageBox()
{
	// 关闭提示框
	//if(m_pUiManager->dialog_is_loaded(DLG_MESSAGE_BOX2)!=NULL)
	//	m_pUiManager->dialog_remove(DLG_MESSAGE_BOX2);
	//if(m_pUiManager->dialog_is_loaded(DLG_MESSAGE_BOX)!=NULL)
	//	m_pUiManager->dialog_remove(DLG_MESSAGE_BOX);
	m_eMessageBox = CLIENT_MESSAGE_BOX_NONE;
	m_eMessageAct = CLIENT_MESSAGE_NOTHING;
}
//////////////////////////////////////////////////////////////////////////
//其实这个函数的返回值没有什么用，因为在CClientCore::OnMessage()中，没有利用这个函数的返回值
//////////////////////////////////////////////////////////////////////////
bool CClientCore::OnMessageBox(UINT nMsg,WPARAM wParam, LPARAM lParam)
{
	UINT DlgID = (UINT)((wParam>>14)&0x1ff);			
	UINT CtrlID=(UINT)(wParam&0x3fff);					
	UINT CtrlMsg=(UINT)((wParam>>23)&0x1ff);
	if(DlgID == DLG_MESSAGE_BOX2)
	{
		// 关闭提示框
		//m_pUiManager->dialog_set_visible(DLG_MESSAGE_BOX2,false);
		if((CtrlID == DLG_MESSAGE_BOX2OK) && (CtrlMsg == ui::EN_CLICKED))
		{
			switch(m_eMessageAct)
			{
			case CLIENT_MESSAGE_NOTHING:			// 确认后无事发生
				break;
			case CLIENT_MESSAGE_QUIT:				// 确认后退出游戏
				{
					if(m_nSession)
						m_pClientNet->Disconnect(m_nSession);
					m_connectstate = edisconnecting;
					PostQuitMessage(0);
				}
				break;
			case CLIENT_MESSAGE_RESTART:		// 确认后返回登陆界面	
				{
					//m_bConnected = FALSE;
					ChangeState(0);
					m_bshowdisconnect=false;
				}
				break;
			case CLIENT_MESSAGE_DISCONNECT:		// 确认后断开连接
				{
					LPVOID param;
					unsigned nsession=m_pClientNet->GetDieSession(param);
					if(nsession!=m_nSession)
					{
						m_pClientNet->Disconnect(m_nSession); 
						m_connectstate=edisconnecting;
					}
				}
				break;
			default:
				{
					// 确认后返回给当前状态，消息框询问的原因。	
					if(m_pClientState && (m_eMessageAct>CLIENT_MESSAGE_USERCODE))
						m_pClientState->OnMessageBox(m_eMessageAct - CLIENT_MESSAGE_USERCODE,DLG_MESSAGE_BOX2OK);  
				}
				break;
			}
			m_pUiManager->dialog_set_visible(DLG_MESSAGE_BOX2,false);
			return true;
		}
		else if ((CtrlID == DLG_MESSAGE_BOX2CANCEL) && (CtrlMsg == ui::EN_CLICKED))
		{
			//用于询问玩家是否接受交易，如果按cancel则拒绝交易
			bool res = false;
			if(m_pClientState && (m_eMessageAct>CLIENT_MESSAGE_USERCODE))
				res = m_pClientState->OnMessageBox(m_eMessageAct - CLIENT_MESSAGE_USERCODE,DLG_MESSAGE_BOX2CANCEL);
			// 关闭提示框
			m_pUiManager->dialog_set_visible(DLG_MESSAGE_BOX2,false);
			return res;
		}

	}
	else if(DlgID == DLG_MESSAGE_BOX)
	{
		if((CtrlID == DLG_MESSAGE_BUTTONOK) && (CtrlMsg == ui::EN_CLICKED))
		{
			m_pUiManager->dialog_set_visible(DLG_MESSAGE_BOX,false);
			//m_bConnected = FALSE;
			//ChangeState(0);
			return true;
		}
	}
	return false;
}

void CClientCore::Minimize()
{
	if(m_Window.IfFullScreen())
		ShowWindow(m_Window,SW_MINIMIZE);  
}

bool CClientCore::InitResultDescMap() 
{
	m_pErrDesc = new CErrDes;

	HVFILE hFile = GetSysVfs()->Open(RESULTDESCDEF);
	if(!hFile)
		return FALSE;
	
	TCHAR * pFileBuf = NULL;
	int iFileLen = GetSysVfs()->Size(hFile);
	if(!iFileLen)
	{
		assert(0);
		return FALSE;
	}

	pFileBuf = new TCHAR[iFileLen+1];
	pFileBuf[iFileLen] = 0;
	if(!GetSysVfs()->Read(hFile,pFileBuf))
	{
		delete[]pFileBuf;
		GetSysVfs()->Close(hFile);
		return FALSE;
	}
	GetSysVfs()->Close(hFile);
	
	bool bRet = m_pErrDesc->Init(pFileBuf);
	delete []pFileBuf;
	return bRet;
}
bool CClientCore::InitGameString()
{
	HVFILE hFile = GetSysVfs()->Open("GameString.xml");
	if(!hFile)
		return false;
	
	TCHAR * pFileBuf = NULL;
	int iFileLen = GetSysVfs()->Size(hFile);
	if(!iFileLen)
	{
		assert(0);
		return false;
	}

	pFileBuf = new TCHAR[iFileLen+1];
	pFileBuf[iFileLen] = 0;
	if(!GetSysVfs()->Read(hFile,pFileBuf))
	{
		delete[]pFileBuf;
		GetSysVfs()->Close(hFile);
		return false;
	}
	GetSysVfs()->Close(hFile);
	m_pMsgString = new CMsgDes();
	m_pMsgString->Init(pFileBuf);
	delete []pFileBuf;
	return true;
}
const TCHAR * CClientCore::GetErrDes(unsigned int Result)
{
	return m_pErrDesc->GetErrDes(GetStringMsg_s2c_Result(Result));
}


BOOL CClientCore::InitUiManager()
{
	_printf("Create UiManager\n");
	
	_printf("\tLoad UiPlugin2007.dll\n");
	m_hUi2004Dll = LoadLibrary(UIMANAGER_DLL);

	if(!m_hUi2004Dll){
		_printf("\tLoad %s failed\n",UIMANAGER_DLL);
		__debugbreak();
	}

	_printf("\tGet uimanager interface\n");
	FARPROC fpnCreate = GetProcAddress(m_hUi2004Dll,"CreateUiManager");
	if(!fpnCreate )
	{
		_printf("\tUimanager interface empty\n");
		__debugbreak();
	}

	_printf("\tCreate uiRander\n");
	m_pUiSupport = new ui::CUiSupport(m_pEngineSDK,m_pConfig->szUiTexturePath);

	_printf("\tCreate uimanager\n");
	m_pUiManager = (ui::CUiManager2004 *)fpnCreate();

	_printf("\tCreate uisupport\n");
	if(!m_pUiManager->create(m_Window,m_pUiSupport)){
		_printf("\tInit uisupport failed\n");
		__debugbreak();
	}

	_printf("\tInit uisupport success\n");
	m_pUiRdCallback = new ui::CUiRender(m_pUiManager);
	m_pEngineSDK->AddRenderCallback(m_pUiRdCallback);

	_printf("\tInit uimanager success\n");
	return TRUE;
}
void CClientCore::DestroyUiManager()
{
	if(m_pEngineSDK)
	{
		m_pEngineSDK->DelRenderCallback(m_pUiRdCallback);
		delete m_pUiRdCallback;
		m_pUiRdCallback = NULL;
	}
	if (m_hUi2004Dll)
	{
		FARPROC fpnDestroy = GetProcAddress(m_hUi2004Dll,"DestroyUiManager");
		fpnDestroy();
		FreeLibrary(m_hUi2004Dll);
	}
	
	delete m_pUiSupport;
	m_pUiSupport = NULL;
}

void CClientCore::SetPlayUISoundEnable(bool bEnable)
{
	if (m_pUiSupport)
	{
		m_pUiSupport->SetPlaySoundEnable(bEnable);
	}
}

bool CClientCore::GetPlayUISoundEnable() const
{
	if (m_pUiSupport)
	{
		return m_pUiSupport->GetPlaySoundEnable();
	}
	return false;
}

VOID CClientCore::SendMd5Code()
{
	_printf("★Stert read MD5\n");
	char szDeveloper[2] = {0};
	GetPrivateProfileString(_T("render"),_T("developer"),_T(""),szDeveloper, 2, "./client.ini");
	bool bDeveloper = (bool)atoi(szDeveloper);
	if (!bDeveloper)		// 非开发者。
	{
		_printf("No-- \n");
		_error("<>Start check MD5 \n");
		CSFMClient SFMC;
		char *p = (char*)SFMC.GetBuffer();
		if (p)
		{
			char szMd5Buf[MAX_PATH]={0};
			int nPtrHwnd = 0;
			_stscanf(p, "%d■%s", &nPtrHwnd, szMd5Buf);
			_error("<>MD5 == %s \n", szMd5Buf);

			int nStrLen = _tcslen(szMd5Buf);
			m_pStrMd5Info = new char[nStrLen+1];
			::memset(m_pStrMd5Info, 0, sizeof(char)*(nStrLen+1));
			_tcscpy(m_pStrMd5Info, szMd5Buf);

			HWND Hwnd = (HWND )nPtrHwnd;
			::PostMessage(Hwnd, WM_CLOSE, 0, 0);
		}
		else
		{
			// 对于非开发者，没有获得LAUNCHER内存共享的MD5数据.直接退出吗？？
			_error("<>MD5 == %s \n", "NULL");
		}
		_printf("End check MD5 \n");
	}
	else	// 对于开发者，那么就直接给个指定的MD5或者就不发送。
	{
		//.
	}

	_printf("★End read D5\n");
}

const char * CClientCore::ReadClientVersion()
{
	// 读取本EXE的版本属性。
	int nA=-1, nB=-1, nC=-1, nD=-1;	
	std::string strRetValue = "";
	CHAR buf_module[MAX_PATH] = {0};
	GetModuleFileName(NULL, buf_module, MAX_PATH);
	int nVersionLen = ::GetFileVersionInfoSize(buf_module,NULL);
	if (nVersionLen <= 0)
	{
		return FALSE;
	}

	CHAR * pBuffer = new CHAR[nVersionLen];
	if (!pBuffer)
	{
		return FALSE;
	}

	if ( !::GetFileVersionInfo( buf_module,  NULL,   nVersionLen,   pBuffer   ) )
	{
		return FALSE;
	}

	struct LANGANDCODEPAGE {   
		WORD   wLanguage;   
		WORD   wCodePage;   
	} *lpTranslate;   

	//   Read   the   list   of   languages   and   code   pages.   
	UINT   cbTranslate;   
	::VerQueryValue( pBuffer, "\\VarFileInfo\\Translation", (LPVOID*)&lpTranslate, &cbTranslate);   

	//   dwBytes最大值应该是301.   
	//   应用仅仅允许输入最多300个字符：）   
	UINT   dwBytes;   
	CHAR*   lpBuffer;   
	CHAR   szSubBlock[256];
	char szVersion[20] = {0};

	_snprintf(szSubBlock,256,"\\StringFileInfo\\%04x%04x\\FileVersion",lpTranslate[0].wLanguage,lpTranslate[0].wCodePage   );
	if(::VerQueryValue(pBuffer,szSubBlock,(LPVOID*)&lpBuffer,&dwBytes))   
	{   
		_tcscpy(szVersion, lpBuffer);
		sscanf(szVersion, "%d.%d.%d.%d", &nA, &nB, &nC, &nD);
	}
	delete[]   pBuffer;

	_printf("VER nA,nB,nC,nD --> %d.%d.%d.%d\n", nA, nB, nC, nD);
	int nTempA, nTempB, nTempC, nTempD;
	if (nA != -1 && nB != -1
		&& nC != -1 && nD != -1)
	{
		m_nVerMajor = 0;
		m_nVersion = 0;
		m_nVerPatch = nD;

		nTempA = nA;nTempB = nB;
		nTempC = nC;nTempD = nD; 
	}
	else
	{
		//读取主要版本，次要版本和补丁号
		m_nVerMajor = GetPrivateProfileInt(_T("version"),_T("major"),0,VERSION_INI); 
		m_nVersion = GetPrivateProfileInt(_T("version"),_T("ver"),0,VERSION_INI); 
		m_nVerPatch = GetPrivateProfileInt(_T("version"),_T("Patch"),0,VERSION_INI); 

		m_nVerMajor = 0;
		m_nVersion = 0; 

		nTempA = 0;nTempB = 0;
		nTempC = 0;nTempD = m_nVerPatch;
	}
	_printf("VER %d.%d.%d.%d\n", m_nVerMajor, m_nVersion, m_nVerPatch, 0);
	
	char bufVersionTemp[MAX_PATH] = {0};
	_stprintf(bufVersionTemp,"%d.%d.%d.%d", nTempA, nTempB, nTempC, nTempD);
	strRetValue = bufVersionTemp;
	m_strVersionInfo = bufVersionTemp;

	return strRetValue.c_str();
}



void CClientCore::RecvCheckSpeedMsg()
{
	m_dwRecvCheckSpeedTm = GetTickCount();
	m_bRecvedCheckSpeed = true;

#ifdef _DEBUG
	_printf("RecvCheckSpeedMsg m_dwRevTm=%d\n", m_dwRecvCheckSpeedTm);
#endif
}

void CClientCore::SendCheckSpeedMsg()
{
	// calc delay time.
	DWORD dwTemp = m_dwRecvCheckSpeedTm;
	m_dwRecvCheckSpeedTm = GetTickCount();
	int b = m_dwRecvCheckSpeedTm-dwTemp-CHECK_SPEED_INTERVAL_TM;


	m_bRecvedCheckSpeed = false;
	
	(*m_pWriter)<<CBEGIN();
	m_pWriter->INT(c2s_CheckSpeed);
	//m_pWriter->INT(b);				// delay time.
	
	int nSession = GetSession();
	m_pClientNet->Send(m_pWriter->Buffer(),m_pWriter->Length(),nSession); 

#ifdef _DEBUG
	_printf("SendCheckSpeedMsg m_dwRevTm=%d, int bEsp=%d\n", m_dwRecvCheckSpeedTm, b);
#endif
}

unsigned CClientCore::AsyncConnect(LPCTSTR pszServer)
{
	assert(m_connectstate==enotconnect);
	m_nSession=m_pClientNet->AsyncConnect(pszServer);
	if(m_nSession!=INVALID_SESSION)
	{
		m_connectstate=econnecting;
	}
	return m_nSession;
}

void CClientCore::Disconnect()
{
	m_pClientNet->Disconnect(m_nSession);
	m_connectstate=edisconnecting;
}