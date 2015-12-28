#ifndef ASYNCIO_2007_1_23
#define ASYNCIO_2007_1_23
//#include <winsock2.h>
//#include <vector>
//#include "xqueue.h"

// #define IOCPOPTIMIZE			IocpOptimize		WSASend 可以使用多缓存发送.
// #define _FASTIOCPREACTION	FastIocpReaction
#define MAX_IP_LEN	32

enum IocpCmd
{
	IocpCmd_Idle,
	IocpCmd_Send,
	IocpCmd_Recv,
	IocpCmd_Accept,
	IocpCmd_Connect,
	//IocpCmd_ReqHeader,
	//IocpCmd_ReqBody,
	//IocpCmd_PostSend,
};

struct OverlapSocketOp:public OVERLAPPED
{
	IocpCmd		cmd;
};


struct Connection
{
	TCHAR	szIP[MAX_IP_LEN];

	// 用来统计峰制值流量(发|发完)
	DWORD	dwSendBytes[2];
	DWORD	dwLastSendBytes[2];
	DWORD	dwMaxSendBytes[2];

	DWORD	dwRecvBytes;
	DWORD	dwLastRecvBytes;
	DWORD	dwMaxRecvBytes;

	DWORD	dwStartTime;
};


//typedef std::vector<WSABUF> SENDBUF;

#define SOF_WRITEDUMP	1
#define SOF_RECVDUMP	2
#define SOF_STATISTICS	4

#define NUM_PRIORITY_LEVEL	4
/*
struct OverlapEx//这是Overlap_Send和OverlapSocketOp的共有结构
{
	OVERLAPPED	overlap;
	IocpCmd		cmd;
};
*/

//每个缓存一个结构
struct SendBuf
{
	DWORD		nsend;			//发送字节数
	SendBuf		*psendbufnext;
};

struct Overlap_Send;

struct OverlapSocket:public OverlapSocketOp//,public Overlap_Send
{
	UINT		Id;
	cSocket		socket;
	CRITICAL_SECTION	cs;
	LPVOID		param;
	Connection	info;

	bool		berror;		//iocp 检测到该socket发生错误,无法继续发送和接受,等待GetDieSession回收
	bool		bnewsession;//true 逻辑层已经启动
	bool		btimerstamp[2];

	//解析包裹标记
	long		packetheadoffset;
	DWORD		ioffset[2];
	DWORD		timersignature[2];

#ifdef _DEBUG
	HANDLE hdebugw,hdebugr,hstatistic;		//记录发送和接收到的数据
	DWORD	debugflag;
#endif

	UINT		nreqpending;		// 剩余多少个iocp操作未完成如果逻辑线程判决为0表示所有的iocp操作均已完成,只要逻辑线程不发起新的操作iocp线程就不会对它操作。
	UINT		npendtimeout;

	// 接收用
	char *		pRecvBuf;			// for accept [sizeof( SOCKADDR_IN) * 2 + 32]
	char *		pWriteStart;		//本次接收的起始地址
	//cXQueue		queue;
	//DWORD		dwBytes;			// 接收时需要处理的字节数
	//DWORD		dwRecved;
	char*		pReadBuf;
	char *		pReadStart;

	UINT		currentpacketlength;

	unsigned	nrecving;			//发起的接收请求

	// 发送用
	SendBuf		*pSendBuf[NUM_PRIORITY_LEVEL];			//发送尾部
	SendBuf		*pSendBufHeader[NUM_PRIORITY_LEVEL];	//发送头部
	UINT		nSendBuf[NUM_PRIORITY_LEVEL];			//缓冲数量计数

	//内存管理,减少线程抢占资源
	char *		pAllocatedBase;

	unsigned	tickcount;
	UINT		nbytepending;	// 正在被发送的对象数量。如果太多了，就应该把它断掉。


	UINT		nbytesend;			// 上一次SendAll结束到这一次SendAll开始总共发送的字节数。
	UINT		nRecvSerial;
	UINT		nSendSerial;
	UINT		nSendObjects;

	Overlap_Send *pSendHead;
	Overlap_Send *pSendTail;

	OverlapSocket();
	~OverlapSocket();
	void Reuse();
};


//每次发送建立一个结构
struct Overlap_Send:public OverlapSocketOp
{
	OverlapSocket * pIocpSocket;
	SendBuf	*psendbuf;
/*
	char *		pBuffer;
	DWORD		nsend;
	UINT		nBufferId;
	UINT		prioritylevel;
	bool		bdiscardable;
*/
	DWORD		tickcount;

	Overlap_Send *pnext,*pprev;
	bool		bsendcomplete;
	DWORD		dwSend;
};

#endif
