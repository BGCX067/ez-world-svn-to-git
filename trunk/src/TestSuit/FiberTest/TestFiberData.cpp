#ifndef _WIN32_WINNT            // ָ��Ҫ������ƽ̨�� Windows Vista��
#define _WIN32_WINNT 0x0600     // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif
#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>
#include <stdio.h>
#include "TestFiberData.h"

CFiber * CTestFiberFactory::Create(UINT nID,LPVOID pMainFiber, SIZE_T stack)
{
	return new CTestFiber(nID,pMainFiber,stack);
}

void CTestFiber::Function()
{
	DWORD dwTime = ::GetTickCount();
	//printf("Fiber Begin:ID = %d,Player = %d\n",GetID(),GetPlayerID());
	while(m_eYieldType <= Yield_Other)
	{
		//printf("Fiber ID = %d,Player = %d,Yield = %d\n",GetID(),GetPlayerID(),m_eYieldType);
		m_eYieldType = (YieldType)((int)m_eYieldType+1);
		if(m_eYieldType <= Yield_Other)
			FYield();
	}
	//printf("Fiber End:ID = %d,Player = %d,Spend = %d ms\n",GetID(),GetPlayerID(),::GetTickCount()-dwTime);
}

