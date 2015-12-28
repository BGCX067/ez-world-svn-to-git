#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <assert.h>
#include "socket.h"
#include "socketio.h"

#define OVERLOAD
#include "overload.h"

CSocketIO::CSocketIO(const SocketIOConfig * pConfig)
{
	unsigned i;
	for(i=0;i<SessionStyler_Max;i++)
	{
		m_nPacketHeadSize[i] = 0;
		if(pConfig[i].bPacketFlags)	
			m_nPacketHeadSize[i] += sizeof(PacketHeader);
		m_nPacketLengthSize[i] = 0;
		m_nPacketLengthSize[i] = sizeof(unsigned short);
		m_nPacketStructSize[i] = m_nPacketHeadSize[i];
		m_nPacketStructSize[i] += m_nPacketLengthSize[i];
		if(pConfig[i].bPacketFlags)	
			m_nPacketStructSize[i] += sizeof(PacketEnd);
		m_dwLengthMask[i]=0x0000ffff;
	}
	m_pConfig = pConfig;
}

CSocketIO::~CSocketIO(void)
{
}
/*
unsigned CSocketIO::GetFailedSession(LPVOID &param)
{
	UINT nid=-1;
	std::set<UINT>::iterator itskset;
	m_failedcs.Enter();
	for(itskset=m_failedconnect.begin();itskset!=m_failedconnect.end();itskset++)
	{
		nid=*itskset;
		assert(nid!=-1);
		OverlapSocket *psocket;
		psocket=m_socketlist[SessionStyler_Consumer].Get(nid&0x7fffffff);
		if(psocket)
		{
			if(psocket->nreqpending)
			{
				nid=-1;
				continue;
			}
			param=psocket->param;
		}
		m_failedconnect.erase(itskset);
		if(psocket)
			FreeSocket(SessionStyler_Consumer,psocket);
		break;
	}
	m_failedcs.Leave();
	return nid;
}
*/

/*
unsigned CSocketIO::GetOverflowSession(LPVOID &param)
{
	UINT nid;
	std::set<UINT>::iterator itskset;
	//m_csoverflow.Enter();
	itskset=m_overflowset.begin();
	if(itskset!=m_overflowset.end())
	{
		nid=*itskset;
		assert(nid);
		param=m_socketlist.Get(nid)->param;
		m_overflowset.erase(itskset);
	}else{
		return 0;
	}
	//m_csoverflow.Leave();
	return nid;
}
*/
#ifdef _DEBUG
void CSocketIO::SetDebugFile(unsigned nsession,DWORD flag,LPCTSTR pfilename)
{
	OverlapSocket *psocket;
	psocket=m_socketlist[(nsession&0x80000000)>>31].Get(nsession&0x7fffffff);
	assert(psocket);
	if(!psocket)
		return;
	psocket->debugflag=flag;
	TCHAR file[MAX_PATH];
	if(flag&SOF_RECVDUMP&&psocket->hdebugr==INVALID_HANDLE_VALUE)
	{
		if(!pfilename)
			_stprintf_s(file,MAX_PATH,_T("socket%dr.dat"),nsession);
		else 
			_tcscpy_s(file,MAX_PATH,pfilename);
		psocket->hdebugr=CreateFile(file,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		assert(psocket->hdebugr!=INVALID_HANDLE_VALUE);
	}
	if(!(flag&SOF_RECVDUMP)&&psocket->hdebugr!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(psocket->hdebugr);
		psocket->hdebugr=INVALID_HANDLE_VALUE;
	}
	if(flag&SOF_WRITEDUMP&&psocket->hdebugw==INVALID_HANDLE_VALUE)
	{
		if(!pfilename)
			_stprintf_s(file,MAX_PATH,_T("socket%dw.dat"),nsession);
		else 
			_tcscpy_s(file,MAX_PATH,pfilename);
		psocket->hdebugw=CreateFile(file,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		assert(psocket->hdebugw!=INVALID_HANDLE_VALUE);
	}
	if(!(flag&SOF_WRITEDUMP)&&psocket->hdebugw!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(psocket->hdebugw);
		psocket->hdebugw=INVALID_HANDLE_VALUE;
	}
	if(flag&SOF_STATISTICS&&psocket->hstatistic==INVALID_HANDLE_VALUE)
	{
		if(!pfilename)
			_stprintf_s(file,MAX_PATH,_T("socket%dstatis.dat"),nsession);
		else 
			_tcscpy_s(file,MAX_PATH,pfilename);
		psocket->hstatistic=CreateFile(file,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		assert(psocket->hstatistic!=INVALID_HANDLE_VALUE);
	}
	if(!(flag&SOF_STATISTICS)&&psocket->hstatistic!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(psocket->hstatistic);
		psocket->hstatistic=INVALID_HANDLE_VALUE;
	}
}
#endif
unsigned CSocketIO::GetNextSession(LPVOID &param,SessionStyler st,unsigned nsession)
{
	OverlapSocket *psocket;
	if(nsession==INVALID_SESSION)
	{
		psocket=m_socketlist[st].NextData();
	}else
	{
		psocket=m_socketlist[st].Get(nsession&0x7fffffff);
		psocket=m_socketlist[st].NextData(psocket);
	}

	for(;psocket;psocket=m_socketlist[st].NextData(psocket))
	{
		if(!psocket->bnewsession)
		{
			continue;
		}else{
			param=psocket->param;
			return m_socketlist[st].GetId(psocket)|(st<<31);
		}
	}
	return -1;
}
