#pragma once
#include <set>
#include "Pool.h"
#include "timer.h"
#include "unitallocator.h"
#include "CSLock.h"
#include "netinterface.h"
#include "AsyncSocketIO.h"

class CSocketIO :
	public iSocketIO
{
protected:
	const SocketIOConfig *		m_pConfig;									// IO配置
	CUnitAllocator<OverlapSocket>	m_socketlist[SessionStyler_Max];	// 索引的连接对象列表
	std::set<UINT>			m_newsocketset[SessionStyler_Max];			//new socket set
	std::set<UINT>			m_killedsocketset[SessionStyler_Max];		//killed socket set
	std::set<UINT>			m_failedconnect;							//connect failed socket set
	CThreadSafeObjectPool<Overlap_Send>			m_lstSendOverlaps[SessionStyler_Max];
	CriticalSection	m_killedcs[SessionStyler_Max],m_newcs[SessionStyler_Max],m_failedcs;

public:
	size_t		m_nPacketLengthSize[SessionStyler_Max];	// 包长
	size_t		m_nPacketHeadSize[SessionStyler_Max];	// 包头
	size_t		m_nPacketStructSize[SessionStyler_Max];	// 包头 + 包长 + 包尾	
	DWORD		m_dwLengthMask[SessionStyler_Max];		// 包长的掩码

	CSocketIO(const SocketIOConfig * pConfig);
	~CSocketIO(void);

#ifdef _DEBUG
	void SetDebugFile(unsigned nsession,DWORD flag,LPCTSTR pfilename=NULL);
#endif
	virtual unsigned GetNextSession(LPVOID &param,SessionStyler st,unsigned nsession);
	
	inline UINT GetPacketSize(SessionStyler st,char * packet)
	{
		return *(UINT*)(packet + m_nPacketHeadSize[st]) & m_dwLengthMask[st];
	}
	inline void PutPacketSize(SessionStyler st,char * packet,UINT nLength)
	{
		*(USHORT*)(packet + m_nPacketHeadSize[st]) = nLength;
	}
};
