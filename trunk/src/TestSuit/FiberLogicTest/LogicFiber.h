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

// »Á∫Œ»√
class CTestFunction : public iFunctionCall
{
public:
	int Call(int nCallID,const char * lpszFunction,const char * pInParams,size_t nInParams,char * pOutParams,size_t * nOutParams);
};


class iLogicInterface;
class CLogicFiber : public CFiber
{
	iLogicInterface * m_pLogicInterface;
	char * m_pOutBuf;
	UINT m_nPlayerID;
	YieldType m_eYieldType;
public:
	CLogicFiber(UINT nID,LPVOID pMainFiber, SIZE_T stack,iLogicInterface * pLogicInterface,char * pOutBuf)
		:CFiber(nID,pMainFiber,stack)
		,m_nPlayerID(0)
		,m_eYieldType(Yield_None)
		,m_pLogicInterface(pLogicInterface)
		,m_pOutBuf(pOutBuf)
	{};
	
	void SetPlayerID(UINT nPlayerID){m_nPlayerID = nPlayerID;}
	UINT GetPlayerID(){return m_nPlayerID;}
	void SetYieldType(YieldType eYield){m_eYieldType = eYield;}
	YieldType GetYieldType(){return m_eYieldType;}
	void Reset(){m_eYieldType = Yield_None;}

	void Function();
};

class CLogicFiberFactory : public CFiberFactory
{
	iLogicInterface * m_pLogicInterface;
	char m_szOutBuf[65535];

public:
	CLogicFiberFactory(iLogicInterface * pLogicInterface)
		:m_pLogicInterface(pLogicInterface)
	{
	}
	CFiber * Create(UINT nID,LPVOID pMainFiber, SIZE_T stack);
};