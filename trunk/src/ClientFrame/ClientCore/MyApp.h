#pragma  once
#include "msg.h"
#define CLIENT_INI	_T("./client.ini")
#define CLIENT_COFIG _T("./myconfig.ini")
struct MyClientConfig : public ClientConfig
{
	BOOL bDebugMap;		// 是否使用未开放地图
	BOOL bDebugMove;	// 是否允许自由移动
	BOOL bDebugState;	// 显示玩家自己的调试状态

	// 场景摄象机纠正
	float fForwardPreFrame;
	float fBackwardPreFrame;
	float fMinDist;
	DWORD dwBackDelay;
	int nCameraMode;
	
	// 摄象机角色透明
	float fMyOpacityDist;
	float fMyDisapperDist;
	float fOtherOpacityDist;
	float fOtherDisapperDist;
};

enum CLIENT_STATE
{
	CLIENT_CONTINUE = -1,	// 不转换状态
	CLIENT_LOGIN,			// 登陆
	CLIENT_SELSERVER,		//选择服务器
	CLIENT_CHARACTER,		// 选择角色
	CLIENT_CREATECHAR,		// 创建角色
	CLIENT_GAME,			// 游戏
	CLIENT_STATE_MAX,
};


class CItemClassesEx;
class CMyClientApp : public CClientApp
{
	BOOL OnInitialize();
	void OnShutdown();
	
public:
	CMyClientApp();
	~CMyClientApp();

	MyClientConfig m_Config;
	DWORD m_dwCheckNum;
	TCHAR m_szSelCharServer[_MAX_SERVERNAME];
	TCHAR m_szGameServer[_MAX_SERVERNAME];
	TCHAR m_szUsername[32];
	

	CItemClassesEx * m_pItemClass;

	void LoadCameraStageModifier();
	void LoadCameraOper();
	void LoadScreenSize();

	void ReplaceSomeStr( std::string &strBig, const std::string &strsrc, const std::string &strdst );
};
