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

#include "support.h"
#include "SupportLoader.h"
#include "LogicInterface.h"

#include "TestFiberData.h"
#include "FiberPool.h"

#define OVERLOAD
#include "overload.h"

IMemManager * pMemManager = NULL;


int _tmain(int argc, _TCHAR* argv[])
{
	// 支持库
	CSupportLoader support;
	if(!support.Load(_T("../bin/log4cplus.properties"),_T("data"),_T("client_update"),NULL))
		return -1;
	pMemManager = support.GetSystem()->GetSysMem();

	int nAlloc = _ttoi(argv[1]);
	printf("nAlloc = %d\n",nAlloc);
	
	CTestFiberFactory testFiberFactory;
	CFiberPool fiberPool(nAlloc);
	if(!fiberPool.Create(&testFiberFactory,0))
	{
		printf("nAlloc Faild\n");
		return -1;
	}

	srand(GetTickCount());
	int nRequest = 4000;
	std::vector<UINT> vecWaiting;
	DWORD dwTime = GetTickCount();
	int nLoop = 0;
	int nAllocNull = 0;
	int nAllocSucess = 0;
	while(1)
	{
		if(!vecWaiting.empty())
		{
			UINT nIndex = rand()%vecWaiting.size();
			UINT nFiberID = vecWaiting[nIndex];

			CTestFiber * pFiber = (CTestFiber*)fiberPool.GetRunningFiber(nFiberID);
			assert(pFiber);

			// 运行之前先设置各种结果
			pFiber->Run();
			vecWaiting.erase(vecWaiting.begin()+nIndex);

			// 如果已经结束了,释放它。
			if(pFiber->IfEnd())
			{
				fiberPool.FreeFiber(pFiber->GetID());
				pFiber = NULL;
			}
			else
			{
				// 变成了新的悬挂
				vecWaiting.push_back(nFiberID);
			}
		}

		if(nRequest > 0)
		{
			CTestFiber * pFiber = (CTestFiber*)fiberPool.AllocFiber();
			if(pFiber)
			{
				nRequest --;
				pFiber->Reset();
				pFiber->SetPlayerID(nRequest+1);
				vecWaiting.push_back(pFiber->GetID());
				pFiber->Run();
				nAllocSucess ++;
			}
			else
			{
				nAllocNull ++;
			}
		}

		nLoop ++;
		if((nRequest == 0) && vecWaiting.empty())
			break;
	}
	printf("Main Spend time = %d,loop = %d,AllocNull = %d,AllocSucess = %d\n",GetTickCount()-dwTime,nLoop,nAllocNull,nAllocSucess);
	fiberPool.Destroy();

	return 0;
}

