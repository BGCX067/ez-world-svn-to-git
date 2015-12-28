#pragma once
#include <vector>
#include "result.h"

class cClientNet;
class cReader;
class cWriter;

enum PACKET
{
	cl2sr_packet = 0,
	sr2cl_packet,
};

// 客户端状态的基类           
class CClientModule 
{
protected:
	cClientNet * m_pClientNet;				// 网络设备接口
	cReader * m_pReader;					// 用于从网络设备中读取数据
	cWriter * m_pWriter;					// 用于向网网络设备写入数据
	int m_nSession;

	// 构造函数被保护,不能直接构造基类.
	CClientModule(cClientNet * pClientNet,cReader * pReader,cWriter * pWriter,int nSession);	
	// 未被相应过的服务器消息的处理.
	virtual Result OnServerMessageDefault(int cmd,cReader* pReader,size_t len);

public:
	// 从网络设备读取指定类型的数据.
	template <class T>
	__forceinline T* Read()	
	{
		return (T*)m_pReader->Struct(sizeof(T));
	}

	// 从网络设备读取指定数量,指定类型的数据.
	template <class T>
	__forceinline T* Read(size_t p_num)
	{
		return (T*)m_pReader->Struct(sizeof(T)*p_num);
	}

	// 向网络设备写入指定类型的数据.
	template <class T>
	__forceinline void Write(T _t)	
	{
		m_pWriter->Struct(&_t,sizeof(T));
	}

	// 向网络设备写入指定数量,指定类型的数据.
	template <class T>
	__forceinline void Write(T *_t,size_t num)	
	{
		m_pWriter->Struct(_t,num);
	}
	
	// 向网络设备写入字符串
	template <class T>
	__forceinline void WriteString(T *_t,size_t num)	
	{
		m_pWriter->String(_t,num);
	}
	
	// 开始向网络设备写入指定的命令.
	template <class T>
	__forceinline void Begin(T nCmd)					
	{
		//m_pWriter->Begin(cl2sr_packet);
		(*m_pWriter)<<CBEGIN();
		m_pWriter->INT(nCmd);
	}


	
	// 发送向网络写入的所有数据.
	size_t Send();

	// 析构函数
	virtual ~CClientModule(){}
	// 初始化
	virtual bool OnInit() = 0;
	// 更新每桢数据,返回>=0的数据表示结束转换到下个状态
	virtual int OnLoop(DWORD dwTime){return 0;}	

	// 网络消息循环.
	virtual void OnNetLoopBegin() {}
	virtual Result OnServerMessage(int cmd,cReader* pReader,size_t len){return Result(Ra_NOTHING,Rc_Successed);}
	virtual void OnNetLoopEnd() {}
};

#define SERVER_COMMAND_BEGIN(the_class)						\
	Result the_class :: OnServerMessage(int cmd,cReader* pReader,size_t len)		\
	{														\
		switch(cmd)											\
		{													
#define SERVER_COMMAND(cmd,func)							\
		case cmd:											\
			return func(pReader);									\
			break;											
#define SERVER_COMMAND_END									\
		case -1:											\
		default:											\
			return OnServerMessageDefault(cmd,pReader,len);	\
			break;											\
		}													\
		return OnServerMessageDefault(cmd,pReader,len);		\
	}	



#define SERVER_COMMAND_DECLARE								\
	void OnServerMessage(int cmd,cReader* pReader,size_t len)

class CClientState : public CClientModule
{
protected:
	struct Module
	{
		CClientModule * pModule;
		int nSrvCmdRange[2];
		int nGuiDlgRange[2];
	};

	std::vector<Module> m_vecModules;
	UINT m_nModules;
	int m_nState;		
	int m_nStateTo;		// 转向的状态

	CClientState(cClientNet * pClientNet,cReader * pReader,cWriter * pWriter,int nSession);
	void RegisterModule(CClientModule * pModule,int nMinSrvCmd = -1,int nMaxSrvCmd = -1,int nMinGuiDlg = -1,int nMaxGuiDlg = -1);
	Result OnServerMessageDefault(int cmd,cReader* pReader,size_t len);

public:
	virtual ~CClientState();
	int GetState(){return m_nState;}
	virtual int OnLoop(DWORD dwTime);						// 更新每桢数据,返回>=0的数据表示结束转换到下个状态
	virtual bool OnInit();
};