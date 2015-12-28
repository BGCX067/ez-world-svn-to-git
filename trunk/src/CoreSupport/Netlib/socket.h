#pragma once

#include <Winsock2.h>
#include "NetInterface.h"

enum IoMode
{
	Io_Iocp,
	Io_CmpRt,			//using completion routine
	Io_OverlappedEx,
};

class cSocket
{
	SOCKET	m_hSocket;
	//int		m_nProtocol;//never used
	BOOL	m_bConnected;

	//static BOOL m_bWSAStartup;
	
public:
	cSocket();
	~cSocket();

	BOOL Create(int nProtocol);
	void Destroy();
	void GetLocalAddress(TCHAR * address,DWORD &addresslen);
	void GetPeerAddress(TCHAR * address,DWORD &addresslen);
	
	//BOOL Recv(char *buf,u_long len,OVERLAPPED* pObject,LPWSAOVERLAPPED_COMPLETION_ROUTINE prt);
	BOOL Send();

	BOOL Listen(IpAddrType eType,const TCHAR * lpszAddress,unsigned backlog=SOMAXCONN);
	bool AsyncConnect(const TCHAR * lpszAddress,HANDLE hevent);
	BOOL Connect(const TCHAR * lpszAddress/*, long nSeconds*/);
	BOOL Disconnect();

	SOCKET GetHandle()
	{
		return m_hSocket;
	}
	BOOL IsValid()const
	{
		return (m_hSocket != INVALID_SOCKET);
	}
	BOOL IsConnected(){return m_bConnected;}
	void SetConnected(){m_bConnected = TRUE;}

};
