#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include "netinterface.h"
#include "socket.h"
#include "asyncsocketio.h"
OverlapSocket::OverlapSocket()
{
	nbytepending=0;
	Id=INVALID_SESSION;
	param = 0;
	memset(&info,0,sizeof(Connection));

	berror=false;
	bnewsession=false;
	nRecvSerial = 0;
	nSendSerial = 0;
	nSendObjects = 0;
	nreqpending=0;
	nrecving=0;

	npendtimeout=-1;


	pWriteStart=NULL;
	pRecvBuf=NULL;

	//dwBytes = 0;

	pReadStart=NULL;
	pReadBuf=NULL;
	currentpacketlength=0;

	pAllocatedBase=NULL;

#ifdef _DEBUG
	hstatistic=hdebugr=hdebugw=INVALID_HANDLE_VALUE;
	debugflag=0;
	nbytesend=0;
#endif
	memset(pSendBuf,0,sizeof(pSendBuf));
	memset(nSendBuf,0,sizeof(nSendBuf));
	memset(pSendBufHeader,0,sizeof(pSendBufHeader));

	tickcount=0;


	pSendHead=NULL;
	pSendTail=NULL;


	btimerstamp[0]=btimerstamp[1]=false;
	ioffset[0]=ioffset[1]=0;
	packetheadoffset=0;
}

void OverlapSocket::Reuse()
{
	nbytepending=0;
	memset(&info,0,sizeof(Connection));

	berror=false;
	bnewsession=false;
	nRecvSerial = 0;
	nSendSerial = 0;
	nSendObjects = 0;
	nreqpending=0;
	nrecving=0;

	npendtimeout=-1;


	//dwBytes = 0;

	pRecvBuf=pReadBuf=pAllocatedBase;
	pWriteStart=pReadStart=pReadBuf+sizeof(void *);

	currentpacketlength=0;

#ifdef _DEBUG
	nbytesend=0;
#endif

	memset(pSendBuf,0,sizeof(pSendBuf));
	memset(nSendBuf,0,sizeof(nSendBuf));
	memset(pSendBufHeader,0,sizeof(pSendBufHeader));

	tickcount=0;

	pSendHead=NULL;
	pSendTail=NULL;

	ioffset[0]=ioffset[1]=0;
	packetheadoffset=0;

}

OverlapSocket::~OverlapSocket()
{

}