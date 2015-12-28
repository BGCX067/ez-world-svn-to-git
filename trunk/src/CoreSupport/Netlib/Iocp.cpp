/**************PROGRAMMED BY QIUXINYU********************/

#include <TCHAR.h>
#include <intrin.h>
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>			// Windows 头文件:
#include <winsock2.h>
#include <Mswsock.h>
#include <assert.h>
#include "thread.h"
#include "support.h"
#include "timer.h"
#include "ModuleSupport.h"

#include "netlib.h"
#include "socket.h"
#include "iocp.h"
#include "readerwriter.h"

#define OVERLOAD
#include "overload.h"

struct SENDSTATISTICS{
	UINT nsend;
	UINT npend;
};

LPCTSTR g_lpszWhich[] = {_T("server modual"),_T("connect modual")};
TCHAR szNum[64]={0};

DWORD cIocpIO::IocpProc(LPVOID lpParam)	
{
	DWORD	dwBytes = 0;
	DWORD	dwClientId;
	OverlapSocketOp * pObject;
	while(1)
	{
		if(GetQueuedCompletionStatus(m_hCompletionPort,
					&dwBytes, 
					&dwClientId,
					(LPOVERLAPPED *)&pObject, 
					INFINITE ))
		{
			// 什么情况下 dwBytes 为0?
			if(dwBytes!=-1)
			{
				switch(pObject->cmd)
				{
				case	IocpCmd_Accept:
					{
						//dwClientId 为ListenScoket CreateIoCompletionPort附加
						OverlapSocket * pIocpSocket = (OverlapSocket *)(OverlapSocketOp *)pObject;
						OnAccept(pIocpSocket);
						continue;
					}
				case	IocpCmd_Send:
					{
						Overlap_Send * psendobj;
						psendobj=(Overlap_Send *)pObject;

						OverlapSocket * pIocpSocket = psendobj->pIocpSocket;
						unsigned st=(dwClientId&0x80000000)>>31;
						assert(m_socketlist[st].GetId(pIocpSocket)==(dwClientId&0x7fffffff));

						_InterlockedExchangeAdd((long *)(m_bytepending+st),-(long)psendobj->dwSend);
						_InterlockedExchangeAdd((long *)&pIocpSocket->nbytepending,-(long)psendobj->dwSend);
						_InterlockedDecrement((LONG*)&pIocpSocket->nreqpending);


						if(!dwBytes)
						{
							int nErr = WSAGetLastError();
							TCHAR buf[0x100];
							_stprintf_s(buf,0x100,_T("err=%d,send 0 byte"),nErr);
							KillSocket(pIocpSocket,buf);
						}

						psendobj->bsendcomplete=true;
					}
					continue;
				case	IocpCmd_Recv:
					{
						OverlapSocket * pIocpSocket = (OverlapSocket *)(OverlapSocketOp *)pObject;
						unsigned st=(dwClientId&0x80000000)>>31;
						assert(pIocpSocket->Id==dwClientId);
						if(dwBytes)
						{
							_InterlockedExchangeAdd( (LONG*)&m_Info[st].m_dwRecvBytes, (LONG)dwBytes );
							if(OnRecvComplete(pIocpSocket,dwBytes)<0)
							{
							}
						}
						else
						{//to do输入缓冲太小,增加RecvBufLen
							int nErr = WSAGetLastError();
							TCHAR buf[0x100];
							_stprintf_s(buf,0x100,_T("err=%d,收到0字节"),nErr);
							KillSocket(pIocpSocket,buf);
						}
						_InterlockedDecrement((LONG*)&pIocpSocket->nrecving);
						_InterlockedDecrement((LONG*)&pIocpSocket->nreqpending);
					}
					continue;
				case IocpCmd_Connect:
					{
						OverlapSocket * pIocpSocket=m_socketlist[(dwClientId&0x80000000)>>31].Get(dwClientId&0x7fffffff);
						setsockopt( pIocpSocket->socket.GetHandle(),
							SOL_SOCKET,
							SO_UPDATE_CONNECT_CONTEXT,
							NULL,
							0);
						PrepareTransmit(pIocpSocket);

						if(!PostRecv(pIocpSocket)) //for iocp fast reaction
						{
							KillSocket(pIocpSocket,_T("PostRecv失败"));
						}
						pIocpSocket->socket.SetConnected();
						//通知逻辑层
						m_newcs[SessionStyler_Consumer].Enter();
						m_newsocketset[SessionStyler_Consumer].insert(dwClientId);
						m_newcs[SessionStyler_Consumer].Leave();

						_InterlockedDecrement((LONG*)&pIocpSocket->nreqpending);
					}
					continue;
				default:
					assert(0);
				}
			}
			else
				break;
		}
		else
		{//ERROR_SUCCESS
			// If GetQueuedCompletionStatus returns ERROR_SUCCESS, with lpNumberOfBytes equal zero,a socket handle associated with a completion port is closed. 
			assert(pObject);
			int nErr = WSAGetLastError();
			if(dwClientId==INVALID_SESSION)//在2k server环境下会走到Listen socket本身
			{
				continue;
			}
			unsigned st=(dwClientId&0x80000000)>>31;
			OverlapSocket * pIocpSocket=m_socketlist[st].Get(dwClientId&0x7fffffff);
			if(pIocpSocket)
			{
				if(pObject->cmd==IocpCmd_Connect)
				{
					pIocpSocket->berror=true;
					pIocpSocket->socket.Destroy();
					m_failedcs.Enter();
					m_failedconnect.insert(pIocpSocket->Id);
					m_failedcs.Leave();
				}
				else
				{
					if(pObject->cmd==IocpCmd_Send)
					{
						Overlap_Send * psendobj;
						psendobj=(Overlap_Send *)pObject;
						_InterlockedExchangeAdd((long *)(m_bytepending+st),-(long)psendobj->dwSend);
						_InterlockedExchangeAdd((long *)&pIocpSocket->nbytepending,-(long)psendobj->dwSend);
						psendobj->bsendcomplete=true;
					}else	if(pObject->cmd==IocpCmd_Recv)
					{
						_InterlockedDecrement((LONG*)&pIocpSocket->nrecving);
					}
					TCHAR buf[0x100];
					_stprintf_s(buf,0x100,_T("异步操作返回错误,错误代码=%d"),nErr);
					KillSocket(pIocpSocket,buf);
				}
				_InterlockedDecrement((LONG*)&pIocpSocket->nreqpending);
			}
		}
	}
	return 1;
}

void cIocpIO::Clear()
{
	for(unsigned i=0;i<SessionStyler_Max;i++)
	{
		m_writetimes[i]=m_writebytes[i]/*=maxwritetimes[i]=maxwritebytes[i]*/=0;
		m_Info[i].maxFrameSendBytes=0;
		m_Info[i].maxFrameSendTimes=0;
		m_Info[i].m_dwSendBytes=0;
		m_Info[i].m_dwSendedBytes=0;
		m_bytepending[i]=0;
	}

	m_hCompletionPort = NULL;
	m_dwWorkThreadNumber = 0;
	m_pIocpThread = NULL;

	ZeroMemory(&m_Info,sizeof(SocketIOStatistics)*SessionStyler_Max);
	memset(m_nreqbuffered,0,sizeof(m_nreqbuffered));
}

#pragma warning( disable : 4407 )

