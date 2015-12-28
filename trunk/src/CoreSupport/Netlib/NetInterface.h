#pragma once
#include <tchar.h>
#include "EzModule.h"

#define PACKET_FIXED		(1<<0)	// 使用定长包
#define PACKET_BEGINEND		(1<<1)	// 使用包 头/尾 标记+序列号

#define MAX_FAILED			10		// 最多失败次数
#define MAX_IP_LEN			32

class cReader;
class cWriter;
struct SocketIOConfig : public iInitParam
{
	int nMaxSocket;					// 最大连接数

	unsigned long nRecvBufLen;			// 接受缓存长度

	unsigned long nSendPool;			// 发送池数量 can be shared
	unsigned long nSendBufLen;			// 发送缓存

	unsigned long nDiscardThreshold;	// 悬挂在windows内部和网络模块中的包裹数量达到此数字开始丢弃发送包裹
	unsigned long nPendLimit;			// 推送限制

	unsigned long dwPacketBegin;	// 包头标记
	unsigned long dwPacketEnd;		// 包尾标记
	BOOL bPacketFlags;				// 是否包含标记
};

struct PacketHeader
{
	unsigned long dwBegin;		
	unsigned int nSerial;		// 包的序列号
};

struct PacketEnd
{
	unsigned long dwEnd;
};

struct PacketStructSize
{
	PacketHeader ph;
	PacketEnd pe;
};

enum SessionStyler
{
	SessionStyler_Provider,	// 服务提供者
	SessionStyler_Consumer,	// 服务消费者
	SessionStyler_Max,
};
enum IpAddrType
{
	IpAddr_Any,
	IpAddr_IP,
	IpAddr_Local,
};

struct SocketIOStatistics
{
	unsigned long		m_dwSendBytes;					// 请求发送的数据
	unsigned long		m_dwSendedBytes;				// 实际发送的数据
	unsigned long		m_dwSendTimes;					// WSASend,send调用次数

	unsigned long		m_dwRecvBytes;					// 接收到的数据
	unsigned int		m_nRecvFulls;					// 效率统计，接收缓存满的次数

	long		m_nmaxsendpacket;				// 发送池的最大数量
	long		m_ncursendpacket;				// 发送池的当前数量

	unsigned long		maxFrameSendBytes;				// 发送字节数帧峰值
	unsigned long		maxFrameSendTimes;				// 发送次数帧峰值

	unsigned long		niopend;
	unsigned long		niobuffered;
	unsigned long		nmaxsend;
	unsigned long		nmaxrecv;
};

#define STATSTICS_CLEAR	1
#define INVALID_SESSION	-1

class cSocket;

class iSessionEvent
{
public:
	virtual void OnPacketError(SessionStyler,unsigned int nsession,void * param)=0;
};

// nSession 高4bits代表了 SessionStyler.范围是 0~7
/** \ingroup NetLib
  * \brief
  * 通过网络收发数据,在网络事件产生的时候回调函数
  * Describe:
  */
class iSocketIO : public iModule
{

public:
	virtual ~iSocketIO(){}
	/**
	  * \brief
	  * 侦听连接请求
	  * Describe:
	  * \param eType		侦听ip地址的类型
	  * \param lpszAddress	侦听的地址
	  * \param param		当侦听到连接请求时将此参数传给事件函数对Listen本身无影响
	  * \return 
	  */
	virtual int Listen(IpAddrType eType,const TCHAR * lpszAddress, void * param=NULL) = 0;		//启动监听模式.只在 SessionStyler_Provider 时使用
	/**
	  * \brief
	  * 异步连接lpszAddress所指定的ip地址
	  * Describe:参见GetFailedSession,GetNewSession
	  * \param lpszAddress 
	  * \param param 当连接成功或者失败时返回给调用代码，对AsyncConnect本身无影响
	  * \return 
	  */
	virtual unsigned AsyncConnect(const TCHAR * lpszAddress, void * param)=0;					//启动非阻塞连接.只在 SessionStyler_Consumer 时使用
	/**
	  * \brief
	  * 连接lpszAddress所指定的ip地址
	  * Describe:
	  * \param lpszAddress 
	  * \param param 
	  * \return 返回会话标识符 0连接失败 非0连接成功
	  */
	virtual unsigned Connect(const TCHAR * lpszAddress, void * param)=0;//阻塞
	virtual void Disconnect(unsigned int nSession) = 0;								//断开指定的连接.
	/**
	  * \brief
	  * 关闭所有网络通信，释放所有资源
	  * Describe:
	  */
	virtual void Close()=0;															//关闭所有连接.
	

	virtual void SetPendTimeOut(unsigned nSession,unsigned long npendtimeout)=0;
	/**
	  * \brief
	  * 将所有Write操作缓冲的数据发送到网络
	  * Describe:
	  */
	virtual void SendAll()=0;														//Flush所有连接的缓存.Write并不真正总是发送.所以在每个逻辑循环一定要做一次SendAll
	//virtual void Send(unsigned int nSession) = 0;									//Flush指定连接的缓存.Write并不真正总是发送.Send可以把指定连接的缓存立即Flush掉.
	virtual void SendSyn(unsigned int nSession,char * buffer, size_t nSize)=0;		//同步发送数据.不通过发送池,使用同步的Send发送数据.发送完才返回.

