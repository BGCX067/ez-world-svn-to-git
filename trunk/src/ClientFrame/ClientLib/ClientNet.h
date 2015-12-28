#pragma once

class iSocketIO;
class cReader;
class cClientNet
{
	iSocketIO *	m_pSocketIO;
	DWORD		m_dwSendBytes;
	DWORD		m_dwRecvBytes;

public:
	unsigned GetNewSession(LPVOID &param);
	unsigned GetDieSession(LPVOID &param);
	unsigned GetFailedSession(LPVOID &param);
	cClientNet(iSocketIO * pSocketIO);
	~cClientNet();
	unsigned AsyncConnect(const TCHAR * p_szIpAddrConnect,void *lparam=NULL);				// 产生一个新连接	
	unsigned int Connect(const TCHAR * p_szIpAddrConnect,DWORD dwWaitTime = 1000);				// 产生一个新连接
				
	BOOL IsConnected(UINT nsession);
	void Disconnect(UINT nSession = 1);							// 断开
	size_t Send(char* p_buffer, size_t nSize,UINT nSession = 1);// 发送消息
	size_t Recive(cReader &,UINT nSession);						// 接收消息
	void SendAll();
	void Close();												// 断开
	void SetTimerStampSignature(unsigned session,DWORD timerstamp=0,DWORD timerstamprecv=0);
};