cIocpIO::cIocpIO(const SocketIOConfig * pConfig)
:CSocketIO(pConfig)
{
	Clear();
	m_ndebugsession=INVALID_SESSION;

	m_nprovider=pConfig[SessionStyler_Provider].nMaxSocket;
	m_nconsumer=pConfig[SessionStyler_Consumer].nMaxSocket;

	if(m_nprovider)
	{
		//if(pConfig->nRecvBufLen < (sizeof( SOCKADDR_IN) * 2 + 32))
		//	pConfig->nRecvBufLen = (sizeof( SOCKADDR_IN) * 2 + 32);

		m_socketlist[SessionStyler_Provider].Create(m_nprovider);

		m_lstSendOverlaps[SessionStyler_Provider].Create(pConfig->nSendPool);
		m_lstSendBuffers[SessionStyler_Provider].Create(pConfig->nSendPool,pConfig->nSendBufLen+sizeof(SendBuf));

		m_RecvBuffers[SessionStyler_Provider].Create(2*m_nprovider,(pConfig->nRecvBufLen+sizeof(void *)));

		assert(pConfig->nPendLimit>0);
		assert(pConfig->nDiscardThreshold>0);

	}

	if(m_nconsumer){
		pConfig++;
		//if(pConfig->nRecvBufLen < (sizeof( SOCKADDR_IN) * 2 + 32))
		//	pConfig->nRecvBufLen = (sizeof( SOCKADDR_IN) * 2 + 32);

		m_socketlist[SessionStyler_Consumer].Create(m_nconsumer);
		m_lstSendOverlaps[SessionStyler_Consumer].Create(pConfig->nSendPool);
		m_lstSendBuffers[SessionStyler_Consumer].Create(pConfig->nSendPool,pConfig->nSendBufLen+sizeof(SendBuf));

		m_RecvBuffers[SessionStyler_Consumer].Create(2*m_nconsumer,(pConfig->nRecvBufLen+sizeof(void *)));

		assert(pConfig->nPendLimit>0);
		assert(pConfig->nDiscardThreshold>0);
	}

	m_hCompletionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

	// 如果还没有启动线程，则启动线程
	
	SYSTEM_INFO	si;
	GetSystemInfo( &si );
	m_dwWorkThreadNumber = si.dwNumberOfProcessors * 2;

	m_pIocpThread = new cThread[m_dwWorkThreadNumber];
	for( DWORD i = 0;i < m_dwWorkThreadNumber;i ++ )
	{
		cThreadHandle * pHandle = (cThreadHandle *)((void *)this);
		ThreadProc pfnProc = static_cast<ThreadProc>(&cIocpIO::IocpProc);
		if( m_pIocpThread[i].Create( 0,pHandle,pfnProc,0) == NULL )
			return;
	}

}

cIocpIO::~cIocpIO()
{
	if(m_nprovider)
	{
	}

	if(m_nconsumer)
	{
	}

	Clear();
}

BOOL cIocpIO::Listen(IpAddrType eType,const TCHAR * lpszAddress,LPVOID param)
{
	if(m_ListenSocket.Create(IPPROTO_TCP))
	{
		if(m_ListenSocket.Listen(eType,lpszAddress))
		{
			if(!CreateIoCompletionPort((HANDLE)m_ListenSocket.GetHandle(), m_hCompletionPort, 0xffffffff,0))
				return FALSE;

			for(int i=0; i<m_pConfig->nMaxSocket; i++) 
			{
				OverlapSocket * pObject;
				if(pObject=AllocIocpObject(SessionStyler_Provider,param))
				{
					PrepareTransmit(pObject);
					if(!PostAccept(pObject))
						return FALSE;
				}
			}
			return TRUE;
		}
		else
		{
			assert(0);//该地址已被占用
			m_ListenSocket.Destroy(); 
		}
	}
	return FALSE;
}

unsigned cIocpIO::AsyncConnect(const TCHAR * lpszAddress, LPVOID param)
{
	 //如果还没有启动线程，则启动线程
	 //分配对象
	OverlapSocket * pWhich = NULL;
	if(!m_nconsumer)
	{
		LOG_ERROR(DLL_LOGGER,_T("socket nubmer not configuredn"));
		assert(0);
		return 0;
	}
	if(pWhich=AllocIocpObject(SessionStyler_Consumer,param))
	{
		// 连接
		assert(pWhich->socket.GetHandle() != 0xffffffff);

		if(PostConnect(pWhich,lpszAddress))
		{
			// 保存连接的目的地址
			lstrcpy(pWhich->info.szIP,lpszAddress);
			pWhich->info.dwStartTime = GetTickCount();
			return pWhich->Id;
		}

		if(pWhich->socket.IsValid())
			pWhich->socket.Destroy();
		m_socketlist[SessionStyler_Consumer].Del(pWhich);

	}else{
		LOG_ERROR(DLL_LOGGER,_T("connection port can not allocate socket on iocp,please add MaxSocket number!"));
	}
	assert(0);
	return INVALID_SESSION;
}

BOOL cIocpIO::IsConnected(UINT nsession)
{
	OverlapSocket *psocket;
	psocket=m_socketlist[(nsession&0x80000000)>>31].Get(nsession&0x7fffffff);
	if(!psocket)
		return FALSE;
	return psocket->socket.IsConnected();
}

void cIocpIO::PrepareTransmit(OverlapSocket * pWhich)
{
	unsigned st=(pWhich->Id&0x80000000)>>31;
	pWhich->pRecvBuf=(char *)m_RecvBuffers[st].New();
	assert(pWhich->pRecvBuf);
	*(char **)pWhich->pRecvBuf=(char *)m_RecvBuffers[st].New();
	assert(*(char **)pWhich->pRecvBuf);
	**(char ***)pWhich->pRecvBuf=0;

	pWhich->pAllocatedBase=pWhich->pReadBuf=pWhich->pRecvBuf;

	pWhich->pReadStart=pWhich->pRecvBuf+sizeof(void *);
	pWhich->pWriteStart=pWhich->pReadStart;
	

	InitializeCriticalSection(&pWhich->cs);
}

unsigned int cIocpIO::Connect(const TCHAR * lpszAddress, LPVOID param)
{
	// 分配对象
	OverlapSocket * pWhich = NULL;
	if(pWhich=AllocIocpObject(SessionStyler_Consumer,param))
	{
		// 连接
		assert(pWhich->socket.GetHandle() != 0xffffffff); 
		if(pWhich->socket.Connect(lpszAddress/*,(long)dwWaiting*/))
		{
			// 保存连接的目的地址
			lstrcpy(pWhich->info.szIP,lpszAddress);
			pWhich->info.dwStartTime = GetTickCount(); 
			// Post Recive
			pWhich->bnewsession=true;
			PrepareTransmit(pWhich);
 
			if(PostRecv(pWhich)) // 从缓冲区头开始接 for iocp fast reaction
			{
				return pWhich->Id;
			}
		}
		pWhich->berror=true;
		FreeSocket(pWhich);
	}
	return INVALID_SESSION;
}

void cIocpIO::Disconnect(unsigned int nSession)
{
	assert(nSession!=-1);
	if(nSession==-1)
		return;
	OverlapSocket *psocket=m_socketlist[(nSession&0x80000000)>>31].Get(nSession&0x7fffffff);
	if(!psocket)
	{
		LOG_ERROR(DLL_LOGGER,
			_T("When Disconnect socket sessionid=0x")
			<<_itot((INT_PTR)nSession,szNum,16)
			<<_T(" socket not found"));
		assert(0);
		return;
	}
	if(!psocket->bnewsession)
	{
		LOG_ERROR(DLL_LOGGER,
			_T("When Disconnect socket sessionid=0x")
			<<_itot((INT_PTR)nSession,szNum,16)
			<<_T(" ,socket not send to logic layer"));
		assert(0);
		return;
	}
	if(!psocket->socket.IsValid())
	{
		LOG_ERROR(DLL_LOGGER,
			_T("When Disconnect socket sessionid=0x")
			<<_itot((INT_PTR)nSession,szNum,16)
			<<_T(" ,socket invalid"));
		return;
	}
	KillSocket(psocket,_T("Disconnect"));
	psocket->socket.Destroy();
}

BOOL cIocpIO::IsValid(unsigned int nSession)
{
	OverlapSocket * pWhich = m_socketlist[(nSession&0x80000000)>>31].Get(nSession&0x7fffffff);
	assert(pWhich);
	if(!pWhich)
		return FALSE;
	return pWhich->socket.IsValid();
}


void cIocpIO::Close()
{
	// 停止 iocp 线程
	LOG_INFO(DLL_LOGGER,_T("Close iocp Handle"));
	HANDLE hthread[64];
	DWORD i;
	for(  i= 0;i < m_dwWorkThreadNumber;i ++ )
	{
		PostQueuedCompletionStatus(m_hCompletionPort,-1,-1,NULL);
		hthread[i]=m_pIocpThread[i].GetHandle();
	}
	WaitForMultipleObjects(i,hthread,TRUE,INFINITE);
	for( i = 0;i < m_dwWorkThreadNumber;i ++ )
	{
		CloseHandle(hthread[i]);
	}
	delete []m_pIocpThread;

	// 遍历列表，关闭所有的Socket
	if(!CloseHandle(m_hCompletionPort))
	{
		LOG_ERROR(DLL_LOGGER,_T("Close iocp handle failed!"));
		DWORD err=GetLastError();
		assert(0);
	}else
		LOG_INFO(DLL_LOGGER,_T("Close iocp succeed!"));

	OverlapSocket *pnext;
	for(i=0;i<SessionStyler_Max;i++)
	{
		OverlapSocket * pWhich = m_socketlist[i].NextData();
		while(pWhich)
		{
			assert(((pWhich->Id&0x80000000)>>31)==i);
			DWORD dwTime = (GetTickCount() - pWhich->info.dwStartTime)/1000;
			dwTime = dwTime?dwTime:1;
			pWhich->berror=true;
			pWhich->nreqpending=0;
			pnext=m_socketlist[i].NextData(pWhich); 
			FreeSocket(pWhich);
			pWhich=pnext;
		}
		if(m_lstSendBuffers[i].GetAllocated())
		{
			LOG_ERROR(DLL_LOGGER,
				_T("memory leakage !m_lstSendBuffers,SessionStyler")
				<<i
				<<_T(" ")
				<<m_lstSendBuffers[i].GetAllocated());
			assert(0);
		}

		if(m_RecvBuffers[i].GetAllocated())
		{
			LOG_ERROR(DLL_LOGGER,
				_T("memory leakage !m_RecvBuffers,SessionStyler")
				<<i
				<<_T(" ")
				<<m_RecvBuffers[i].GetAllocated());
			assert(0);
		}

		if(m_lstSendOverlaps[i].GetAllocated())
		{
			LOG_ERROR(DLL_LOGGER,
				_T("memory leakage !m_lstSendOverlaps,SessionStyler")
				<<i
				<<_T(" ")
				<<m_lstSendOverlaps[i].GetAllocated());
			assert(0);
		}

	}

	// 如果是Listen,关闭Listen Socket
	if(m_ListenSocket.IsValid())
	{
		LOG_INFO(DLL_LOGGER,_T("Destroy listening socket!"));
		m_ListenSocket.Destroy();
	}
}

