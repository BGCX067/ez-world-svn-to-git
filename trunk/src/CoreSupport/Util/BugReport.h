#ifndef __BUGREPORT_H__
#define __BUGREPORT_H__

#include "StackTrace.h"

struct EmailConfig
{
	LPCSTR lpszSmtpIp;
	UINT	nSmtpPort;
	LPCSTR lpszUsername;
	LPCSTR	lpszPassword;
	LPCSTR	lpszRecipient;
	LPCSTR	lpszSender;
};

class CMessageBoxEx 
{
	HHOOK hMsgBoxHook;
	static CMessageBoxEx * m_tThis;
	static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);

public:
	CMessageBoxEx();
	~CMessageBoxEx();
	int Show(HWND hwnd, TCHAR *szText, TCHAR *szCaption, UINT uType);
};

class COutputCallback :  public IStackTraceCallback
{
protected:
	LPCSTR	m_lpszVersion;
public:
	COutputCallback(LPCTSTR lpszVersion);
	LPCSTR GetVersion()const{return m_lpszVersion;}
	virtual void Output(LPEXCEPTION_POINTERS e,LPCTSTR lpszLogFile,LPCTSTR lpszDumpFile)const;
};

class COutputCallbackEmail : public COutputCallback
{
	const EmailConfig * m_pConfig;
public:
	COutputCallbackEmail(const EmailConfig * pConfig,LPCTSTR lpszVersion);
	virtual ~COutputCallbackEmail();
	void Output(LPEXCEPTION_POINTERS e,LPCTSTR lpszLogFile,LPCTSTR lpszDumpFile)const;
	
};

#endif//__BUGREPORT_H__
