#ifndef _WIN32_WINNT            // ָ��Ҫ������ƽ̨�� Windows Vista��
#define _WIN32_WINNT 0x0600     // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif
#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>
#include <stdio.h>
#include "LogicInterface.h"
#include "LogicFiber.h"
#include "FiberPool.h"

CFiber * CLogicFiberFactory::Create(UINT nID,LPVOID pMainFiber, SIZE_T stack)
{
	return new CLogicFiber(nID,pMainFiber,stack,m_pLogicInterface,m_szOutBuf);
}

void CLogicFiber::Function()
{
	size_t nOutLen = 0;
	m_pLogicInterface->Call(m_nID,"TestFunction","1234567890",strlen("1234567890"),m_pOutBuf,&nOutLen);
}

int CTestFunction::Call(int nCallID,const char * lpszFunction,const char * pInParams,size_t nInParams,char * pOutParams,size_t * nOutParams)
{
	CLogicFiber * pFiber = (CLogicFiber *)CFiberPool::getSingletonPtr()->GetFiber(nCallID);
	YieldType eType = pFiber->GetYieldType();
	if(eType<=Yield_Other)
	{
		//printf("Fiber ID = %d,Player = %d,Yield = %d\n",pFiber->GetID(),pFiber->GetPlayerID(),eType);
		pFiber->SetYieldType((YieldType)((int)eType+1));
		pFiber->FYield();
	}
	return pFiber->GetYieldType()>=Yield_Max?fc_success:fc_continue;
}