void cIocpIO::SetPendTimeOut(UINT nSession,DWORD npendtimeout)
{
	OverlapSocket * pIocpSocket=m_socketlist[(nSession&0x80000000)>>31].Get(nSession&0x7fffffff);
	if(!pIocpSocket)
		return;
	pIocpSocket->npendtimeout=npendtimeout;
}

BOOL cIocpIO::PostAccept(OverlapSocket * pWhich)
{
	pWhich->cmd = IocpCmd_Accept;
	pWhich->bnewsession=false;			//未进入iocp线程不用同步
	ZeroMemory((OVERLAPPED *)pWhich,sizeof(OVERLAPPED));

	_InterlockedIncrement((LONG*)&pWhich->nreqpending);
	DWORD dwSize = 0;
	if(!AcceptEx(m_ListenSocket.GetHandle(), pWhich->socket.GetHandle(), 
		pWhich->pWriteStart, 0, sizeof( SOCKADDR_IN )+16, sizeof( SOCKADDR_IN )+16,
		&dwSize,(OVERLAPPED *)pWhich))
	{
		int nErr = WSAGetLastError();
		if( WSA_IO_PENDING != nErr)
		{
			_InterlockedDecrement((LONG*)&pWhich->nreqpending);
			assert(0);
			KillSocket(pWhich,_T("PostAccept failed!"));
			return FALSE;	//	todo:其他错误处理		
		}
	}
	return TRUE;
}

BOOL cIocpIO::PostConnect(OverlapSocket * pWhich,const TCHAR *lpszAddress)
{
	pWhich->cmd = IocpCmd_Connect;
	ZeroMemory((OVERLAPPED *)pWhich,sizeof(OVERLAPPED));

	struct	sockaddr_in ServAddr;
	INT len = sizeof(sockaddr_in);
	WSAStringToAddress(const_cast<TCHAR *>(lpszAddress),AF_INET,NULL,(LPSOCKADDR)&ServAddr,&len);


	DWORD dwBytes = 0;
	int nErr;
	LPFN_CONNECTEX lpfnConnectEx = NULL; // a pointer to the 'ConnectEx()' function
	GUID GuidConnectEx = WSAID_CONNECTEX; // The Guid

	nErr = WSAIoctl(pWhich->socket.GetHandle(),
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidConnectEx,
		sizeof(GuidConnectEx),
		&lpfnConnectEx,
		sizeof(lpfnConnectEx),
		&dwBytes,
		NULL,
		NULL); 
	if(nErr == SOCKET_ERROR)
	{
		LOG_ERROR(DLL_LOGGER,_T("can not get function entry poing!"));
		assert(0);
		return FALSE;
	}


	struct	sockaddr_in temp;
	temp.sin_family = AF_INET;
	temp.sin_port = htons(0);
	temp.sin_addr.s_addr = htonl(ADDR_ANY);
	SOCKET hSocket = pWhich->socket.GetHandle();
	if(!bind(hSocket, (struct sockaddr * )&temp, sizeof( struct sockaddr_in ))== 0 )
	{
		LOG_ERROR(DLL_LOGGER,_T("When connecting,can not bind to port 0!"));
		assert(0);
		return FALSE;
	}

	_InterlockedIncrement((LONG*)&pWhich->nreqpending);
	DWORD dwSize  = 0;
	if( !lpfnConnectEx( hSocket, (struct sockaddr * )&ServAddr, sizeof( struct sockaddr_in ),
		NULL,0,&dwSize,(OVERLAPPED *)pWhich) )
	{
		nErr = WSAGetLastError();
		if( WSA_IO_PENDING != nErr)
		{
			_InterlockedDecrement((LONG*)&pWhich->nreqpending);
			UINT nID = pWhich->Id; 
			LOG_ERROR(DLL_LOGGER,
				g_lpszWhich[m_ListenSocket.IsValid()]
				<<_T("PostConnect Error.ID = ")
				<<nID
				<<_T(",Error = ")
				<<nErr);
			assert(0);
			return FALSE;	//	todo:其他错误处理
		}
	}
	return TRUE;
}

char *cIocpIO::AllocRecvBuf(OverlapSocket *pWhich)
{
	char *pfree;
	if(pWhich->pAllocatedBase!=pWhich->pReadBuf)
	{
		pfree=pWhich->pAllocatedBase;
		pWhich->pAllocatedBase=*(char **)pWhich->pAllocatedBase;
	}else
		pfree=(char *)m_RecvBuffers[(pWhich->Id&0x80000000)>>31].New();
	return pfree;
}

void cIocpIO::FreeSendBuf(OverlapSocket *pWhich)
{
	//销毁发送缓存	
	unsigned st=(pWhich->Id&0x80000000)>>31;
	Overlap_Send *psendobj,*pnext;
	psendobj=pWhich->pSendHead;
	for(;psendobj;psendobj=pnext)
	{
		pnext=psendobj->pnext;
		if(psendobj->bsendcomplete)
		{
			DelSendObj(st,psendobj);
		}else
			break;
	}
}

BOOL cIocpIO::PostRecv(OverlapSocket * pWhich)
{
	unsigned st=(pWhich->Id&0x80000000)>>31;
	if(pWhich->pWriteStart==pWhich->pRecvBuf+m_pConfig[st].nRecvBufLen+sizeof(void *))
	{//一页满了
		if(*(char **)pWhich->pRecvBuf==NULL)
		{//下一页未分配
			*(char **)pWhich->pRecvBuf=AllocRecvBuf(pWhich);
			pWhich->pRecvBuf=*(char **)pWhich->pRecvBuf;
			*(char **)pWhich->pRecvBuf=NULL;
		}else{
			//下一页已分配
			pWhich->pRecvBuf=*(char **)pWhich->pRecvBuf;
		}
		_WriteBarrier();
		pWhich->pWriteStart=pWhich->pRecvBuf+sizeof(void *);//pWhich->pWriteStart被当成另一个线程的判决标志，所以必须放在最后
	}


	if(*(char **)pWhich->pRecvBuf==NULL)
	{//当前页未满，但是下一页未分配
		*(char **)(pWhich->pRecvBuf)=AllocRecvBuf(pWhich);
		**(char ***)(pWhich->pRecvBuf)=NULL;
	}
	assert(pWhich->pWriteStart>=pWhich->pRecvBuf+sizeof(void *)&&pWhich->pWriteStart<=pWhich->pRecvBuf+m_pConfig[st].nRecvBufLen+sizeof(void *));

	WSABUF wsabuf[2];
	char *pnext=*(char **)pWhich->pRecvBuf;
	assert(pnext);
	wsabuf[0].buf=pWhich->pWriteStart;
	wsabuf[0].len=m_pConfig[st].nRecvBufLen-(pWhich->pWriteStart-pWhich->pRecvBuf-sizeof(void *));
	wsabuf[1].buf=pnext+sizeof(void *);
	wsabuf[1].len=m_pConfig[st].nRecvBufLen;

	pWhich->cmd=IocpCmd_Recv;
	ZeroMemory((OVERLAPPED *)pWhich,sizeof(OVERLAPPED));


	DWORD dwSize = 0,dwFlag = 0;
	_InterlockedIncrement((LONG*)&pWhich->nreqpending);
	_InterlockedIncrement((LONG*)&pWhich->nrecving);
	assert(pWhich->socket.GetHandle()!=INVALID_SOCKET);
	if(WSARecv( pWhich->socket.GetHandle(), wsabuf,2,&dwSize, &dwFlag, pWhich, NULL ) == SOCKET_ERROR)
	{
		int nErr = WSAGetLastError();
		if( WSA_IO_PENDING != nErr)
		{
			_InterlockedDecrement((LONG*)&pWhich->nreqpending);
			_InterlockedDecrement((LONG*)&pWhich->nrecving);
			pWhich->berror=true;
			LOG_ERROR(DLL_LOGGER,
				_T("PostRecv Error.ID = ")<<pWhich->Id<<_T(",Error = ,")<<nErr);
			return FALSE;	//	todo:其他错误处理
		}
	}
	return TRUE;
}

