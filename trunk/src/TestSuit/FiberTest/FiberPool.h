#pragma once
#include <queue>
#include <set>
#include "singleton.h"

class CFiber;
class CFiberFactory;
class CFiberPool :public Singleton<CFiberPool>
{
	LPVOID m_pMainFiber;
	UINT m_nMaxFibers;
	CFiber ** m_ppFibers;
	std::queue<UINT> m_FreeFibers;
	std::set<UINT> m_setRunning;
public:
	CFiberPool(UINT nMaxFibers);
	~CFiberPool();

	BOOL Create(CFiberFactory * pFactor,SIZE_T stack = 0);
	void Destroy();

	CFiber * AllocFiber();
	void FreeFiber(UINT nID);
	CFiber * GetFiber(UINT nID);
	CFiber * GetRunningFiber(UINT nID);
};