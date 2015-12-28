#define WIN32_LEAN_AND_MEAN
#include <process.h>
#include "ZipArchive.h"
#include "smtp\CSmtp.h"
#include "BugReport.h"
#include "strcode.h"

char m_szExceptFile[MAX_PATH] = {0};

CMessageBoxEx * CMessageBoxEx::m_tThis = NULL;
CMessageBoxEx::CMessageBoxEx()
	:hMsgBoxHook(NULL)
{
	m_tThis = this;	
}
CMessageBoxEx::~CMessageBoxEx()
{
	m_tThis = NULL;
}

LRESULT CALLBACK CMessageBoxEx::CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd;
	if(nCode < 0)
		return CallNextHookEx(m_tThis->hMsgBoxHook, nCode, wParam, lParam);
	switch(nCode)
	{
	case HCBT_ACTIVATE:
		{
		// 现在wParam中就是message box的句柄
		hwnd = (HWND)wParam;
		
		// 我们已经有了message box的句柄，在这里我们就可以定制message box了!
		SetDlgItemText(hwnd,IDOK,_T("报告异常"));
		SetDlgItemText(hwnd,IDCANCEL,_T("停止程序"));
		}		
		return 0;
	}
	// Call the next hook, if there is one
	return CallNextHookEx(m_tThis->hMsgBoxHook, nCode, wParam, lParam);
}
int CMessageBoxEx::Show(HWND hwnd, TCHAR *szText, TCHAR *szCaption, UINT uType)
{
	int ret;
	// Install a thread hook, so we can customize it
	hMsgBoxHook = SetWindowsHookEx(
		WH_CBT,
		CBTProc,
		NULL,
		GetCurrentThreadId()
		);
	// Display a standard message box
	ret = MessageBox(hwnd, szText, szCaption, uType);
	// remove the window hook
	UnhookWindowsHookEx(hMsgBoxHook);
	return ret;
}
COutputCallback::COutputCallback(LPCTSTR lpszVersion)
	:m_lpszVersion(CTcharToChar(lpszVersion).Get())
{
}
void COutputCallback::Output(LPEXCEPTION_POINTERS e,LPCTSTR lpszLogFile,LPCTSTR lpszDumpFile)const
{
	_snprintf(m_szExceptFile,MAX_PATH,"0x%08x_%08x_%s.zip",e->ExceptionRecord->ExceptionAddress,GetTickCount(),m_lpszVersion);
	
	// 压缩文件
	CZipArchive zip;
	zip.Open(CCharToTchar(m_szExceptFile).Get(),CZipArchive::zipCreate);
	zip.AddNewFile(lpszDumpFile);
	if(lpszLogFile)
	{
		BOOL b = zip.AddNewFile(lpszLogFile);
	}
	zip.Close();
}

COutputCallbackEmail::COutputCallbackEmail(const EmailConfig * pConfig,LPCTSTR lpszVersion)
	:COutputCallback(lpszVersion)
	,m_pConfig(pConfig)
{
}

COutputCallbackEmail::~COutputCallbackEmail()
{
}

void COutputCallbackEmail::Output(LPEXCEPTION_POINTERS e,LPCTSTR lpszLogFile,LPCTSTR lpszDumpFile)const
{
	COutputCallback::Output(e,lpszLogFile,lpszDumpFile);

	CMessageBoxEx box;
	TCHAR szMessage[MAX_PATH] = {0};
	//_stprintf(szMessage,_T("Sorry,program abnormal at addrress 0x%08x.\nPlease report to us in time,so we can do better job,thank you!"),e->ExceptionRecord->ExceptionAddress);
	_stprintf_s(szMessage,MAX_PATH,_T("抱歉,程序崩溃在地址 0x%08x.\n请将此情况发送给我们以便及时改进错误(请点击左边按钮),谢谢!"),e->ExceptionRecord->ExceptionAddress);
	int nRet = box.Show(NULL,szMessage,_T("程序异常"),MB_OKCANCEL|MB_ICONERROR);
	if(nRet == IDOK)
	{
		CSmtp mail;
		if(mail.GetLastError() != CSMTP_NO_ERROR)
		{
			printf("Unable to initialise winsock2.\n");
		}

		char nSendName[16] = {0};
		sprintf(nSendName,"0x%08x",e->ExceptionRecord->ExceptionAddress);
		mail.SetSubject(nSendName);
		mail.SetSMTPServer(m_pConfig->lpszSmtpIp,m_pConfig->nSmtpPort);
		mail.SetLogin(m_pConfig->lpszUsername);
		mail.SetPassword(m_pConfig->lpszPassword);
		mail.AddRecipient(m_pConfig->lpszRecipient);	// 接收者
		mail.SetSenderName(m_lpszVersion);
		mail.SetSenderMail(m_pConfig->lpszSender);
		mail.SetReplyTo(m_pConfig->lpszSender);
		mail.SetXPriority(XPRIORITY_NORMAL);
		mail.SetXMailer(m_lpszVersion);
		mail.SetMessageBody("Except");
		mail.AddAttachment(m_szExceptFile);

		if( mail.Send() )
			MessageBoxA(GetActiveWindow(),"发送成功","运气不错:)",MB_OK);
		else
			MessageBoxA(GetActiveWindow(),GetErrorText(mail.GetLastError()),"运气太差了:(",MB_ICONSTOP);

		DeleteFileA(m_szExceptFile);
	}
}