void cIocpIO::OnAccept(OverlapSocket * pWhich)
{
	sockaddr *	p1, *p2;
	int	len1, len2;
	DWORD dwLen = MAX_IP_LEN;
	GetAcceptExSockaddrs( pWhich->pWriteStart, 0, sizeof( SOCKADDR_IN) + 16, sizeof( SOCKADDR_IN)+16, &p1, &len1, &p2, &len2 );
	if(len2 == 0)	
	{
		int nErrCode = WSAGetLastError();
		if(nErrCode != 0)
		{
			LPVOID lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS,    
				NULL, nErrCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
				(LPTSTR) &lpMsgBuf, 0, NULL);
			LocalFree(lpMsgBuf);
		}
	}

	int nRet = WSAAddressToString(p2,len2,NULL,pWhich->info.szIP,&dwLen);   
	if(nRet != 0)
	{
		int nErrCode = WSAGetLastError();
		if(nErrCode != 0)
		{
			LPVOID lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS,    
				NULL, nErrCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
				(LPTSTR) &lpMsgBuf, 0, NULL);
			LocalFree(lpMsgBuf);
		}
	}


	// 当前连接Post为接受对象 
	// todo:错误处理
	if(!PostRecv(pWhich)) // 从缓冲区头开始接 for iocp fast reaction
	{
		KillSocket(pWhich,_T("PostRecv failed in OnAccept"));
	}

	pWhich->socket.SetConnected();
	// 增加一个连接对象
	// todo:错误处理f
	pWhich->info.dwStartTime = GetTickCount(); 
	_InterlockedDecrement((LONG *)&pWhich->nreqpending);
	m_newcs[SessionStyler_Provider].Enter();
	m_newsocketset[SessionStyler_Provider].insert(pWhich->Id);
	m_newcs[SessionStyler_Provider].Leave();
}

int cIocpIO::OnRecvComplete(OverlapSocket * pWhich,DWORD dwbytes)
{
	unsigned st=(pWhich->Id&0x80000000)>>31;

#ifdef _DEBUG
	if(pWhich->debugflag&SOF_RECVDUMP)
	{
		DWORD ra;
		assert(pWhich->hdebugr!=INVALID_HANDLE_VALUE);
		DWORD firstpagelen=pWhich->pRecvBuf+sizeof(void *)+m_pConfig[st].nRecvBufLen-pWhich->pWriteStart;
		if(dwbytes>firstpagelen)
		{
			WriteFile(pWhich->hdebugr,pWhich->pWriteStart,(DWORD)firstpagelen,&ra,NULL);
			assert(dwbytes-firstpagelen>0);
			WriteFile(pWhich->hdebugr,*(char **)pWhich->pRecvBuf+sizeof(void *),dwbytes-firstpagelen,&ra,NULL);
		}else
		{
			WriteFile(pWhich->hdebugr,pWhich->pWriteStart,(DWORD)dwbytes,&ra,NULL);
		}
	}
#endif
	pWhich->info.dwRecvBytes+=dwbytes;
	
	DWORD size=pWhich->pWriteStart+dwbytes-pWhich->pRecvBuf-sizeof(void *);
	while(size>m_pConfig[st].nRecvBufLen)
	{
		size-=m_pConfig[st].nRecvBufLen;
		pWhich->pRecvBuf=*(char **)pWhich->pRecvBuf;
	}
	_WriteBarrier();
	pWhich->pWriteStart=pWhich->pRecvBuf+sizeof(void *)+size;
	assert(pWhich->pWriteStart>=pWhich->pRecvBuf+sizeof(void *)&&pWhich->pWriteStart<=pWhich->pRecvBuf+m_pConfig[st].nRecvBufLen+sizeof(void *));
	// 无论是否有完整包，都需要 Post 下一个接受 for iocp fast reaction
	if(!PostRecv(pWhich)) // 从缓冲区头开始接 for iocp fast reaction
	{
		KillSocket(pWhich,_T("PostRecv failed in OnRecvComplete"));
	}
	return TRUE;
}
void cIocpIO::KillSocket(OverlapSocket * pWhich,LPCTSTR psdes)
{
	assert(pWhich);
	unsigned st;
	st=(pWhich->Id&0x80000000)>>31;
	{
		pWhich->berror=true;//关闭iocp和逻辑线程对socket的进一步操作
		pWhich->tickcount=GetTickCount();
		
		m_killedcs[st].Enter();
		if(m_killedsocketset[st].find(pWhich->Id)==m_killedsocketset[st].end())
		{
			LOG_INFO(DLL_LOGGER,
				psdes
				<<_T(",放入killed socket队列,session=0x")
				<<_itot((INT_PTR)(pWhich->Id),szNum,16)
				<<_T(",param=0x")
				<<_itot((INT_PTR)(pWhich->param),szNum,16)
				<<_T(".悬挂操作数量")
				<<pWhich->nreqpending);
		}
		m_killedsocketset[st].insert(pWhich->Id);//放入此队列的socket会在iocp线程对此socket的操作终止后取出
		m_killedcs[st].Leave();
	}
}

unsigned cIocpIO::GetNewSession(SessionStyler st,LPVOID &param)
{
	UINT nID=INVALID_SESSION;
	OverlapSocket * pWhich;
	std::set<UINT>::iterator itnew;
	m_newcs[st].Enter();
	for(itnew=m_newsocketset[st].begin();itnew!=m_newsocketset[st].end();itnew++)
	{
		nID=*itnew;
		m_newsocketset[st].erase(itnew);
		pWhich=m_socketlist[st].Get(nID&0x7fffffff);
		param = pWhich->param;
		pWhich->bnewsession=true;
		break;
	}
	m_newcs[st].Leave();
	return nID;
}

void cIocpIO::ReUseSocket(OverlapSocket *pWhich)
{
	unsigned st=(pWhich->Id&0x80000000)>>31;
	int i;
	SendBuf		*psendbuf;
	unsigned nbytesend;
	for(i=0;i<NUM_PRIORITY_LEVEL;i++)
	{
		psendbuf=pWhich->pSendBufHeader[i];
		if(!psendbuf)
			continue;
		nbytesend=0;
		for(;psendbuf;psendbuf=psendbuf->psendbufnext)
		{
			nbytesend+=psendbuf->nsend;
		}
		m_nreqbuffered[st][i]-=nbytesend;
	}
	//销毁发送缓存
	FreeSendBuf(pWhich);


	pWhich->Reuse();

	if(!pWhich->socket.Create(IPPROTO_TCP))
	{
		assert(0);
		return;
	}

	if(!CreateIoCompletionPort((HANDLE)pWhich->socket.GetHandle(), m_hCompletionPort, 	pWhich->Id, 0 ))
	{
		assert(0);
		return;
	}

	PostAccept(pWhich);
}

unsigned cIocpIO::GetFailedSession(LPVOID &param)
{
	UINT nid=INVALID_SESSION;
	std::set<UINT>::iterator itskset;
	m_failedcs.Enter();
	for(itskset=m_failedconnect.begin();itskset!=m_failedconnect.end();itskset++)
	{
		nid=*itskset;
		assert(nid!=INVALID_SESSION);
		OverlapSocket *psocket;
		psocket=m_socketlist[SessionStyler_Consumer].Get(nid&0x7fffffff);
		assert(psocket);
		if(!psocket->nreqpending)
		{
			param=psocket->param;
			m_failedconnect.erase(itskset);
			m_socketlist[SessionStyler_Consumer].Del(psocket);
			break;
		}
		nid=INVALID_SESSION;
	}
	m_failedcs.Leave();
	return nid;
}