	/**
	  * \brief
	  *	从连接中读取一个数据块
	  * Describe:
	  * \param nSession 连接的标示符
	  * \param reader 数据存放的目标位置
	  * \return 返回读取到的字节数
	  */
	virtual size_t Read(unsigned int nSession,cReader &reader)=0;						//read one packet from network

	/**
	  * \brief
	  * 将buffer缓冲中长度为nSize数据作为一个数据块发送出去
	  * Describe:
	  * \param nSession 连接标示符
	  * \param buffer	数据存放的起始地址
	  * \param nSize	数据长度字节计算
	  * \param flag		发送标志
	  * \return 
	  */
	virtual size_t Write(unsigned int nSession,const char * buffer, size_t nSize,unsigned ipriority=0,bool bdiscardbale=false) = 0;	//异步 write one packet to network.
	virtual size_t Write(unsigned int nSession,cWriter &writer,unsigned ipriority=0,bool bdiscardbale=false) = 0;

	virtual const TCHAR * GetConnInfo(unsigned int nSession) = 0;							//取得连接的IP信息.
	/**
	  * \brief
	  * 获取st中新的会话连接的标示符
	  * Describe:
	  * \param st 
	  * \param nSessionType 用户设定的参数见AsyncConnect和Listen
	  * \return 
	  */
	virtual unsigned int GetNewSession(SessionStyler st,void * & nSessionType) = 0;	//取得新的连接.
	/**
	  * \brief
	  * 获取st中已断开连接的标示符,返回的标示符已被iSocketIO释放，不能再通过iSocketIO进行操作
	  * Describe:
	  * \param st 
	  * \param &param 
	  * \return 
	  */
	virtual unsigned int GetDieSession(SessionStyler st,void * &param)=0;			//取得断开的连接.
	/**
	  * \brief
	  * 获取AsyncConnect连接失败的标示符
	  * Describe:
	  * \param &param 
	  * \return 
	  */
	virtual	unsigned GetFailedSession(void * &param)=0;			//取得错误的Session	(目前尚未使用.意图是把Socket的错误传递给逻辑层)
#ifdef _DEBUG
	/**
	  * \brief
	  * 将收发数据记录中文件中以便调试
	  * Describe:
	  * \param nsession 
	  * \param flag 
	  * \param file 
	  */
	virtual void SetDebugFile(unsigned nsession,unsigned long flag,const TCHAR * file=NULL)=0;		
#endif

	//获得单个session属性
	//virtual unsigned GetSendPacketNo(unsigned nsession)=0;					// 取得当前连接的发送序列号(校验用.目前尚未使用)
	virtual int IsConnected(unsigned int nSession = 1) = 0;						// 判断是否还在连通状态.
	virtual int IsValid(unsigned int nSession)=0;							// 对应的socket是否已经被关闭
	/**
	  * \brief
	  * 设置一个连接队列的用户参数
	  * Describe:
	  * \param nsession 
	  * \param lparam 
	  */
	virtual void SetParam(unsigned nsession,void * lparam)=0;				// 设置指定 Session 的用户参数.用来把用户自己的变量内容关联到Session上.
	/**
	* \brief
	* 获取一个连接队列的用户参数
	* Describe:
	* \param nsession 
	*/
	virtual void * GetParam(unsigned nsession)=0;							// 取得指定 Session 的用户参数

	virtual void Statistics(SessionStyler st,SocketIOStatistics * info,unsigned long flag)=0;	// 取得统计信息
	/**
	  * \brief
	  * 返回nsession得下一个连接
	  * Describe:
	  * \param &param 
	  * \param nsession 
	  * \return 
	  */
	virtual unsigned GetNextSession(void * &param,SessionStyler st,unsigned nsession=INVALID_SESSION)=0;	// 遍历所有Session.
	virtual size_t GetPacketStructSize(unsigned st) = 0;

	virtual const cSocket &GetListenSocket()=0;

	virtual void SetSendTimerStampSignature(unsigned session,bool benable=false,unsigned long ichar=0,unsigned long timerstamp=0)=0;
	virtual void SetRecvTimerStampSignature(unsigned session,bool benable=false,unsigned long ichar=0,unsigned long timerstamp=0)=0;

	virtual unsigned GetPendedDataSize(unsigned session)=0;

	virtual void	SetDebugSession(unsigned session)=0;
};

#if defined( NETLIB_EXPORTS )
#	define NETLIB_EXPORTS_API	extern "C" __declspec( dllexport )
#	define NETLIB_EXPORTS_CLASS	__declspec( dllexport )
#else
#	define NETLIB_EXPORTS_API	extern "C" __declspec( dllimport )
#	define NETLIB_EXPORTS_CLASS	__declspec( dllexport )
#endif



