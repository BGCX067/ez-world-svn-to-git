#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>			// Windows 头文件:
#include "support.h"

#include "NetInterface.h"
#include "readerwriter.h"
#include "clientnet.h"

#define OVERLOAD
#include "overload.h"

cClientNet::cClientNet(iSocketIO * pSocketIO)
	:m_pSocketIO(pSocketIO)
	,m_dwSendBytes(0)
	,m_dwRecvBytes(0)
{
}

cClientNet::~cClientNet()
{
}

unsigned cClientNet::GetNewSession(LPVOID &param)
{
	return m_pSocketIO->GetNewSession(SessionStyler_Consumer,param);
}

unsigned cClientNet::GetDieSession(LPVOID &param)
{
	return m_pSocketIO->GetDieSession(SessionStyler_Consumer,param);
}

unsigned cClientNet::GetFailedSession(LPVOID &param)
{
	return m_pSocketIO->GetFailedSession(param);
}


unsigned cClientNet::AsyncConnect(const TCHAR * p_szIpAddrConnect,void *lparam)
{
	return m_pSocketIO->AsyncConnect(p_szIpAddrConnect,lparam);
}

unsigned int cClientNet::Connect(const TCHAR * p_szIpAddrConnect,DWORD dwWaitTime)
{
	return m_pSocketIO->Connect(p_szIpAddrConnect,0);
}

void cClientNet::Disconnect(UINT nSession)
{
	if (m_pSocketIO->IsValid(nSession))
	{
		m_pSocketIO->Disconnect(nSession); 
	}
}

size_t cClientNet::Send(char* p_buffer, size_t nSize,UINT nSession)
{
	assert(nSession!=INVALID_SESSION);
	m_dwSendBytes += (DWORD)nSize + m_pSocketIO->GetPacketStructSize(SessionStyler_Consumer);
	return m_pSocketIO->Write(nSession, p_buffer, nSize);
}

size_t cClientNet::Recive(cReader &reader,UINT nSession)
{
	size_t s = m_pSocketIO->Read(nSession,reader);
	if(s)
		m_dwRecvBytes += (DWORD)s + m_pSocketIO->GetPacketStructSize(SessionStyler_Consumer);
	return s;
}

void cClientNet::SendAll()
{
	m_pSocketIO->SendAll(); 
}


BOOL cClientNet::IsConnected(UINT nSession)
{
	return m_pSocketIO->IsConnected(nSession); 
}

void cClientNet::Close()
{
	m_pSocketIO->Close(); 
}

void cClientNet::SetTimerStampSignature(unsigned session,DWORD timerstampsend,DWORD timerstamprecv)
{
	m_pSocketIO->SetSendTimerStampSignature(session,true,0,timerstampsend);
	m_pSocketIO->SetRecvTimerStampSignature(session,true,0,timerstamprecv);
}

