// apptest.cpp : 定义控制台应用程序的入口点。
//

#ifndef _WIN32_WINNT            // 指定要求的最低平台是 Windows Vista。
#define _WIN32_WINNT 0x0600     // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <fstream>

#include "support.h"
#include "SupportLoader.h"
#include "LogicInterface.h"
#include "GameProto.h"

#define OVERLOAD
#include "overload.h"

#define LOGIC_DLL	_T("DotNetInterface.dll")
IMemManager * pMemManager = NULL;

// 如何让
class CTestFunction : public iFunctionCall
{
public:
	int Call(int nCallID,const char * lpszFunction,const char * pInParams,size_t nInParams,char * pOutParams,size_t * nOutParams){return 0;}
};


int _tmain(int argc, _TCHAR* argv[])
{
	// 支持库
	CSupportLoader support;
	if(!support.Load(_T("../bin/log4cplus.properties"),_T("data"),_T("client_update"),NULL))
		return -1;
	pMemManager = support.GetSystem()->GetSysMem();

	CModuleLoader<iLogicInterface> logiclib;
	if(!logiclib.Load(LOGIC_DLL,NULL,support.GetSystem()))
		return -1;
	
#ifdef _DEBUG
	const char * lpszDotNetDll = ".\\debug\\DotNetLogicTest.dll";
#else
	const char * lpszDotNetDll = ".\\release\\DotNetLogicTest.dll";
#endif

	CTestFunction testFunctions;
	if(!logiclib.GetInterface()->Initialize(lpszDotNetDll,&testFunctions))
		return -1;
	
	google::protobuf::GameProto gp(support);
	if(!gp.Init(_T("overlord.desc")))
		return -1;

	google::protobuf::Message * pTemplateRoot = gp.New("world.template.Templates");
	if(pTemplateRoot)
	{
		std::ifstream is( "..\\..\\data\\equip.data" );
		bool b = pTemplateRoot->ParseFromIstream(&is);
		is.close();
		int nByteSize = pTemplateRoot->ByteSize();
	}
	
	
	delete pTemplateRoot;
	
	return 0;
}

