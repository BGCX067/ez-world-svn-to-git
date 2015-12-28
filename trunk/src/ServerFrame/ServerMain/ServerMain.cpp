// Server.cpp : 定义控制台应用程序的入口点。
//

#include <Windows.h>
#include <dbghelp.h>
#include <tchar.h>
#include <conio.h>
#include <stdio.h>
#include <Shlwapi.h>
//#include "BugReport.h"

#include "support.h"
#include "SupportLoader.h"
#include "crashoutput.h"
#include "srvlib.h"
#include "servernet.h"

// IsKeyPressQuit. by zhouqq
// 按Q键退出. 忽略大小写。
bool IsKeyPressQuit() ;

class CFrameCallBack:public iFrameCallBack
{
	DWORD dwLastTime;
	unsigned lastsend1,lastsend2;
	iSocketIO *	m_pIO;
	cServerNet *m_pservernet;

public:
	CFrameCallBack(iSocketIO *	pIO,cServerNet *pservernet)
	{
		dwLastTime=GetTickCount();
		lastsend1=lastsend2=0;
		m_pIO=pIO;
		m_pservernet=pservernet;
	}

	virtual bool FrameCallBack()
	{
		SocketIOStatistics Info[2] = {{0},{0}};

		DWORD dwTime = GetTickCount();

		if((dwTime-dwLastTime)>1000) {
			// 连接统计
			for(int i=0;i<2;i++)
				m_pIO->Statistics((SessionStyler)i,&Info[i],0); 

			TCHAR szTitle[128] = {0};
			UINT nFPS = m_pservernet->GetFPS(); 
			_stprintf_s(szTitle,sizeof(szTitle)/sizeof(TCHAR),_T("%s fps = %d SendPool [%d/%d/%d][%d/%d/%d]"),m_pservernet->GetName(),nFPS,
				Info[0].m_ncursendpacket,Info[0].m_nmaxsendpacket,Info[0].m_dwSendTimes-lastsend1,Info[1].m_ncursendpacket,Info[1].m_nmaxsendpacket,Info[1].m_dwSendTimes-lastsend2);
			SetConsoleTitle(szTitle);

			lastsend1=Info[0].m_dwSendTimes;
			lastsend2=Info[1].m_dwSendTimes;
			dwLastTime=dwTime;
		}

		return IsKeyPressQuit();
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	_set_error_mode(_OUT_TO_MSGBOX);
	if(argc<=1)
	{
		printf("使用方式:Server 配置文件名\n");
		printf("按任意键退出\n");
		_getch();
		return 0;
	}


#ifndef _DEBUG
	TCHAR szVersion[64] = {0};
	int ver = GetPrivateProfileInt("ver","ver3",0,"./version.ini"); 
	sprintf(szVersion,"%d",ver);
	COutputCallback callback(szVersion);
	InitStackTraceLibrary(&callback);
#endif

	CSupportLoader support;
	cServerNet myServer(support);

	myServer.CreateConfigTemplate(_T("./server_config.json"));

	if(!myServer.Create(argv[1])) {
		_getch();
		return -1;
	}

	{
		CFrameCallBack fcb(myServer.GetSocketIO(),&myServer);
		myServer.Run(&fcb);

		myServer.Stop();
	}
	return 0;
}
bool IsKeyPressQuit() 
{
	int kb=_kbhit();
	if ( 0!=kb ) {
		char c=_getch() ;
		if ( 'Q'==c || 'q'==c )
			return true ;
	}
	return false ;
}
