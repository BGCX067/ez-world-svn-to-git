#pragma once
#include <assert.h>

/****************************************************************************
*                                class CFiber
****************************************************************************/

class CFiber {
public: // constructors
	CFiber(UINT nID,LPVOID pMainFiber, SIZE_T stack = 0)
		:m_pMainFiber(pMainFiber)
		,m_nID(nID)
	{ 
		fiber = ::CreateFiber(stack, Function, this); 
		//assert(fiber);
	}
	CFiber() { fiber = NULL; }
	virtual ~CFiber() { ::DeleteFiber(fiber); }

	BOOL Create(SIZE_T stack = 0)
	{
		assert(fiber == NULL);
		if(fiber != NULL)
			return FALSE;
		fiber = ::CreateFiber(stack, Function, this);
		return fiber != NULL;
	}

public: // data get/set
	LPVOID GetFiber() { return fiber; }
	UINT GetID()const{return m_nID;};
	BOOL IfEnd(){return m_bEnd;}

public: // methods
	virtual void Function(){}
	void Run() { assert(fiber != NULL);
		LPVOID pFiber = GetCurrentFiber();
		::SwitchToFiber(fiber);
		assert(GetCurrentFiber()==m_pMainFiber);
	}
	void FYield(){
		LPVOID pFiber = GetCurrentFiber();
		::SwitchToFiber(m_pMainFiber);
		assert(GetCurrentFiber()==fiber);
	}
	static void CALLBACK Function(LPVOID p)
	{
		CFiber * pFiber = (CFiber *)p;
		while(1)
		{
			pFiber->m_bEnd = FALSE;
			pFiber->Function();
			pFiber->m_bEnd = TRUE;
			LPVOID _pFiber = GetCurrentFiber();
			::SwitchToFiber(pFiber->m_pMainFiber);
			assert(GetCurrentFiber()==pFiber->fiber);
		}
	}

protected: // data
	LPVOID fiber;
	LPVOID m_pMainFiber;
	UINT m_nID;
	BOOL m_bEnd;
};

class CFiberFactory
{
public:
	virtual CFiber * Create(UINT nID,LPVOID pMainFiber, SIZE_T stack) = 0;
};

