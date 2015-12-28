#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include "netlib.h"
#include "socket.h"
#include "support.h"
#include "ModuleSupport.h"

#define OVERLOAD
#include "overload.h"

cSocket::cSocket()
{
	m_hSocket = INVALID_SOCKET;
	m_bConnected = FALSE;
}
cSocket::~cSocket()
{
	if(m_hSocket != INVALID_SOCKET)
		Destroy();
}

BOOL cSocket::Create(int nProtocol)
{
	assert(m_hSocket == INVALID_SOCKET);
	m_hSocket = socket(AF_INET, SOCK_STREAM, nProtocol);

	int ret;
	int nSendBuffer = 0;
	int nRetLen = sizeof(nSendBuffer);
	ret = getsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*)&nSendBuffer,&nRetLen);

	nSendBuffer=0;
	ret = setsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*)&nSendBuffer,sizeof(nSendBuffer));
	assert(!ret);

	ret = getsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*)&nSendBuffer,&nRetLen);
	assert(!ret);

	//close nagle algorithm to speed up
	int parameter=1;
	ret = setsockopt(m_hSocket,IPPROTO_TCP,TCP_NODELAY,(char*)&parameter,sizeof(parameter));
	assert(!ret);

	int optlen;
	optlen=sizeof(parameter);
	ret=getsockopt(m_hSocket,IPPROTO_TCP,TCP_NODELAY,(char *)&parameter,&optlen);
	assert(!ret);

	return (m_hSocket != INVALID_SOCKET);
}

void cSocket::Destroy()
{
	assert(m_hSocket!=INVALID_SOCKET);

	m_bConnected = FALSE;
	int nRet = shutdown(m_hSocket,SD_BOTH);
	int r;
	r=closesocket(m_hSocket); 
	if(r==SOCKET_ERROR)
	{
		r=WSAGetLastError();
		
		TCHAR szNum[64]={0};
		LOG_ERROR(DLL_LOGGER,
			_T("close socket 0x")
			<<_itot((INT_PTR)m_hSocket,szNum,16)
			<<_T("error code=")
			<<r);
	}
	else
	{//warning do NOT remove,do NOT remove
		//_printf("close socket ok\n");
	}
	m_hSocket = INVALID_SOCKET;
}

BOOL cSocket::Listen(IpAddrType eType,const TCHAR * lpszAddress,unsigned nbacklog)
{
	if(m_hSocket == INVALID_SOCKET)
		return FALSE;

	struct	sockaddr_in serveraddr;
	INT len = sizeof(sockaddr_in);
	WSAStringToAddress(const_cast<TCHAR *>(lpszAddress),AF_INET,NULL,(LPSOCKADDR)&serveraddr,&len);
	if(eType == IpAddr_Any)
		serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );
	else if(eType == IpAddr_Local)
		serveraddr.sin_addr.s_addr = htonl( INADDR_LOOPBACK );

	int err;
	if(bind(m_hSocket,(struct sockaddr * )&serveraddr,sizeof( struct sockaddr_in ))==0)
	{
		if( listen( m_hSocket,nbacklog) == 0 )
		{
			return TRUE;
		}else
		{
			err=WSAGetLastError();
			LOG_ERROR(DLL_LOGGER,
				_T("listen failed,WSAGetLastError = ")<<err);
		}
	}else
	{
		err=WSAGetLastError();
		LOG_ERROR(DLL_LOGGER,
			_T("bind to ")
			<<lpszAddress
			<<_T(" failed,WSAGetLastError = ")
			<<err);
	}
	return FALSE;
}

bool cSocket::AsyncConnect(const TCHAR * lpszAddress,HANDLE hevent)
{
	if(m_hSocket == INVALID_SOCKET)
		return true;
	WSAEventSelect(m_hSocket,hevent,FD_CONNECT);
	assert(hevent!=0);
	struct	sockaddr_in ServAddr;
	INT len = sizeof(sockaddr_in);
	WSAStringToAddress(const_cast<TCHAR *>(lpszAddress),AF_INET,NULL,(LPSOCKADDR)&ServAddr,&len);

	int nErr;
	if( SOCKET_ERROR == connect(m_hSocket, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) )
	{
		nErr = WSAGetLastError();
		return (nErr!=WSAEWOULDBLOCK);
	}
	m_bConnected = TRUE;
	return false;

}

// todo:阻塞连接和非阻塞连接...
BOOL cSocket::Connect(const TCHAR * lpszAddress/*, long nSeconds*/)
{
	if(m_hSocket == INVALID_SOCKET)
		return FALSE;

	struct	sockaddr_in ServAddr;
	INT len = sizeof(sockaddr_in);
	WSAStringToAddress(const_cast<TCHAR *>(lpszAddress),AF_INET,NULL,(LPSOCKADDR)&ServAddr,&len);

	int nErr;
	if( SOCKET_ERROR == connect(m_hSocket, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) )
	{
		nErr = WSAGetLastError();
		if(nErr==WSAEWOULDBLOCK)
		{
			return FALSE;
		}
		else
		{
			return FALSE;
		}
	}
	m_bConnected = TRUE;
	return TRUE;
}

BOOL cSocket::Disconnect()
{
	TCHAR szNum[64]={0};
	LOG_INFO(DLL_LOGGER,
			_T("SOCKET HANDLE = 0x")
			<<_itot((INT_PTR)m_hSocket,szNum,16)
			<<_T(" connection disconnected"));	

	int nRet = shutdown(m_hSocket,SD_BOTH); 
	return nRet == 0;
}

void cSocket::GetLocalAddress(TCHAR * address,DWORD &addresslen)
{
	struct sockaddr_in sock;
	int	slen = sizeof(struct sockaddr_in);
	if(getsockname(m_hSocket,(struct sockaddr *)&sock,&slen)==0)
	{
		WSAAddressToString((LPSOCKADDR)&sock,slen,NULL,address,&addresslen);
	}else
		address[0]='\0';
}

void cSocket::GetPeerAddress(TCHAR * address,DWORD &addresslen)
{
	struct sockaddr_in sock;
	int	slen = sizeof(struct sockaddr_in);

	if(getpeername(m_hSocket, (struct sockaddr *) &sock, &slen)==0)
	{
		WSAAddressToString((LPSOCKADDR)&sock,slen,NULL,address,&addresslen);
	}else
	{
		int err=WSAGetLastError();
		address[0]='\0';
	}

}


BOOL cSocket::Send()
{
	return FALSE;
}
