// apptest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "support.h"
#include "SupportLoader.h"
#include "NetInterface.h"
#include "LogicInterface.h"
#include "tinyxml.h"

#define OVERLOAD
#include "overload.h"

#define NETLIB_DLL	_T("NetLib.dll")
#define LOGIC_DLL	_T("DotNetInterface.dll")
IMemManager * pMemManager = NULL;

class CTestFunction : public iFunctionCall
{
public:
	int Call(int nID,const char * lpszFunction,const char * pInParams,size_t nInParams,char * pOutParams,size_t * nOutParams){return 0;}
};


int _tmain(int argc, _TCHAR* argv[])
{
	// 支持库
	CSupportLoader support;
	if(!support.Load(_T("log4cplus.properties"),_T("data"),_T("client_update"),NULL))
		return -1;

	pMemManager = support.GetSystem()->GetSysMem();
	
	// 网络库
	SocketIOConfig sio[2];
	ZeroMemory(sio,sizeof(SocketIOConfig)*2);
	for(int i=0;i<2;i++)
	{
		sio[i].nMaxSocket = (i==0)?2048:0;
		sio[i].dwPacketBegin = *(DWORD *)"TSTB";
		sio[i].dwPacketEnd = *(DWORD *)"TSTE";
		sio[i].nRecvBufLen = 4096;
		sio[i].nSendPool = 2048;
		sio[i].bPacketFlags = TRUE;
		sio[i].nDiscardThreshold=1024;
		sio[i].nPendLimit=1048576;
	}
	CModuleLoader<iSocketIO> netlib;
	if(!netlib.Load(NETLIB_DLL,sio,support.GetSystem()))
		return -1;

	CModuleLoader<iLogicInterface> logiclib;
	if(!logiclib.Load(LOGIC_DLL,NULL,support.GetSystem()))
		return -1;
	CTestFunction testFunctions;
	if(!logiclib.GetInterface()->Initialize(".\\debug\\DotNetLogicTest.dll",&testFunctions))
		return -1;
	char szOutBuf[65535]={0};
	size_t nOutLen = 0;
	logiclib.GetInterface()->Call(0,"TestFunction","1234567890",strlen("1234567890"),szOutBuf,&nOutLen);

	
	// LOG测试
	ILogInstance * pTestLog = support.GetSystem()->GetSysLog()->GetRoot();
	LOGF_INFO(pTestLog,_T("%s = %d"),_T("变量"),5);
	LOG_INFO(pTestLog,_T("test 测试"));

	// VFile测试
	{
		CVfsReader fr(support.GetSystem()->GetSysVfs(),_T("client/MapToMapAiPath.xml"),TRUE);
		IVFile * f = fr.GetFile();
		if(f)
		{
			const BYTE * pBuf = f->GetBuf();
			TiXmlDocument xml;
			const char * p = xml.Parse((const char *)pBuf);
			TiXmlNode * node = xml.FirstChild("BUSES");

			pBuf = NULL;
		}
	}
	{
		CVfsReader fr(support.GetSystem()->GetSysVfs(),_T("ui2004/class/class.skin2"),TRUE);
		IVFile * f = fr.GetFile();
		if(f)
		{
			const BYTE * pBuf = f->GetBuf();
			pBuf = NULL;
		}
	}
	// 效率测试

	// 崩溃报告测试

	return 0;
}

