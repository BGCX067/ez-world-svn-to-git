#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件：
#include <windows.h>
#include <iostream>
#include <tlhelp32.h>  
#include <tchar.h>  

#include "support.h"
#include "NetInterface.h"
#include "readerwriter.h"
#include "ModuleSupport.h"

#include "ClientNet.h"
#include "clientcore.h"
#include "state.h"

#define _error
#define _printf

void CClientCore::Clear()
{
	m_pClientState = NULL;
	m_nStates = 0;
	m_ppCreateFns = NULL;
	m_pClientNet = NULL;
	m_pReader = NULL;
	m_pWriter = NULL;
	m_connectstate = enotconnect;
	m_bOtherLogin = FALSE;

	m_nSession=INVALID_SESSION;
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
BOOL CClientCore::Initialize(cClientNet * pClientNet,cReader * pReader,cWriter * pWriter)
{
	m_pClientNet = pClientNet;
	m_pReader = pReader;
	m_pWriter = pWriter;

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
	m_pReader = NULL;
	m_pWriter = NULL;

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

Result CClientCore::Run()
{
	if(m_pClientState)
	{
		DWORD dwBegin = GetTickCount();

		DLL_EFFC->update();
		
		EFFC_BEGIN(_T("ClientLoop"));

		// 收取网络消息
		EFFC_BEGIN(_T("NetLoop"));
		Result r = NetLoop();
		EFFC_END(_T("NetLoop"));
		if(r.code != Rc_Successed)
			return r;
		
		// 逻辑循环
		EFFC_BEGIN(_T("LogicLoop"));
		int nRet = m_pClientState->OnLoop(GetTickCount());
		if(nRet >=0)
			nRet = ChangeState(nRet);
		EFFC_END(_T("LogicLoop"));
		if(!nRet)
			return Result(Ra_QUIT,Rc_Status_Fail);

		// 引擎渲染
		EFFC_BEGIN(_T("EngineLoop"));
		//EngineLoop();
		EFFC_END(_T("EngineLoop"));

		EFFC_END(_T("ClientLoop"));
		// end

		// 如果单桢时间过长,看看发生了什么问题
		DWORD dwCurrTime = ::GetTickCount();
		if((dwCurrTime - dwBegin) >= 100)
		{
			DLL_EFFC->DumpFrame();
			_printf("In the loop use much time：%d\n", dwCurrTime - dwBegin);
		}
	}
	return Result(Ra_NOTHING,Rc_Successed);
}

Result CClientCore::NetLoop()
{
	UINT nSession = 0;
	LPVOID pparam;
	if(m_connectstate==econnecting)
	{
		if(m_nSession==m_pClientNet->GetFailedSession(pparam))
		{
			m_connectstate=enotconnect;
			return Result(Ra_NOTHING,Rc_Connect_Fail);
		}
		unsigned nSession=m_pClientNet->GetNewSession(pparam);
		if(nSession==m_nSession)
		{
			m_connectstate=econnected;
			OnConnected();
		}
	}else if(m_connectstate==edisconnecting)
	{
		if(m_nSession==m_pClientNet->GetFailedSession(pparam)||m_nSession==m_pClientNet->GetNewSession(pparam))
		{
			m_connectstate=enotconnect;
		}
	}else	if(m_connectstate==econnected)
	{
		LPVOID param;
		unsigned nsession=m_pClientNet->GetDieSession(param);

		if(nsession!=m_nSession)
		{
			m_pClientState->OnNetLoopBegin();
			DWORD dwUsed = 0;
			DWORD dwStart = ::GetTickCount();

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
				int nResult = m_pReader->INT();
				Result r = m_pClientState->OnServerMessage(nHeader,m_pReader,len); 
				
				if(r.code != Rc_Successed && r.action != Ra_NOTHING)
				{
					return r;
				}
				dwUsed = ::GetTickCount() - dwStart; //m_pEngineSDK->getTime() - dwStart;
			}

			m_pClientNet->SendAll(); 
			m_pClientState->OnNetLoopEnd();
		}
		else 
		{//连接断开
			m_connectstate=enotconnect;
			ChangeState(0);
			return Result(Ra_RESTART,Rc_Disconnected);
		}
	}
	return Result(Ra_NOTHING,Rc_Successed);
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