unsigned cIocpIO::GetDieSession(SessionStyler st,LPVOID &param)
{
	//this function must be called by logic thread
	OverlapSocket *psocket;
	UINT nid=INVALID_SESSION;
	std::set<UINT>::iterator itskset,itcur;
	m_killedcs[st].Enter();
	for(itskset=m_killedsocketset[st].begin();itskset!=m_killedsocketset[st].end();itskset++)
	{
		psocket=m_socketlist[st].Get(*itskset&0x7fffffff);
		assert(psocket);
		if(!psocket->bnewsession)
			continue;
		//LOG_INFO(DLL_LOGGER,_T("Enter critical section %p!",&psocket->cs);
		if(!psocket->nreqpending)
		{
			param=psocket->param;
			//销毁socket
			if(psocket->socket.IsValid())
			{
				psocket->socket.Destroy();
			}
			if(!st)	// 如果是客户端层的要重新初始为监听状态
			{
				ReUseSocket(psocket);
			}
			else
			{
				FreeSocket(psocket);
			}

			nid=*itskset;
			m_killedsocketset[st].erase(itskset);
			break;

		}else{

			DWORD tickcount=GetTickCount();
			if(tickcount-psocket->tickcount>=1000*6)
			{//包裹在internet最多停留时间
				if(psocket->socket.IsValid())
				{
					LOG_INFO(DLL_LOGGER,
						_T("die session 中的socket异步操作长时间未返回,强制关闭 socketid=0x")
						<<_itot((INT_PTR)(*itskset),szNum,16));
					psocket->socket.Destroy();
				}else
				{
					if(tickcount-psocket->tickcount>=1000*30)
					{
						TCHAR localaddress[32],remoteaddress[32];
						DWORD slen=32;
						psocket->socket.GetLocalAddress(localaddress,slen);
						psocket->socket.GetPeerAddress(remoteaddress,slen);
						LOG_ERROR(DLL_LOGGER,
							_T("已经destroy的socket session=0x")
							<<_itot((INT_PTR)(*itskset),szNum,16)
							<<_T("操作长时间未返回")
							<<psocket->nreqpending
							<<_T(" reqpending,local address ")
							<<localaddress
							<<_T(", peer address ")
							<<remoteaddress);
						assert(0);
					}
				}
			}
		}
	}
	m_killedcs[st].Leave();
	if(nid!=INVALID_SESSION)
	{
		if(nid==m_ndebugsession)
		{
			if(m_debugfilehandle!=INVALID_HANDLE_VALUE)
				CloseHandle(m_debugfilehandle);
			m_ndebugsession=INVALID_SESSION;
		}
	}
	return nid;
}

void cIocpIO::Read(unsigned long size,void *pbuf,OverlapSocket *pWhich)
{
	unsigned st=(pWhich->Id&0x80000000)>>31;
	char *pWriteStart;//=pWhich->pWriteStart;
	pWriteStart=pWhich->pWriteStart;
	UINT leftsize;
	for(;;)
	{
		//在同一页上，且数据足够
		if(pWriteStart<=pWhich->pReadBuf+sizeof(void *)+m_pConfig[st].nRecvBufLen&&pWriteStart>pWhich->pReadBuf)
		{
			leftsize=pWriteStart-pWhich->pReadStart;
			if(leftsize>=size)
			{
				memcpy(pbuf,pWhich->pReadStart,size);
				pWhich->pReadStart+=size;
			}else//之前检查过有充分的数据才来读的，怎么可能不够，哪里出错了
				vwassert(0);
			return;
		}

		leftsize=pWhich->pReadBuf+m_pConfig[st].nRecvBufLen+sizeof(void*)-pWhich->pReadStart;
		if(leftsize>=size)
		{//在不同页上，数据足够
			memcpy(pbuf,pWhich->pReadStart,size);
			pWhich->pReadStart+=size;
			size=0;
		}else
		{//在不同页上，数据不足
			if(leftsize)
			{
				memcpy(pbuf,pWhich->pReadStart,leftsize);
				pWhich->pReadStart+=leftsize;
				*(char **)&pbuf+=leftsize;
				size-=leftsize;
			}
		}
		if(pWhich->pReadStart==pWhich->pReadBuf+m_pConfig[st].nRecvBufLen+sizeof(void*))
		{//读完一页释放内存
			_WriteBarrier();
			pWhich->pReadBuf=*(char **)pWhich->pReadBuf;
			pWhich->pReadStart=pWhich->pReadBuf+sizeof(void *);
		}
		if(size==0)
			return;
	}
}


size_t cIocpIO::Read(unsigned int nSession,cReader &reader)
{
	reader.clear();
	assert(nSession!=INVALID_SESSION);
	if(nSession==INVALID_SESSION)
		return 0;
	unsigned st=(nSession&0x80000000)>>31;
	OverlapSocket * pWhich = m_socketlist[st].Get(nSession&0x7fffffff);
	assert(pWhich);
	if(!pWhich)
		return 0;

	assert(pWhich->bnewsession);//可能被killsocket删除，但还没被GetDieSession回收
	if(!pWhich->bnewsession)
	{
		LOG_ERROR(DLL_LOGGER,
			_T("read verifying pWhich->bnewsession error!nSession=0x")
			<<_itot((INT_PTR)nSession,szNum,16));
		assert(0);
		return 0;
	}

	//有没有一个完整的包裹
	//m_nPacketHeadSize[st] + m_nPacketLengthSize[st]
	TCHAR buf[0x100];
	char	*pnext;
	unsigned nLen;
	//避免线程同步和线程错误
	//读取剩余字节数
	assert((((DWORD_PTR)&pWhich->pWriteStart)&3)==0);//确认对齐，确保原子操作

	char *pWriteStart;
	pWriteStart=pWhich->pWriteStart;
	if(pWriteStart<=pWhich->pReadBuf+sizeof(void *)+m_pConfig[st].nRecvBufLen&&pWriteStart>pWhich->pReadBuf)
	{//写指针与读指针在同一个页上
		nLen=pWriteStart-pWhich->pReadStart;
	}else
	{
		//写指针与读指针不在同一个页上
		nLen=pWhich->pReadBuf+sizeof(void *)+m_pConfig[st].nRecvBufLen-pWhich->pReadStart;

		for(pnext=*(char **)pWhich->pReadBuf;
			(pWriteStart<pnext||(pWriteStart>pnext+sizeof(void *)+m_pConfig[st].nRecvBufLen));
			pnext=*(char **)pnext)
		{
			nLen+=m_pConfig[st].nRecvBufLen;
		}
		nLen+=pWriteStart-pnext-sizeof(char *);
	}	
	if(!nLen)
		return 0;


	unsigned packetsize=0;
	if(!pWhich->currentpacketlength)
	{//读取包头
		// 必须够一个包头的长度
		if(nLen<=GetPacketDataOffset(st))
			return 0;
		

		//如果有包头验证包头
		//DWORD packetbeg/*,packetend*/;
		if(m_pConfig[st].bPacketFlags)
		{
			Read(m_nPacketHeadSize[st],buf,pWhich);
			if(*(DWORD *)buf!=m_pConfig[st].dwPacketBegin)
			{
				LOG_ERROR(DLL_LOGGER,_T("data header error!"));
				assert(0);
				KillSocket(pWhich,_T("data header mark error"));
				return 0;
			}
			_InterlockedIncrement((LONG *)&pWhich->nRecvSerial);
			if(*(DWORD *)(buf+4)!=pWhich->nRecvSerial)
			{
				LOG_ERROR(DLL_LOGGER,_T("data serial error!"));
				assert(0);
				KillSocket(pWhich,_T("data serial error"));
				return 0;
			}
		}
		//读取和校验长度
		Read(m_nPacketLengthSize[st],&packetsize,pWhich);
		if(packetsize>reader.GetCapacity())
		{
			DWORD addresslen=sizeof(buf);
			pWhich->socket.GetPeerAddress(buf,addresslen);
			LOG_ERROR(DLL_LOGGER,
				_T("IOCP::packet length ")
				<<packetsize
				<<_T(",excceed buffer ")
				<<reader.GetCapacity()
				<<_T(",kick connection ip= ")
				<<buf);
			assert(0);
			KillSocket(pWhich,_T("length error"));
			return 0;
		}
		pWhich->currentpacketlength=packetsize;
	}else
		packetsize=pWhich->currentpacketlength;

	//必须至少一个完整包
	//读取数据
	if(nLen<GetPacketStructSize(st)+packetsize)
		return 0;
	Read(packetsize,reader.Buffer(),pWhich);

	// 尾标记校验
	if(m_pConfig[st].bPacketFlags)
	{
		//读取包尾
		Read(m_nPacketStructSize[st]-GetPacketDataOffset(st),buf,pWhich);

		PacketEnd * pEnd = (PacketEnd *)(buf);
		assert(pEnd->dwEnd == m_pConfig[st].dwPacketEnd);
		if(pEnd->dwEnd != m_pConfig[st].dwPacketEnd)
		{
			KillSocket(pWhich,_T("data end error"));
			return 0;
		}
	}

	reader.SetLength(packetsize);

	pWhich->currentpacketlength=0;
	return packetsize;
}

