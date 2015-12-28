#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件：
#include <windows.h>
#include <iostream>
#include <tlhelp32.h>  
#include <tchar.h>  

#include "support.h"
#include "netlib.h"
#include "EngineSDK.h"
#include "ICamera.h"
#include "uimanager2004.h"
#include "LogicEngine.h"

#include "Window.h"
#include "App.h"
#include "State.h"
#include "Icons.h"
#include "Main.h"
#include "errdes.h"


#include "MyApp.h"
#include "Login.h"
#include "SelChar.h"
#include "Gameworld.h"
#include "iStage.h"
#include "ixmdlnode.h"
#include "ixmdlconfig.h"
#include "IEffect2DManager.h"
#define USE_MODEL_CHANNEL
#include "xmdlconfig.h"
#include "SelServer.h"
#include "crashoutput.h"
#include "mathlib.h"
#include "EngineSetting.h"

#define WORLDDEF	_T("../../../public/Data/世界定义_新.xml")

void InitDirect();

CMyClientApp theClientApp;
char  m_CurrentServerName[MAX_SERVERNAME];

StackTraceInitializer stackTraceInitializer(MiniDumpNormal);
//StackTraceInitializer *Singleton<StackTraceInitializer>::ms_Singleton=NULL;


CMyClientApp::CMyClientApp()
{
	InitDirect();
	
	ZeroMemory(&m_Config,sizeof(MyClientConfig));
	m_dwCheckNum = 0;
	m_szSelCharServer[0] = 0;
	m_szGameServer[0] = 0;
	m_szUsername[0] = 0;
	m_pItemClass = NULL;

	// debug
	m_Config.bConsole = GetPrivateProfileInt(_T("debug"),_T("console"),0,CLIENT_INI);
	m_Config.bDebugInfo = GetPrivateProfileInt(_T("debug"),_T("debug"),0,CLIENT_INI);
	m_Config.bDebugState = (BOOL)GetPrivateProfileInt(_T("debug"),_T("debugstate"),0,CLIENT_INI);
	m_Config.bDebugMap = TRUE;
	m_Config.bDebugMove = FALSE;
	// render
	m_Config.bFullScreen =		GetPrivateProfileInt(_T("render"),_T("fullscreen"),-1,CLIENT_COFIG);
	m_Config.bFullScreen =		GetPrivateProfileInt(_T("render"),_T("fullscreen"),-1,CLIENT_COFIG); 
	m_Config.nWidth =			GetPrivateProfileInt(_T("render"),_T("width"),-1,CLIENT_COFIG); 
	m_Config.nHeight =			GetPrivateProfileInt(_T("render"),_T("height"),-1,CLIENT_COFIG); 
	m_Config.nBpp =				GetPrivateProfileInt(_T("render"),_T("bpp"),-1,CLIENT_COFIG);
	m_Config.bGlow=				GetPrivateProfileInt(_T("render"),_T("glow"),-1,CLIENT_COFIG);
	m_Config.bVSync =			GetPrivateProfileInt(_T("render"),_T("vsync"),-1,CLIENT_COFIG);
	m_Config.bAnisotropic =		GetPrivateProfileInt(_T("render"),_T("anisotropic"),-1, CLIENT_COFIG);
	m_Config.nTextureDetail =	GetPrivateProfileInt(_T("render"),_T("texturedetail"),-1, CLIENT_COFIG);
	m_Config.nWaterDetail =		GetPrivateProfileInt(_T("render"),_T("waterdetail"),-1,CLIENT_COFIG);
	m_Config.bWaterRefract =	GetPrivateProfileInt(_T("render"),_T("waterrefract"),-1,CLIENT_COFIG);
	m_Config.bWaterReflect =	GetPrivateProfileInt(_T("render"),_T("waterreflect"),-1,CLIENT_COFIG);
	m_Config.bUseDynamicShadow = GetPrivateProfileInt(_T("render"),_T("dynamicshadow"),-1,CLIENT_COFIG);
	m_Config.nShadowDetail =	GetPrivateProfileInt(_T("render"),_T("shadowdetail"),-1,CLIENT_COFIG);
	m_Config.nVisibleDistance = GetPrivateProfileInt(_T("render"),_T("visibledistance"),-1,CLIENT_COFIG);
	m_Config.nFloraDistance =	GetPrivateProfileInt(_T("render"),_T("floradistance"),-1,CLIENT_COFIG);
	m_Config.bTerrainSpec =		GetPrivateProfileInt(_T("render"),_T("terrainspec"),-1,CLIENT_COFIG);
	m_Config.nReflectDistance = GetPrivateProfileInt(_T("render"), _T("reflectdistance"), -1, CLIENT_COFIG);
	m_Config.bFlora =			GetPrivateProfileInt(_T("render"),_T("flora"),-1,CLIENT_COFIG);
	m_Config.nDisplayQuality =	GetPrivateProfileInt(_T("render"),_T("DisplayQuality"),-1,CLIENT_COFIG);
	if(-1 == m_Config.bFullScreen)
		m_Config.bFullScreen = GetPrivateProfileInt(_T("render"),_T("fullscreen"),0,CLIENT_INI); 
	if(-1 == m_Config.nWidth || -1 == m_Config.nHeight || -1 == m_Config.nBpp )
		LoadScreenSize();
	if(-1 == m_Config.bGlow)
		m_Config.bGlow=	GetPrivateProfileInt(_T("render"),_T("glow"),1,CLIENT_INI);
	if(-1 == m_Config.bVSync)
		m_Config.bVSync = GetPrivateProfileInt(_T("render"),_T("vsync"),0,CLIENT_INI);
	if(-1 == m_Config.bAnisotropic)
		m_Config.bAnisotropic = GetPrivateProfileInt(_T("render"),_T("anisotropic"),1, CLIENT_INI);
	if(-1 == m_Config.nTextureDetail)
		m_Config.nTextureDetail = GetPrivateProfileInt(_T("render"),_T("texturedetail"),2, CLIENT_INI);
	if(-1 == m_Config.nWaterDetail)
		m_Config.nWaterDetail =	GetPrivateProfileInt(_T("render"),_T("waterdetail"),2,CLIENT_INI);
	if(-1 == m_Config.bWaterRefract)
		m_Config.bWaterRefract = GetPrivateProfileInt(_T("render"),_T("waterrefract"),1,CLIENT_INI);
	if(-1 == m_Config.bWaterReflect)
		m_Config.bWaterReflect = GetPrivateProfileInt(_T("render"),_T("waterreflect"),1,CLIENT_INI);
	if(-1 == m_Config.bUseDynamicShadow)
		m_Config.bUseDynamicShadow = GetPrivateProfileInt(_T("render"),_T("dynamicshadow"),1,CLIENT_INI);
	if(-1 == m_Config.nShadowDetail)
		m_Config.nShadowDetail = GetPrivateProfileInt(_T("render"),_T("shadowdetail"),2,CLIENT_INI);
	if(-1 == m_Config.nVisibleDistance)
		m_Config.nVisibleDistance = GetPrivateProfileInt(_T("render"),_T("visibledistance"),2,CLIENT_INI);
	if(-1 == m_Config.nFloraDistance)
		m_Config.nFloraDistance = GetPrivateProfileInt(_T("render"),_T("floradistance"),1,CLIENT_INI);
	if(-1 == m_Config.bTerrainSpec)
		m_Config.bTerrainSpec = GetPrivateProfileInt(_T("render"),_T("terrainspec"),0,CLIENT_INI);
	if(-1 == m_Config.nReflectDistance)
		m_Config.nReflectDistance = GetPrivateProfileInt(_T("render"), _T("reflectdistance"), 1, CLIENT_INI);
	if(-1 == m_Config.bFlora)
		m_Config.bFlora = GetPrivateProfileInt(_T("render"),_T("flora"),0,CLIENT_INI);
	if(-1 == m_Config.nDisplayQuality)
		m_Config.nDisplayQuality = GetPrivateProfileInt(_T("render"),_T("DisplayQuality"),2,CLIENT_INI); 
	//engine
	m_Config.nTerrainDetail = GetPrivateProfileInt(_T("engine"),_T("terraindetail"),2,CLIENT_INI);
	m_Config.bWaterEdgeBlend = GetPrivateProfileInt(_T("engine"), _T("wateredgeblend"),0,CLIENT_INI);
	// system
	DWORD dwRet = GetPrivateProfileString(_T("system"),_T("log"),_T(""),m_Config.szLogFile,MAX_PATH,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("system"),_T("path"),_T(""),m_Config.szBasePath,MAX_PATH,CLIENT_INI);
	m_Config.bIgnoreFiles = GetPrivateProfileInt(_T("system"),_T("IgnoreFiles"),0,CLIENT_INI);
	m_Config.bThreadLoad = GetPrivateProfileInt(_T("system"),_T("ThreadLoad"),0,CLIENT_INI); 
	// socket
	m_Config.socketmode = GetPrivateProfileInt(_T("net"),_T("SockMode"),0,CLIENT_INI);
	// client
	dwRet = GetPrivateProfileString(_T("client"),_T("windowtitle"),_T("CleanWorld"),m_Config.szWindowTitle,MAX_PATH,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("client"),_T("world"),_T(""),m_Config.szLogicDefine,MAX_PATH,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("client"),_T("define"),_T(""),m_Config.szLogicXml,MAX_PATH,CLIENT_INI);
	m_Config.nPickDistance = GetPrivateProfileInt(_T("client"),_T("pick_distance"), 200, CLIENT_INI); 
	// ui
	dwRet = GetPrivateProfileString(_T("ui"),_T("fxpath"),_T(""),m_Config.szUiFxPath,MAX_PATH,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("ui"),_T("texturepath"),_T(""),m_Config.szUiTexturePath,MAX_PATH,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("ui"), _T("NpcPhoto"), _T(""), m_Config.szUiNpcPhoto, MAX_PATH, CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("ui"),_T("skin"),_T(""),m_Config.szUiSkin,MAX_PATH,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("ui"),_T("font"),_T(""),m_Config.szUiFont,MAX_PATH,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("ui"),_T("dlgRelation"),_T(""), m_Config.szUiDlgRelation, MAX_PATH, CLIENT_INI);
	// model group
	dwRet = GetPrivateProfileString(_T("model"),_T("modelpath"),_T(""),m_Config.szModelPath,MAX_PATH,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("model"),_T("named_config"),_T(""),m_Config.szConfig[0],MAX_PATH,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("model"),_T("resource_config"),_T(""),m_Config.szConfig[1],MAX_PATH,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("model"),_T("group_config"),_T(""),m_Config.szConfig[2],MAX_PATH,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("model"),_T("hidden_config"),_T(""),m_Config.szConfig[3],MAX_PATH,CLIENT_INI);
	//上一次选择的服务器名
	dwRet = GetPrivateProfileString(_T("myserver"),_T("server_name"),_T(""),m_Config.szServerName,MAX_SERVERNAME,CLIENT_INI);
	//声音资源路径
	dwRet = GetPrivateProfileString(_T("sound"),_T("sound_path"),_T(""),m_Config.szSoundPath,MAX_PATH,CLIENT_INI);
	TCHAR szFloat[64];
	GetPrivateProfileString(_T("sound"),_T("DistanceMin"),_T("25.6"),szFloat,64,CLIENT_INI);
	m_Config.m_DistanceMin = (float)atof(szFloat);
	GetPrivateProfileString(_T("sound"),_T("DistanceMax"),_T("0.0"),szFloat,64,CLIENT_INI);
	m_Config.m_DistanceMax = (float)atof(szFloat);
	GetPrivateProfileString(_T("sound"),_T("DistanceFactor"),_T("0.1"),szFloat,64,CLIENT_INI);
	m_Config.m_DistanceFactor = (float)atof(szFloat);
	GetPrivateProfileString(_T("sound"),_T("RolloffFactor"),_T("10.0"),szFloat,64,CLIENT_INI);
	m_Config.m_RolloffFactor = (float)atof(szFloat);
	m_Config.nBGMusicVolume = GetPrivateProfileInt(_T("sound"),_T("BGMusicVolume"),-1,CLIENT_COFIG);
	m_Config.nBGSoundVolume = GetPrivateProfileInt(_T("sound"),_T("BGSoundVolume"),-1,CLIENT_COFIG);
	m_Config.nSceneSoundVolume = GetPrivateProfileInt(_T("sound"),_T("SceneSoundVolume"),-1,CLIENT_COFIG);
	m_Config.nRoleSoundVolume = GetPrivateProfileInt(_T("sound"),_T("RoleSoundVolume"),-1,CLIENT_COFIG);
	m_Config.bBGMusic = GetPrivateProfileInt(_T("sound"),_T("BGMusic"),-1,CLIENT_COFIG);
	m_Config.bBGSound = GetPrivateProfileInt(_T("sound"),_T("BGSound"),-1,CLIENT_COFIG);
	m_Config.bSceneSound = GetPrivateProfileInt(_T("sound"),_T("SceneSound"),-1,CLIENT_COFIG);
	m_Config.bRoleSound = GetPrivateProfileInt(_T("sound"),_T("RoleSound"),-1,CLIENT_COFIG);
	if(-1 == m_Config.nBGMusicVolume)
		m_Config.nBGMusicVolume = GetPrivateProfileInt(_T("sound"),_T("BGMusicVolume"),99,CLIENT_INI);
	if(-1 == m_Config.nBGSoundVolume)
		m_Config.nBGSoundVolume = GetPrivateProfileInt(_T("sound"),_T("BGSoundVolume"),99,CLIENT_INI);
	if(-1 == m_Config.nSceneSoundVolume)
		m_Config.nSceneSoundVolume = GetPrivateProfileInt(_T("sound"),_T("SceneSoundVolume"),99,CLIENT_INI);
	if(-1 == m_Config.nRoleSoundVolume)
		m_Config.nRoleSoundVolume = GetPrivateProfileInt(_T("sound"),_T("RoleSoundVolume"),99,CLIENT_INI);
	if(-1 == m_Config.bBGMusic)
		m_Config.bBGMusic = GetPrivateProfileInt(_T("sound"),_T("BGMusic"),1,CLIENT_INI);
	if(-1 == m_Config.bBGSound)
		m_Config.bBGSound = GetPrivateProfileInt(_T("sound"),_T("BGSound"),1,CLIENT_INI);
	if(-1 == m_Config.bSceneSound)
		m_Config.bSceneSound = GetPrivateProfileInt(_T("sound"),_T("SceneSound"),1,CLIENT_INI);
	if(-1 == m_Config.bRoleSound)
		m_Config.bRoleSound = GetPrivateProfileInt(_T("sound"),_T("RoleSound"),1,CLIENT_INI);
	// 角色灯光方向与Y轴的夹角
	m_Config.m_fLightAngleChar = (float)GetPrivateProfileInt(_T("stage"),_T("LightAngleChar"),75,CLIENT_INI);
	// 时段是否变化
	m_Config.m_bTimeStateChange = (BOOL)GetPrivateProfileInt(_T("stage"),_T("TimeStateChange"),1,CLIENT_INI);
	//zone可视范围
	m_Config.m_nZoneVisibleRange = GetPrivateProfileInt(_T("stage"), _T("VisibleRange"), 7, CLIENT_INI);
	LoadCameraOper();
	LoadCameraStageModifier();
	// 登陆注册网页,以及官方网站
	dwRet = GetPrivateProfileString(_T("web"),_T("LoginWeb"),_T(""),m_Config.m_strLoginWeb,1024,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("web"),_T("OfficialWeb"),_T(""),m_Config.m_strOfficialWeb,1024,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("web"),_T("ServiceWeb"),_T(""),m_Config.m_strServiceWeb,1024,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("web"),_T("TeamWeb"),_T(""),m_Config.m_strTeamWeb,1024,CLIENT_INI);
	dwRet = GetPrivateProfileString(_T("web"),_T("MoneyWeb"),_T(""),m_Config.m_MoneyWeb,1024,CLIENT_INI);
	//dwRet = GetPrivateProfileString(_T("User"),_T("LastName"),_T(""),m_Config.m_LastName,128,CLIENT_COFIG);
	//m_Config.m_RemberName = GetPrivateProfileInt(_T("User"),_T("RemberName"),0,CLIENT_COFIG);
	// 寻路
	
	m_Config.m_fZoneRange    = (float)GetPrivateProfileInt(_T("pathfind"),_T("fZoneRange"),    80,  CLIENT_INI) * 10.0f;
	m_Config.m_fPickRange    = (float)GetPrivateProfileInt(_T("pathfind"),_T("fPickRange"),    160, CLIENT_INI) * 10.0f;
	m_Config.m_fPlayerRadius = (float)GetPrivateProfileInt(_T("pathfind"),_T("fPlayerRadius"), 4,  CLIENT_INI);

	m_Config.m_bUseVs = GetPrivateProfileInt(_T("performance"),_T("vs"),FALSE,CLIENT_INI); 
	m_Config.m_bZoomUi = GetPrivateProfileInt(_T("self"),_T("bZoomUi"),0,CLIENT_COFIG);		// 默认换位置的，不是缩放的。
	// 用户自己的配置信息
	m_Config.m_PickAuto = GetPrivateProfileInt(_T("self"),_T("pickauto"),0,CLIENT_COFIG);
	m_Config.m_Attack = GetPrivateProfileInt(_T("self"),_T("attackauto"),0,CLIENT_COFIG);
	m_Config.m_NoGroup = GetPrivateProfileInt(_T("self"),_T("nogroup"),0,CLIENT_COFIG);
	m_Config.m_NoGuild = GetPrivateProfileInt(_T("self"),_T("noguild"),0,CLIENT_COFIG);
	m_Config.m_NoExchange = GetPrivateProfileInt(_T("self"),_T("noexchange"),0,CLIENT_COFIG);
	m_Config.m_NoSystemMessage = GetPrivateProfileInt(_T("self"),_T("nosystemmessage"),0,CLIENT_COFIG);
	m_Config.m_NoSecretMessage = GetPrivateProfileInt(_T("self"),_T("nosecretmessage"),0,CLIENT_COFIG);
	m_Config.m_NoNearMessage = GetPrivateProfileInt(_T("self"),_T("nonearmessage"),0,CLIENT_COFIG);
	m_Config.m_NoGuildMessage = GetPrivateProfileInt(_T("self"),_T("noguildmessage"),0,CLIENT_COFIG);
	m_Config.m_NoGroupMessage = GetPrivateProfileInt(_T("self"),_T("noguoupmessage"),0,CLIENT_COFIG);
	m_Config.m_AreamapQuestion = GetPrivateProfileInt(_T("self"),_T("areamapquestion"),1,CLIENT_COFIG);

	m_Config.m_MinimapScale = (float)GetPrivateProfileInt(_T("self"),_T("minimapscale"),100,CLIENT_COFIG);

	m_Config.PreLoadDirectories = new std::vector<std::string>();
	int nDir = GetPrivateProfileInt(_T("PreLoad"), _T("DirectoryCount"), 0, CLIENT_INI);
	for (int i = 0; i < nDir; i++)
	{
		TCHAR keyName[MAX_PATH];
		TCHAR pattern[MAX_PATH];
		_stprintf(keyName, _T("Directory%d"), i + 1);
		GetPrivateProfileString(_T("PreLoad"), keyName, _T(""), pattern, MAX_PATH, CLIENT_INI);
		m_Config.PreLoadDirectories->push_back(std::string(pattern));
	}

	m_Config.ExcludePreLoadFxs = new std::vector<std::string>();
	int nFx = GetPrivateProfileInt(_T("ExcludePreLoadFx"), _T("Count"), 0, CLIENT_INI);
	for (int i = 0; i < nFx; i++)
	{
		TCHAR keyName[MAX_PATH];
		TCHAR fxName[MAX_PATH];
		_stprintf(keyName, _T("fx%d"), i + 1);
		GetPrivateProfileString(_T("ExcludePreLoadFx"), keyName, _T(""), fxName, MAX_PATH, CLIENT_INI);
		m_Config.ExcludePreLoadFxs->push_back(std::string(fxName));
	}

	m_pConfig = &m_Config;
}
CMyClientApp::~CMyClientApp()
{
	delete m_Config.PreLoadDirectories;
	delete m_Config.ExcludePreLoadFxs;
	ZeroMemory(&m_Config,sizeof(MyClientConfig));
}

