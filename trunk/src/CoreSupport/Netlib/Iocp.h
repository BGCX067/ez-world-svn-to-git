#pragma once

#include <set>
#include <vector>
#include "SocketIO.h"
#include "Thread.h"
#include "CSLock.h"
#include "Pool.h"
//#include "blockAllocator2.h"
//#include <boost/pool/singleton_pool.hpp>

class cReader;
class cIocpIO : public CSocketIO,public cThreadHandle
{
	enum {eBufferSize=65535};

	CThreadSafePool		m_lstSendBuffers[SessionStyler_Max];	// 每个连接的发送组包缓存
	CThreadSafePool		m_RecvBuffers[SessionStyler_Max];		// 每个连接的接收包缓存


	HANDLE				m_hCompletionPort;
	cSocket				m_ListenSocket;
	DWORD				m_dwWorkThreadNumber;
	cThread *			m_pIocpThread;
	DWORD				m_nprovider,m_nconsumer;

	long				m_bytepending[SessionStyler_Max];			//当前悬挂的字节数
	unsigned m_nreqbuffered[SessionStyler_Max][NUM_PRIORITY_LEVEL];	//字节数

	HANDLE				m_hhandlelogic;


	DWORD m_writetimes[2],m_writebytes[2];
	//DWORD maxwritetimes[2],maxwritebytes[2];



	SocketIOStatistics m_Info[SessionStyler_Max];
	char				m_szBuffer[eBufferSize];			// 阻塞模式下发送用

	CriticalSection		m_csWriteLock;						// 多线程发送需要

	unsigned		m_ndebugsession;
	HANDLE			m_debugfilehandle;


	void Clear();
	OverlapSocket * AllocIocpObject(SessionStyler st,LPVOID nType);
	void FreeSocket(OverlapSocket *psocket);
	void ReUseSocket(OverlapSocket *psocket);
	void OnAccept(OverlapSocket * pObject);
	int	OnRecvComplete(OverlapSocket * pObject,DWORD dwbytes);

	//void OnPacketFlagError(UINT nObjID);
	//void OnPacketSerialError(UINT nObjID);

	// 每个继承类的实现可能不一样
	DWORD IocpProc(LPVOID lpParameter);	
	BOOL PostAccept(OverlapSocket * pWhich);
	BOOL PostConnect(OverlapSocket * pWhich,const TCHAR * lpszAddress);
	BOOL PostRecv(OverlapSocket * pWhich);
	BOOL PostSend(OverlapSocket * pWhich,unsigned curlevel,unsigned &maxsend);
	void KillSocket(OverlapSocket * pWhich,LPCTSTR psdes);
	void CopyToSendBuf(const void * ptosend,DWORD nsendlen,unsigned ipriority,OverlapSocket * pWhich);

	void DelSendObj(unsigned st,Overlap_Send * pSendObj);
	// GetStyler, 辅助函数. [zhouqq]
	inline int GetStyler(const unsigned int nSession) const {
		return ((nSession&0x80000000)>>31) ;
	}
	void Read(unsigned long size,void *pbuf,OverlapSocket *pWhich);

	char *AllocRecvBuf(OverlapSocket *pWhich);
	void FreeSendBuf(OverlapSocket *pWhich);
	void PrepareTransmit(OverlapSocket * pWhich);
	
	void GetLevelLeft(unsigned st,unsigned &ilevelsend,unsigned &lastlevelleft);


public:
	cIocpIO(const SocketIOConfig * pConfig);
	~cIocpIO();
	virtual size_t GetPacketStructSize(unsigned st)
	{
		return m_nPacketStructSize[st];
	}
	unsigned GetPacketDataOffset(unsigned st)
	{//包头+长度
		return m_nPacketHeadSize[st]+m_nPacketLengthSize[st];
	}
	const cSocket &GetListenSocket()
	{
		return m_ListenSocket;
	}
	BOOL Listen(IpAddrType eType,const TCHAR * lpszAddress,LPVOID nType);
	unsigned AsyncConnect(const TCHAR * lpszAddress, LPVOID param);//非阻塞
	unsigned int Connect(const TCHAR * lpszAddress,LPVOID param);//阻塞

	virtual BOOL IsConnected(UINT nSession = 1);// 判断是否连接
	void Disconnect(unsigned int nSession);
	void Close();

	void SetPendTimeOut(UINT nSession,DWORD npendtimeout);

	unsigned int GetNewSession(SessionStyler styler,LPVOID &param);//must be called by logic thread
	unsigned GetDieSession(SessionStyler styler,LPVOID &param);//must be called by logic thread
	unsigned GetFailedSession(LPVOID &param);

	size_t Read(unsigned int nSession,cReader &reader);//read one packet from network must be called by logic thread
	//virtual size_t GetReadQueueLength(unsigned int nSession);

	size_t Write(unsigned int nSession,const char * buffer, size_t nSize,unsigned ipriority=0,bool bdiscardbale=false);//write one packet to the network must be called by logic thread
	size_t Write(unsigned int nSession,cWriter &writer,unsigned ipriority=0,bool bdiscardbale=false);

	const TCHAR * GetConnInfo(unsigned int nSession);
	BOOL IsValid(unsigned int nSession);
	void Statistics(SessionStyler st,SocketIOStatistics * info,DWORD flag);
	void SetParam(unsigned nsession,LPVOID lparam);
	LPVOID GetParam(unsigned nsession);
	//virtual unsigned GetNextSession(unsigned nsession=0);
	virtual void SendAll();
	//virtual void Send(unsigned int nSession);
	virtual void SendSyn(unsigned int nSession,char * buffer, size_t nSize);
	//UINT GetConnCount();
	//Connection * GetFirstConn();
	//Connection * GetNextConn();
	virtual void SetSendTimerStampSignature(unsigned session,bool benable=false,DWORD ichar=0,DWORD timerstamp=0);
	virtual void SetRecvTimerStampSignature(unsigned session,bool benable=false,DWORD ichar=0,DWORD timerstamp=0);
	unsigned GetPendedDataSize(unsigned session);

	//virtual void SetLogicHandleIocp(HANDLE);
	//virtual void ReleaseObject(void *);
	virtual void	SetDebugSession(unsigned session);

};