void cIocpIO::CopyToSendBuf(const void * ptos,DWORD nsendlen,unsigned ipriority,OverlapSocket * pWhich)
{
	unsigned st=(pWhich->Id&0x80000000)>>31;
	char *ptosend=(char *)ptos;
	char *pdstbuf;
	SendBuf *psendbuf;
	assert(pWhich->pSendBuf[ipriority]);
	m_nreqbuffered[st][ipriority]+=nsendlen;
	psendbuf=pWhich->pSendBuf[ipriority];
	while(1)
	{
		DWORD left;
		pdstbuf=(char *)(psendbuf+1)+psendbuf->nsend;
		left=m_pConfig[st].nSendBufLen-psendbuf->nsend;
		if(left<nsendlen)
		{//如果目标缓存不够大
			//拷贝剩余数据
			assert(psendbuf->nsend<=m_pConfig[st].nSendBufLen);
			if(left)
			{
				memcpy(pdstbuf,ptosend,left);
				ptosend+=left;
				nsendlen-=left;
				psendbuf->nsend=m_pConfig[st].nSendBufLen;
			}
			assert(psendbuf->nsend<=m_pConfig[st].nSendBufLen);

			psendbuf=(SendBuf *)m_lstSendBuffers[st].New();
			if(!psendbuf)
			{
				LOG_ERROR(DLL_LOGGER,_T("can not allocate buffer"));
				assert(0);
				return;
			}
			psendbuf->nsend=0;
			psendbuf->psendbufnext=NULL;

			pWhich->pSendBuf[ipriority]->psendbufnext=psendbuf;
			pWhich->pSendBuf[ipriority]=psendbuf;
			pWhich->nSendBuf[ipriority]++;


		}else
		{
			memcpy(pdstbuf,ptosend,nsendlen);
			psendbuf->nsend+=nsendlen;
			assert(psendbuf->nsend<=m_pConfig[st].nSendBufLen);
			break;
		}
	}
}

void cIocpIO::DelSendObj(unsigned st,Overlap_Send * psendobj)
{
	OverlapSocket *pWhich;
	pWhich=psendobj->pIocpSocket;
	EnterCriticalSection(&pWhich->cs);
	if(psendobj->pprev)
		psendobj->pprev->pnext=psendobj->pnext;
	else pWhich->pSendHead=psendobj->pnext;

	if(psendobj->pnext)
		psendobj->pnext->pprev=psendobj->pprev;
	else pWhich->pSendTail=psendobj->pprev;
	LeaveCriticalSection(&pWhich->cs);


	SendBuf *psendbuf,*pnext;
	psendbuf=psendobj->psendbuf;
	long bytecount=0;
	while(psendbuf)
	{
		pnext=psendbuf->psendbufnext;
		bytecount-=psendbuf->nsend;
		m_lstSendBuffers[st].Del(psendbuf);
		psendbuf=pnext;
	}
	m_lstSendOverlaps[st].Del((Overlap_Send *)psendobj);
}

size_t cIocpIO::Write(unsigned int nSession,const char * buffer, size_t nSize,unsigned ipriority,bool bdiscardbale)
{
	// 该函数是线程不安全的，多线程的情况下，会造成包的乱序问题

	//参数校验
	assert(nSession!=INVALID_SESSION);
	if(nSession==INVALID_SESSION)
		return 0;


	if( buffer == NULL || nSize == 0)
	{
		assert(0);
		return 0;
	}

	unsigned st=(nSession&0x80000000)>>31;
	assert(nSize<(1u<<m_nPacketLengthSize[st]*8));

	OverlapSocket * pWhich = m_socketlist[st].Get(nSession&0x7fffffff);
	if(!pWhich)
	{
		assert(0);
		return 0;
	}
	
	FreeSendBuf(pWhich);

	if(pWhich->berror)
	{
		return 0;
	}

	if(!pWhich->bnewsession)
	{
		LOG_ERROR(DLL_LOGGER,
			_T("write verify pWhich->bnewsession error!nSession=0x")<<_itot((INT_PTR)nSession,szNum,16));
		assert(0);
		return 0;
	}

	UINT contentsize=nSize;

	u_long nSendSize;
	nSendSize=(u_long)nSize+m_nPacketStructSize[st];


	CSLocker csl(m_csWriteLock.GetCS());
	if(ipriority>=NUM_PRIORITY_LEVEL)
	{
		LOG_ERROR(DLL_LOGGER,_T("error parameter ipriority,please verify ipriority<NUM_PRIORITY_LEVEL!"));
		assert(0);
		return 0;
	}

	assert(ipriority<NUM_PRIORITY_LEVEL);

	//如果积累的包裹过多则直接放弃
	if(bdiscardbale)
	{
		if(m_lstSendBuffers[st].GetAllocated()>=m_pConfig[st].nDiscardThreshold)
			return 0;
	}

	//分配缓存
	SendBuf *psendbuf;
	if(!pWhich->pSendBuf[ipriority])
	{
		psendbuf=(SendBuf *)m_lstSendBuffers[st].New();
		if(!psendbuf)
		{
			LOG_ERROR(DLL_LOGGER,_T("can not allocate buffer"));
			assert(0);
			return 0;
		}
		psendbuf->nsend=0;
		psendbuf->psendbufnext=NULL;

		pWhich->pSendBuf[ipriority]=psendbuf;
		pWhich->pSendBuf[ipriority]->nsend=0;
		pWhich->nSendBuf[ipriority]++;
		pWhich->pSendBufHeader[ipriority]=psendbuf;
	}

	char packetstruct[32];
	char *pbuf=packetstruct;
	//填写包头
	if(m_pConfig[st].bPacketFlags)
	{
		_InterlockedIncrement((LONG *)&pWhich->nSendSerial);  //[zhouqq], 这行原来代码: pWhich->nSendSerial++;
		((PacketHeader *)pbuf)->dwBegin = m_pConfig[st].dwPacketBegin;
		((PacketHeader *)pbuf)->nSerial = pWhich->nSendSerial;
		PutPacketSize((SessionStyler)st,pbuf,nSize);// 填写包长度
		CopyToSendBuf(pbuf,	GetPacketDataOffset(st),ipriority,pWhich);
		pbuf+=GetPacketDataOffset(st);
	}else{
		CopyToSendBuf(&nSize,m_nPacketLengthSize[st],ipriority,pWhich);
	}

	//填写发送数据
	CopyToSendBuf(buffer,nSize,ipriority,pWhich);

	//填写包尾
	if(m_pConfig[st].bPacketFlags)
	{
		// 填写包尾
		((PacketEnd *)(pbuf))->dwEnd = m_pConfig[st].dwPacketEnd;
		CopyToSendBuf(pbuf,sizeof(PacketEnd),ipriority,pWhich);
	}

	return nSize;
}

size_t cIocpIO::Write(unsigned int nSession,cWriter &writer,unsigned ipriority,bool bdiscardbale)
{
	return Write(nSession,writer.Buffer(),writer.Length(),ipriority,bdiscardbale);
}

void cIocpIO::SendSyn(unsigned int nSession,char * buffer, size_t nSize)
{
	unsigned st=(nSession&0x80000000)>>31;
	OverlapSocket * pWhich = m_socketlist[st].Get(nSession&0x7fffffff);
	if(!pWhich)
	{
		return;
	}

	assert(pWhich->bnewsession);
	assert(!pWhich->berror);
	if(!pWhich->berror)
	{
		SOCKET hSocket;
		hSocket=pWhich->socket.GetHandle();
		if(hSocket==INVALID_SOCKET)
		{
			assert(0);
			assert(pWhich->berror&&pWhich->nreqpending);
			return;
		}

		char *pBuf = &m_szBuffer[0];
		pWhich->nSendSerial ++;
		// 填写包头
		if(m_pConfig[st].bPacketFlags)
		{
			((PacketHeader *)pBuf)->dwBegin = m_pConfig[st].dwPacketBegin;
			((PacketHeader *)pBuf)->nSerial = pWhich->nSendSerial;
		}

		// 填写包长度
		PutPacketSize((SessionStyler)st,pBuf,nSize);
		assert(nSize <= eBufferSize-m_nPacketStructSize[st]);
		
		pBuf = pBuf + m_nPacketHeadSize[st] + m_nPacketLengthSize[st];
		memcpy(pBuf,buffer,nSize);

		// 填写包尾
		if(m_pConfig[st].bPacketFlags)
			((PacketEnd *)(pBuf+nSize))->dwEnd = m_pConfig[st].dwPacketEnd;
		assert(nSize+m_nPacketStructSize[st] < eBufferSize);
		int nSendLen = nSize+m_nPacketStructSize[st];
		int nLeft = nSendLen;
		int nIdx = 0;
		int nRet = 0;
		while (nLeft > 0)
		{
			nRet = send(hSocket,&m_szBuffer[nIdx],nLeft,0);
			if(nRet == SOCKET_ERROR)
			{
				KillSocket(pWhich,_T("send error!"));
			}
			nLeft -= nRet;
			nIdx += nRet;
		}
		_InterlockedExchangeAdd((long *)&pWhich->nbytesend,nRet);
		_InterlockedIncrement((long *)&m_Info[st].m_dwSendTimes);
		_InterlockedExchangeAdd( (LONG*)&m_Info[st].m_dwSendBytes, (LONG)nRet);
		_InterlockedExchangeAdd( (LONG*)&pWhich->info.dwSendBytes[0], (LONG)nRet);	
	}
}