void CMyClientApp::ReplaceSomeStr( std::string &strBig, const std::string &strsrc, const std::string &strdst )
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while( (pos=strBig.find(strsrc, pos)) != std::string::npos )
	{
		strBig.replace( pos, srclen, strdst );
		pos += dstlen;
	}
} 

void CMyClientApp::LoadScreenSize()
{
	DWORD iMaxWidth = max(MIN_SUPPORTED_WIDTH, (DWORD)GetSystemMetrics(SM_CXSCREEN));    
	DWORD iMaxHeight = max(MIN_SUPPORTED_HEIGHT, (DWORD)GetSystemMetrics(SM_CYSCREEN));
	float rate = (float)iMaxHeight/(float)iMaxWidth;

	std::vector<DEVMODE> vDisplayMode;
	for (DWORD i = 0; i < 9999; i++)
	{
		DEVMODE devMode;
		memset(&devMode, 0, sizeof(devMode));
		devMode.dmSize = sizeof(DEVMODE);
		if (!EnumDisplaySettings(NULL, i, &devMode))
			break;

		if (devMode.dmPelsWidth >= MIN_SUPPORTED_WIDTH && devMode.dmPelsHeight >= MIN_SUPPORTED_HEIGHT 
			&& devMode.dmPelsWidth <= iMaxWidth && devMode.dmPelsHeight <= iMaxHeight
			&& devMode.dmBitsPerPel == 32)
		{
			bool bFound = false;
			for (UINT j = 0; j < vDisplayMode.size(); j++)
			{
				DEVMODE currDevMode = vDisplayMode[j];
				if (currDevMode.dmPelsWidth == devMode.dmPelsWidth && currDevMode.dmPelsHeight == devMode.dmPelsHeight)
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
				vDisplayMode.push_back(devMode);
		}
	}

	m_Config.nWidth = (int)iMaxWidth;    
	m_Config.nHeight = (int)iMaxHeight;
	m_Config.nBpp = GetPrivateProfileInt(_T("render"),_T("bpp"),32,CLIENT_INI);
	int iFind = -1;
	for (UINT n = 0; n < vDisplayMode.size(); n++)
	{
		float currRate = (float)vDisplayMode[n].dmPelsHeight / (float)vDisplayMode[n].dmPelsWidth;
		if (Math::SameValue(currRate, rate))
		{
			iFind = n;
			break;
		}
	}

	if(-1 != iFind)
	{
		m_Config.nWidth = vDisplayMode[iFind].dmPelsWidth;    
		m_Config.nHeight = vDisplayMode[iFind].dmPelsHeight;
		m_Config.nBpp = 32;
	}

	/*int iMinWidth = 800;
	int iMinHeight = 600;
	int iMaxWidth = GetSystemMetrics(SM_CXSCREEN);    
	int iMaxHeight = GetSystemMetrics(SM_CYSCREEN);
	float rate = (float)iMaxHeight/(float)iMaxWidth;
	std::vector<DisplayMode> vDisplayMode;
	vDisplayMode.clear();
	if(LPDIRECT3D9	pD3D = Direct3DCreate9(D3D_SDK_VERSION))
	{
		const D3DFORMAT allowedAdapterFormatArray[] = 
		{   
			D3DFMT_X8R8G8B8, 
			D3DFMT_X1R5G5B5, 
			D3DFMT_R5G6B5, 
		};
		const UINT allowedAdapterFormatArrayCount  = sizeof(allowedAdapterFormatArray) / sizeof(allowedAdapterFormatArray[0]);
		for( UINT iFormatList = 0; iFormatList < allowedAdapterFormatArrayCount; iFormatList++ )
		{
			D3DFORMAT allowedAdapterFormat = allowedAdapterFormatArray[iFormatList];
			UINT numAdapterModes = pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, allowedAdapterFormat );
			DisplayMode dismode;
			for (UINT mode = 0; mode < numAdapterModes; mode++)
			{
				D3DDISPLAYMODE displayMode;
				pD3D->EnumAdapterModes( D3DADAPTER_DEFAULT, allowedAdapterFormat, mode, &displayMode );

				if( displayMode.Width < (UINT)iMinWidth ||
					displayMode.Height < (UINT)iMinHeight || 
					displayMode.Width > (UINT)iMaxWidth ||
					displayMode.Height > (UINT)iMaxHeight)
				{
					continue;
				}
				dismode.nWidth = displayMode.Width;
				dismode.nHeight = displayMode.Height;
				dismode.nBpp = 32;
				if (displayMode.Format == D3DFMT_X1R5G5B5 || displayMode.Format == D3DFMT_R5G6B5)
				{
					dismode.nBpp = 16;
				}
				BOOL bContains = FALSE;
				for (UINT n=0; n<vDisplayMode.size(); n++)
				{
					if (vDisplayMode[n].nWidth == dismode.nWidth && 
						vDisplayMode[n].nHeight == dismode.nHeight &&
						vDisplayMode[n].nBpp == dismode.nBpp)
					{
						bContains = TRUE;
					}
				}
				if (!bContains)
				{
					vDisplayMode.push_back(dismode);
				}
			}
		}
		pD3D->Release();
		pD3D = NULL;
	}
	m_Config.nWidth = iMaxWidth;    
	m_Config.nHeight = iMaxHeight;
	m_Config.nBpp = GetPrivateProfileInt(_T("render"),_T("bpp"),32,CLIENT_INI);
	int iFind = -1;
	//------- add by machaoyang 2011/02/28 -------
	//	先查找有没有正好可是的，没有的话才去找比例相近的
	for (UINT n=0; n<vDisplayMode.size(); n++)
	{
		if(vDisplayMode[n].nHeight == iMaxHeight &&
			vDisplayMode[n].nWidth == iMaxWidth)
		{
			iFind = n;
			break;
		}
	}
	if(-1 == iFind)
		for (UINT n=0; n<vDisplayMode.size(); n++)
		{
			float currRate = (float)vDisplayMode[n].nHeight/(float)vDisplayMode[n].nWidth;
			if (Math::SameValue(currRate, rate))
			{
				iFind = n;
				break;
			}
		}
	if(-1 != iFind)
	{
		m_Config.nWidth = vDisplayMode[iFind].nWidth;    
		m_Config.nHeight = vDisplayMode[iFind].nHeight;
		m_Config.nBpp = vDisplayMode[iFind].nBpp;
	}*/
}

void CMyClientApp::LoadCameraOper()
{
	TCHAR szFloat[64];
	GetPrivateProfileString(_T("CameraOperation"),_T("maxview"),_T("270"),szFloat,64,CLIENT_INI);
	m_Config.m_MaxView = (float)atof(szFloat);
	GetPrivateProfileString(_T("CameraOperation"),_T("minview"),_T("35"),szFloat,64,CLIENT_INI);
	m_Config.m_MinView = (float)atof(szFloat);
	GetPrivateProfileString(_T("CameraOperation"),_T("NearPlane"),_T("35"),szFloat,64,CLIENT_INI);
	m_Config.m_NearPlane = (float)atof(szFloat);
	GetPrivateProfileString(_T("CameraOperation"),_T("RotationScaler"),_T("0.005"),szFloat,64,CLIENT_INI);	
	m_Config.m_RotationScaler = (float)atof(szFloat);
	GetPrivateProfileString(_T("CameraOperation"),_T("DistScaler"),_T("0.05"),szFloat,64,CLIENT_INI);	
	m_Config.m_DistScaler = (float)atof(szFloat);
	GetPrivateProfileString(_T("CameraOperation"),_T("DistancePreSecond"),_T("20.0"),szFloat,64,CLIENT_INI);	
	m_Config.m_DistancePreSecond = (float)atof(szFloat);
}

void CMyClientApp::LoadCameraStageModifier()
{
	TCHAR szFloat[64];
	GetPrivateProfileString(_T("CameraStageModifier"),_T("fForwardPreFrame "),_T("20.0"),szFloat,64,CLIENT_INI);
	m_Config.fForwardPreFrame = (float)atof(szFloat);
	GetPrivateProfileString(_T("CameraStageModifier"),_T("fbackwardPreFrame "),_T("20.0"),szFloat,64,CLIENT_INI);
	m_Config.fBackwardPreFrame = (float)atof(szFloat);
	GetPrivateProfileString(_T("CameraStageModifier"),_T("fMinDist "),_T("35.0"),szFloat,64,CLIENT_INI);
	m_Config.fMinDist = (float)atof(szFloat);
	m_Config.dwBackDelay = GetPrivateProfileInt(_T("CameraStageModifier"),_T("dwBackDelay"),500,CLIENT_INI);
	GetPrivateProfileString(_T("CameraStageModifier"),_T("MyOpacityDist "),_T("20.0"),szFloat,64,CLIENT_INI);
	m_Config.fMyOpacityDist = (float)atof(szFloat);
	GetPrivateProfileString(_T("CameraStageModifier"),_T("MyDisapperDist "),_T("20.0"),szFloat,64,CLIENT_INI);
	m_Config.fMyDisapperDist = (float)atof(szFloat);
	GetPrivateProfileString(_T("CameraStageModifier"),_T("OtherOpacityDist "),_T("20.0"),szFloat,64,CLIENT_INI);
	m_Config.fOtherOpacityDist = (float)atof(szFloat);
	GetPrivateProfileString(_T("CameraStageModifier"),_T("OtherDisapperDist "),_T("20.0"),szFloat,64,CLIENT_INI);
	m_Config.fOtherDisapperDist = (float)atof(szFloat);
	m_Config.nCameraMode = GetPrivateProfileInt(_T("CameraStageModifier"),_T("CameraMode"),1,CLIENT_INI);

	if(m_Config.fMyOpacityDist<m_Config.fMyDisapperDist)
		m_Config.fMyOpacityDist = m_Config.fMyDisapperDist;
	if(m_Config.fOtherOpacityDist<m_Config.fOtherDisapperDist)
		m_Config.fOtherOpacityDist = m_Config.fOtherDisapperDist;
}

BOOL CMyClientApp::OnInitialize()
{
	D3DCAPS9 caps;
	m_pEngineSDK->GetD3dDevice()->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0))
		m_pConfig->m_bUseVs = FALSE;

	m_pEngineSDK->GetMdlManager()->SetFxPath(_T("dxsas\\"));
	m_pEngineSDK->GetMdlManager()->SetPerformanceFlags(m_pConfig->m_bUseVs);

	IStage * pStage = (IStage*)m_pEngineSDK->GetStage();
	pStage->SetNodePath(STAGE_SKY,_T("dxsas\\sky\\"),_T("stage\\sky\\texture\\"));
	pStage->SetNodePath(STAGE_TERRAIN,_T("dxsas\\Terrain\\"),_T("stage\\terrain\\texture\\"));
	pStage->SetNodePath(STAGE_SCENE,_T("dxsas\\Scene\\"),_T("xmdl\\scene\\texture\\"));
	pStage->GetMdlManager()->SetFxPath(_T("dxsas\\"));
	pStage->GetMdlManager()->SetTexturePath(_T("xmdl\\scene\\texture\\"));
	pStage->GetMdlManager()->SetPerformanceFlags(m_pConfig->m_bUseVs);
	pStage->ShowFlora(m_pConfig->bFlora);

	BOOL bRet;
	if (m_pConfig->bUseDynamicShadow)
	{
		bRet = m_pEngineSDK->AddSceneFx("dxsas\\Post_ShadowMap.fx");
		if (!bRet)
		{
			m_pEngineSDK->AddSceneFx("dxsas\\ProjectShadow.fx");
			_printf("use project shadow\n");
		}
	}	
	
	// 初始化摄象机
	ICamera* pCamera = m_pEngineSDK->GetCamera(); 
	pCamera->SetProjParams(34.516f,m_pConfig->nWidth/(float)m_pConfig->nHeight);
	pCamera->SetViewParams(); 
	
	// 模型配置文件
	BOOL b = XMdlConfig::CModelGroupConfig::GetPointer()->LoadConfig(m_Config.szConfig[0],m_Config.szConfig[1],m_Config.szConfig[2],m_Config.szConfig[3]);
	if(!b)
	{
		_error(_T("Load model config file %s,%s,%s failed\n"),m_Config.szConfig[0],m_Config.szConfig[1],m_Config.szConfig[2]);
		return FALSE;
	}
	XMdlConfig::CModelGroupConfig::GetPointer()->SetModelPath(m_Config.szModelPath);

	// 初始化状态
	CreateFn * pCreateFn[] = {
		CreateState<CLoginState>,
		CreateState<CSelServer>,
		CreateState<CSelCharState>,
		CreateState<CCreateRoleModule>,
		CreateState<CGameWorldState>,
	};

	UINT nCounts = sizeof(pCreateFn)/sizeof(CreateFn *);
	InitState(pCreateFn,nCounts);

	// 初始化2D效果器
	if(m_pEngineSDK->GetEffect2DManager())
		m_pEngineSDK->GetEffect2DManager()->LoadConfig(_T("Effect2D\\Effect2D.xml"));

	// 初始化RenderTarget管理器
	bRet = m_pEngineSDK->InitExtraRenderTarget(_T("Dxsas\\post_extra.fx"));
	assert(bRet);
	
	return TRUE;
}
void CMyClientApp::OnShutdown()
{
	//delete m_pItemClass;
	
	// 写INI
	TCHAR szBuf[64];
	WritePrivateProfileString(_T("render"),_T("fullscreen"),_itoa(m_pConfig->bFullScreen,szBuf,10),CLIENT_COFIG); 
	WritePrivateProfileString(_T("render"),_T("DisplayQuality"),_itoa(m_pConfig->nDisplayQuality,szBuf,10),CLIENT_COFIG); 
	WritePrivateProfileString(_T("render"),_T("width"),_itoa(m_pConfig->nWidth,szBuf,10),CLIENT_COFIG); 
	WritePrivateProfileString(_T("render"),_T("height"),_itoa(m_pConfig->nHeight,szBuf,10),CLIENT_COFIG); 
	WritePrivateProfileString(_T("render"),_T("bpp"), _itoa(m_pConfig->nBpp, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("vsync"),_itoa(m_pConfig->bVSync, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("glow"),_itoa(m_pConfig->bGlow, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("anisotropic"),_itoa(m_pConfig->bAnisotropic, szBuf, 10), CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("dynamicshadow"),_itoa(m_pConfig->bUseDynamicShadow, szBuf, 10), CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("texturedetail"),_itoa(m_pConfig->nTextureDetail, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("waterdetail"),_itoa(m_pConfig->nWaterDetail, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("waterreflect"),_itoa(m_pConfig->bWaterReflect, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("waterrefract"),_itoa(m_pConfig->bWaterRefract, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("terrainspec"),_itoa(m_pConfig->bTerrainSpec, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("shadowdetail"),_itoa(m_pConfig->nShadowDetail, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("visibledistance"),_itoa(m_pConfig->nVisibleDistance, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("floradistance"),_itoa(m_pConfig->nFloraDistance, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("reflectdistance"),_itoa(m_pConfig->nReflectDistance, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("render"),_T("flora"),_itoa(m_pConfig->bFlora,szBuf,10),CLIENT_COFIG); 

	WritePrivateProfileString(_T("sound"),_T("BGMusicVolume"),_itoa(m_pConfig->nBGMusicVolume, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("sound"),_T("BGSoundVolume"),_itoa(m_pConfig->nBGSoundVolume, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("sound"),_T("SceneSoundVolume"),_itoa(m_pConfig->nSceneSoundVolume, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("sound"),_T("RoleSoundVolume"),_itoa(m_pConfig->nRoleSoundVolume, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("sound"),_T("BGMusic"),_itoa(m_pConfig->bBGMusic, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("sound"),_T("BGSound"),_itoa(m_pConfig->bBGSound, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("sound"),_T("SceneSound"),_itoa(m_pConfig->bSceneSound, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("sound"),_T("RoleSound"),_itoa(m_pConfig->bRoleSound, szBuf, 10),CLIENT_COFIG);
	WritePrivateProfileString(_T("packet"),_T("IgnoreFiles"),_itoa(m_pConfig->bIgnoreFiles,szBuf,10),CLIENT_INI); 
	//本人配置
	WritePrivateProfileString(_T("self"),_T("pickauto"),_itoa(m_pConfig->m_PickAuto,szBuf,10),CLIENT_COFIG);
	WritePrivateProfileString(_T("self"),_T("attackauto"),_itoa(m_pConfig->m_Attack,szBuf,10),CLIENT_COFIG);
	WritePrivateProfileString(_T("self"),_T("nogroup"),_itoa(m_pConfig->m_NoGroup,szBuf,10),CLIENT_COFIG);
	WritePrivateProfileString(_T("self"),_T("noguild"),_itoa(m_pConfig->m_NoGuild,szBuf,10),CLIENT_COFIG);
	WritePrivateProfileString(_T("self"),_T("noexchange"),_itoa(m_pConfig->m_NoExchange,szBuf,10),CLIENT_COFIG);
	WritePrivateProfileString(_T("self"),_T("nosystemmessage"),_itoa(m_pConfig->m_NoSystemMessage,szBuf,10),CLIENT_COFIG);
	WritePrivateProfileString(_T("self"),_T("nosecretmessage"),_itoa(m_pConfig->m_NoSecretMessage,szBuf,10),CLIENT_COFIG);
	WritePrivateProfileString(_T("self"),_T("nonearmessage"),_itoa(m_pConfig->m_NoNearMessage,szBuf,10),CLIENT_COFIG);
	WritePrivateProfileString(_T("self"),_T("noguildmessage"),_itoa(m_pConfig->m_NoGuildMessage,szBuf,10),CLIENT_COFIG);
	WritePrivateProfileString(_T("self"),_T("noguoupmessage"),_itoa(m_pConfig->m_NoGroupMessage,szBuf,10),CLIENT_COFIG);
	WritePrivateProfileString(_T("self"),_T("minimapscale"),_itoa((int)m_pConfig->m_MinimapScale,szBuf,10),CLIENT_COFIG);
	WritePrivateProfileString(_T("self"),_T("areamapquestion"),_itoa(m_pConfig->m_AreamapQuestion,szBuf,10),CLIENT_COFIG);
}
