#pragma once
#include "fiber.h"

enum YieldType
{
	Yield_None,
	Yield_Database,
	Yield_MapServer,
	Yield_Other,
	Yield_Max,
};

class CTestFiber : public CFiber
{
	UINT m_nPlayerID;
	YieldType m_eYieldType;
public:
	CTestFiber(UINT nID,LPVOID pMainFiber, SIZE_T stack)
		:CFiber(nID,pMainFiber,stack)
		,m_nPlayerID(0)
		,m_eYieldType(Yield_None)
	{};
	
	void SetPlayerID(UINT nPlayerID){m_nPlayerID = nPlayerID;}
	UINT GetPlayerID(){return m_nPlayerID;}
	void SetYieldType(YieldType eYield){m_eYieldType = eYield;}
	YieldType GetYieldType(){return m_eYieldType;}
	void Reset(){m_eYieldType = Yield_None;}

	void Function();
};

class CTestFiberFactory : public CFiberFactory
{
public:
	CFiber * Create(UINT nID,LPVOID pMainFiber, SIZE_T stack);
};