BOOL cIocpIO::PostSend(OverlapSocket * pWhich,unsigned curlevel,unsigned &maxsend)
{
	CSLocker csl(m_csWriteLock.GetCS());

	if(pWhich->berror)
	{
		return FALSE;
	}
	
	UINT i;
	for(i=0;i<curlevel;i++)
	{
		if(pWhich->pSendBufHeader[i])
			break;
	}
	if(i==curlevel)
	{
		if(curlevel==NUM_PRIORITY_LEVEL||!maxsend)
			return TRUE;
		else{
			if(!pWhich->pSendBufHeader[i])
				return TRUE;
		}
	}
	SOCKET hSocket;
	hSocket=pWhich->socket.GetHandle();
	if(hSocket==INVALID_SOCKET)
	{
		assert(0);
		return FALSE;
	}

 
	// 初始化发送对象

	// 从池中分配对象
	Overlap_Send * pSendObj = NULL;
	unsigned st=(pWhich->Id&0x80000000)>>31;
	pSendObj = m_lstSendOverlaps[st].New();
	if(!pSendObj)
	{
		assert(0);
		return 0;
	}


	ZeroMemory((OVERLAPPED *)pSendObj,sizeof(OVERLAPPED));
	pSendObj->cmd=IocpCmd_Send;
	pSendObj->pIocpSocket=pWhich;
	pSendObj->bsendcomplete=false;


	_InterlockedIncrement((LONG*)&pWhich->nreqpending);

	pSendObj->tickcount=GetTickCount();

	//设置socket的发送列表，以便检查超时
	Overlap_Send * psend;
	EnterCriticalSection(&pWhich->cs);
	psend=pWhich->pSendTail;
	if(psend)
		psend->pnext=pSendObj;

	pSendObj->pprev=psend;
	pSendObj->pnext=NULL;

	pWhich->pSendTail=pSendObj;
	if(!pWhich->pSendHead)
		pWhich->pSendHead=pSendObj;
	LeaveCriticalSection(&pWhich->cs);

	_InterlockedIncrement((long *)&m_Info[st].m_dwSendTimes);

	//检查是否有包裹标记

	DWORD dwSendSize = 0;
	//找到发送头部指针
	UINT nsendbuf=0;
	pSendObj->psendbuf=NULL;
	for(i=0;i<curlevel;i++)
	{
		if(pWhich->pSendBufHeader[i])
		{
			pSendObj->psendbuf=pWhich->pSendBufHeader[i];
			break;
		}
	}
	//计算发送数据块数
	for(i=0;i<curlevel;i++)
	{
		nsendbuf+=pWhich->nSendBuf[i];
		pWhich->pSendBuf[i]=NULL;
	}
	if(curlevel<NUM_PRIORITY_LEVEL)
	{
		if(!pSendObj->psendbuf)
			pSendObj->psendbuf=pWhich->pSendBufHeader[i];

		SendBuf *psendbuf;
		psendbuf=pWhich->pSendBufHeader[i];
		for(;psendbuf;psendbuf=psendbuf->psendbufnext)
		{
			nsendbuf++;
			if(maxsend>psendbuf->nsend)
			{
				maxsend-=psendbuf->nsend;
			}else{
				maxsend=0;
				break;
			}
		}
	}
	assert(pSendObj->psendbuf);

	WSABUF *pbuf=(WSABUF *)_alloca(sizeof(WSABUF)*nsendbuf);
	SendBuf *psendbuf,*psendprev=NULL;
	UINT ncurbuf=0;
	long nbytesend=0;
	//可完全发送的级别
	for(i=0;i<curlevel;i++)
	{
		psendbuf=pWhich->pSendBufHeader[i];
		if(!psendbuf)
			continue;
		for(;psendbuf;psendbuf=psendbuf->psendbufnext,ncurbuf++)
		{
			if(psendprev)
			{
				psendprev->psendbufnext=psendbuf;
			}
			pbuf[ncurbuf].buf=(char *)(psendbuf+1);
			pbuf[ncurbuf].len=psendbuf->nsend;

			psendprev=psendbuf;
			nbytesend+=psendbuf->nsend;
			m_nreqbuffered[st][i]-=psendbuf->nsend;
			assert((int)m_nreqbuffered[st][i]>=0);
		}
		pWhich->pSendBufHeader[i]=pWhich->pSendBuf[i]=NULL;
		pWhich->nSendBuf[i]=0;
	}
	//最后一个级别
	if(i<NUM_PRIORITY_LEVEL)
	{
		UINT nlastleftbuf=nsendbuf-ncurbuf;
		for(psendbuf=pWhich->pSendBufHeader[i];ncurbuf<nsendbuf;psendbuf=psendbuf->psendbufnext,ncurbuf++)
		{
			if(psendprev)
			{
				psendprev->psendbufnext=psendbuf;
			}
			pbuf[ncurbuf].buf=(char *)(psendbuf+1);
			pbuf[ncurbuf].len=psendbuf->nsend;
			psendprev=psendbuf;
			nbytesend+=psendbuf->nsend;
			m_nreqbuffered[st][i]-=psendbuf->nsend;
			assert((int)m_nreqbuffered[st][i]>=0);
		}
		psendprev->psendbufnext=NULL;
		if(!psendbuf)
			pWhich->pSendBuf[i]=NULL;
		pWhich->pSendBufHeader[i]=psendbuf;
		pWhich->nSendBuf[i]-=nlastleftbuf;
	}
	_InterlockedExchangeAdd((long *)(m_bytepending+st),nbytesend);
	_InterlockedExchangeAdd((long *)&pWhich->nbytepending,nbytesend);
	pSendObj->dwSend=nbytesend;

	if(pWhich->Id==m_ndebugsession)
	{
		DWORD ra;
		WriteFile(m_debugfilehandle,&pSendObj->tickcount,sizeof(pSendObj->tickcount),&ra,NULL);
		WriteFile(m_debugfilehandle,&nbytesend,sizeof(nbytesend),&ra,NULL);
		for(i=0;i<ncurbuf;i++)
		{
			WriteFile(m_debugfilehandle,pbuf[i].buf,pbuf[i].len,&ra,NULL);
		}
	}


	if( WSASend(hSocket,pbuf,ncurbuf,
		&dwSendSize, 0, (OVERLAPPED*)pSendObj, NULL ) == SOCKET_ERROR )
	{
		int nErr = WSAGetLastError();
		if( WSA_IO_PENDING != nErr)
		{
			_InterlockedExchangeAdd((long *)&pWhich->nbytepending,-nbytesend);
			_InterlockedExchangeAdd((long *)(m_bytepending+st),-nbytesend);
			UINT nID = pWhich->Id;
			TCHAR buf[0x100];
			_stprintf_s(buf,0x100,_T("WSASend return error in PostSend %d"),nErr);
			KillSocket(pWhich,buf);
			_InterlockedDecrement((LONG*)&pWhich->nreqpending);

			// 先释放发送对象的临时缓存
			DelSendObj(st,pSendObj);
			return 0;
		}
	}

	pWhich->nSendObjects ++;
	return TRUE;	
}

void cIocpIO::GetLevelLeft(unsigned st,unsigned &ilevelsend,unsigned &lastlevelleft)
{
	unsigned total;
	total=m_bytepending[st];

	for(ilevelsend=0;ilevelsend<NUM_PRIORITY_LEVEL;ilevelsend++)
	{
		if(total+m_nreqbuffered[st][ilevelsend]>m_pConfig[st].nPendLimit)//太小，不能充分利用windows发送能力，cpu多出额外的处理，太大容易断线.
		{
			//估算:<分配给当前程序的网卡发送速度*下次调用距离本次调用的时间/缓冲大小,不能充分利用windows发送能力,cpu多出额外的处理
			break;
		}
		total+=m_nreqbuffered[st][ilevelsend];
	}
	lastlevelleft=m_pConfig[st].nPendLimit-total;
	if((int)lastlevelleft<0)
		lastlevelleft=0;
}

