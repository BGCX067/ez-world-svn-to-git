#include <Windows.h>
#include <dbghelp.h>
#include <tchar.h>
#include <conio.h>
#include <stdio.h>
#include <Shlwapi.h>

#include "support.h"
#include "SupportLoader.h"
#include "GameFrame.h"
#include "VersionInfo.h"

bool IsKeyPressQuit() ;
class CFrameCallback:public iFrameCallback
{
	DWORD dwLastTime;
	CGameFrame * m_pClient;

public:
	CFrameCallback(CGameFrame * pClient)
		:m_pClient(pClient)
	{
		dwLastTime = GetTickCount();
	}

	virtual bool FrameCallback()
	{
		DWORD dwTime = GetTickCount();

		if((dwTime-dwLastTime)>1000) {
			TCHAR szTitle[128] = {0};
			_stprintf_s(szTitle,sizeof(szTitle)/sizeof(TCHAR),_T("%s fps = %d"),_T("ClientMain.exe"),m_pClient->GetFps());
			SetConsoleTitle(szTitle);
			dwLastTime=dwTime;
		}

		return IsKeyPressQuit();
	}
};

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

int _tmain(int argc, _TCHAR* argv[])
{
	_set_error_mode(_OUT_TO_MSGBOX);
	if(argc<=1)
	{
		printf("使用方式:ClientMain  启动来源 服务器名 服务器地址\n");
		printf("按任意键退出\n");
		_getch();
		return 0;
	}

	CJsonVersionInfo version(_T("version.json"));
	if(!version.Init())
	{
		_getch();
		return -1;
	}

	CSupportLoader support;
	CGameFrame game(support);
	if(!game.Initialize(_T("ClientMain.json"),argv[1],(CVersionInfo*)&version)) {
		_getch();
		return -1;
	}
	{
		CFrameCallback fcb(&game);
		game.Run(&fcb);
	}
	game.Shutdown();
}