void cIocpIO::SendAll()
{
	DWORD max;

	unsigned i;
	int ret = 0;
	DWORD tickcount;
	tickcount=GetTickCount();
	for(i=0;i<2;i++)
	{

		max=m_Info[i].m_dwSendTimes-m_writetimes[i];
		if(max>m_Info[i].maxFrameSendTimes)
			m_Info[i].maxFrameSendTimes=max;
		m_writetimes[i]=m_Info[i].m_dwSendTimes;


		max=m_Info[i].m_dwSendBytes-m_writebytes[i];
		if(max>m_Info[i].maxFrameSendBytes)
			m_Info[i].maxFrameSendBytes=max;
		m_writebytes[i]=m_Info[i].m_dwSendBytes;

		unsigned ilevelsend,lastlevelleft;
		GetLevelLeft(i,ilevelsend,lastlevelleft);



		DWORD tickcount=GetTickCount();
		OverlapSocket * pWhich = m_socketlist[i].NextData();
		for(;pWhich;pWhich = m_socketlist[i].NextData(pWhich))
		{
			if(!pWhich->berror&&pWhich->bnewsession)
			{
				FreeSendBuf(pWhich);
				//检查悬挂的发送
				Overlap_Send *psendobj,*pnext;
				psendobj=pWhich->pSendHead;
				for(;psendobj;psendobj=pnext)
				{
					pnext=psendobj->pnext;
					if(!psendobj->bsendcomplete)
					{
						if(pWhich->npendtimeout!=-1)
						{
							if(tickcount-psendobj->tickcount>pWhich->npendtimeout)
							{
								TCHAR buf[0x100];
								if(m_pConfig[i].bPacketFlags)
									_stprintf_s(buf,200,_T("send time out,current system time %d first pended serial number %d send time %d"),tickcount,*((DWORD *)(psendobj->psendbuf+1)+1),psendobj->tickcount);
								else
									_stprintf_s(buf,200,_T("send time out,current system time %d first pended packet send time%d"),tickcount,psendobj->tickcount);
								KillSocket(pWhich,buf);
								pWhich->socket.Destroy();
							}
						}
						break;
					}else
					{
						DelSendObj(i,psendobj);
					}
				}
				if(pWhich->berror)
					continue;
				PostSend(pWhich,ilevelsend,lastlevelleft);

			}
		}
	}
}

const TCHAR * cIocpIO::GetConnInfo(unsigned int nSession)
{
	int styler=GetStyler(nSession);
	OverlapSocket * pWhich = m_socketlist[styler].Get(nSession&0x7fffffff);
	if(!pWhich)
		return NULL;
	return pWhich->info.szIP; 
}

void cIocpIO::SetParam(unsigned nsession,LPVOID lparam)
{
	OverlapSocket * pWhich = m_socketlist[(nsession&0x80000000)>>31].Get(nsession&0x7fffffff);
	if(pWhich)
		pWhich->param=lparam; 
}

LPVOID cIocpIO::GetParam(unsigned nsession)
{
	return (LPVOID)m_socketlist[(nsession&0x80000000)>>31].Get(nsession&0x7fffffff)->param; 
}


OverlapSocket * cIocpIO::AllocIocpObject(SessionStyler st,LPVOID param)
{
	OverlapSocket *pobject;

	pobject=m_socketlist[st].New();
	if(pobject==NULL)
	{
		assert(0);
		return NULL;
	}
	if(!pobject->socket.Create(IPPROTO_TCP/*m_pConfig[st].nProtocol*/))
	{
		assert(0);
		return NULL;
	}

	unsigned id;
	id=m_socketlist[st].GetId(pobject);
	pobject->Id = (id|(st<<31));
	pobject->param =param;

	if(!CreateIoCompletionPort((HANDLE)pobject->socket.GetHandle(), m_hCompletionPort, 	pobject->Id, 0 ))
	{
		assert(0);
		return NULL;
	}


	return pobject;
}

void cIocpIO::FreeSocket(OverlapSocket *pWhich)
{
	assert(pWhich);
	if(!pWhich)
		return;
	assert(!pWhich->nreqpending);
	assert(pWhich->berror);

	unsigned st=(pWhich->Id&0x80000000)>>31;
	//销毁接收缓存
	char *pnext,*pcur;
	for(pcur=pWhich->pAllocatedBase;pcur;pcur=pnext)
	{
		pnext=*(char **)pcur;
		m_RecvBuffers[st].Del(pcur);
	}


	UINT nbytesend;
	int i;
	for(i=0;i<NUM_PRIORITY_LEVEL;i++)
	{
		nbytesend=0;
		SendBuf *psndbuf,*psendbufnext;
		for(psndbuf=pWhich->pSendBufHeader[i];psndbuf;psndbuf=psendbufnext)
		{
			psendbufnext=psndbuf->psendbufnext;
			nbytesend+=psndbuf->nsend;
			m_lstSendBuffers[st].Del(psndbuf);
		}
		m_nreqbuffered[st][i]-=nbytesend;
	}

	//销毁发送缓存
	FreeSendBuf(pWhich);

	if(pWhich->socket.IsValid())
		pWhich->socket.Destroy();

	if(pWhich->pAllocatedBase)
		DeleteCriticalSection(&pWhich->cs);
	m_socketlist[st].Del(pWhich);
}

void cIocpIO::Statistics(SessionStyler st,SocketIOStatistics * info,DWORD flag)
{
	m_Info[st].m_ncursendpacket = m_lstSendOverlaps[st].GetAllocated();
	*info = m_Info[st];
	info->niopend=m_bytepending[st];
	int i;
	info->niobuffered=0;
	info->nmaxrecv=m_RecvBuffers[st].GetMaxAllocated();
	info->nmaxsend=m_lstSendBuffers[st].GetMaxAllocated();
	for(i=0;i<NUM_PRIORITY_LEVEL;i++)
	{
		info->niobuffered+=m_nreqbuffered[st][i];
	}
	
	if(STATSTICS_CLEAR&flag)
	{
		for(unsigned i=0;i<SessionStyler_Max;i++)
		{
			m_Info[i].maxFrameSendBytes=0;
			m_Info[i].maxFrameSendTimes=0;
		}
	}
}

void cIocpIO::SetSendTimerStampSignature(unsigned nSession,bool benable,DWORD idchar,DWORD timerstamp)
{
	unsigned st;
	st=(nSession&0x80000000)>>31;
	OverlapSocket * pWhich;
	pWhich=m_socketlist[st].Get(nSession&0x7fffffff);
	assert(pWhich);
	if(!pWhich)
		return;
	pWhich->ioffset[0]=idchar;
	pWhich->btimerstamp[0]=benable;
	pWhich->timersignature[0]=timerstamp;
}

void cIocpIO::SetRecvTimerStampSignature(unsigned nSession,bool benable,DWORD idchar,DWORD timerstamp)
{
	unsigned st;
	st=(nSession&0x80000000)>>31;
	OverlapSocket * pWhich;
	pWhich=m_socketlist[st].Get(nSession&0x7fffffff);
	assert(pWhich);
	if(!pWhich)
		return;
	pWhich->ioffset[1]=idchar;
	pWhich->btimerstamp[1]=benable;
	pWhich->timersignature[1]=timerstamp;
}

unsigned cIocpIO::GetPendedDataSize(unsigned session)
{
	unsigned st;
	st=(session&0x80000000)>>31;
	OverlapSocket * pWhich;
	pWhich=m_socketlist[st].Get(session&0x7fffffff);
	assert(pWhich);
	if(!pWhich)
		return 0;

	Overlap_Send *psendobj;
	SendBuf	*psendbuf;
	unsigned total=0;
	for(psendobj=pWhich->pSendHead;psendobj;psendobj=psendobj->pnext)
	{
		psendbuf=psendobj->psendbuf;
		while(psendbuf)
		{
			total+=psendbuf->nsend;
			psendbuf=psendbuf->psendbufnext;
		}
	}
	return total;
}

void cIocpIO::SetDebugSession(unsigned session)
{
	if(m_ndebugsession!=INVALID_SESSION)
	{
		CloseHandle(m_debugfilehandle);
	}
	m_ndebugsession=session;
	if(session!=INVALID_SESSION&&m_socketlist[session>>31].Get(session&0x7fffffff))
	{
		m_debugfilehandle=CreateFile(_T("iocpdebug.bin"),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	}
}
