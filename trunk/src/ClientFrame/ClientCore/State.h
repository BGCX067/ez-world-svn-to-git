#pragma once
#include <vector>

class iClientNet;
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
	iClientNet * m_pClientNet;				// 网络设备接口
	cReader * m_pReader;					// 用于从网络设备中读取数据
	cWriter * m_pWriter;					// 用于向网网络设备写入数据

	// 构造函数被保护,不能直接构造基类.
	CClientModule();	
	// 未被相应过的服务器消息的处理.
	virtual void OnServerMessageDefault(int cmd,cReader* pReader,size_t len);
	// 未被相应过的界面消息的处理.
	virtual bool OnGuiMessageDefault(UINT nMsg,WPARAM wParam, LPARAM lParam){return false;}	

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
	// 响应windows标准窗口消息
	virtual bool OnWndMessage(UINT message, WPARAM wParam, LPARAM lParam);					
	// 更新每桢数据,返回>=0的数据表示结束转换到下个状态
	virtual int OnLoop(DWORD dwTime){return -1;}	
	// 渲染模块数据
	virtual void OnRender(){}
	// 相应弹出对话框的玩家输入.
	// (因为游戏是基于帧循环,对话框的处理不能使用真正的模态)
	virtual bool OnMessageBox(UINT nCode,UINT nClickBt=0){return false;}
	
	// 响应界面系统(ui::CUiManager2004)的消息
	virtual bool OnGuiMessage(UINT nMsg,WPARAM wParam, LPARAM lParam){return false;}
	
	virtual void FormDialog(int nDialogLeft,int nDialogRight,bool bCenter=true);

	// 网络消息循环.
	virtual void OnNetLoopBegin() {}
	virtual void OnServerMessage(int cmd,cReader* pReader,size_t len){}
	virtual void OnServerBegin(int nCmd) {}
	virtual void OnServerEnd(int nCmd) {}
	virtual void OnNetLoopEnd() {}
};

#define SERVER_COMMAND_BEGIN(the_class)						\
	void the_class :: OnServerMessage(int cmd,cReader* pReader,size_t len)		\
	{														\
		OnServerBegin(cmd);                                 \
		switch(cmd)											\
		{													
#define SERVER_COMMAND(cmd,func)							\
		case cmd:											\
			func(pReader);									\
			break;											
#define SERVER_COMMAND_END									\
		case -1:											\
		default:											\
			OnServerMessageDefault(cmd,pReader,len);		\
			break;											\
		}													\
		OnServerEnd(cmd);                                   \
	}	



#define SERVER_COMMAND_DECLARE								\
	void OnServerMessage(int cmd,cReader* pReader,size_t len)


#define GUI_COMMAND_BEGIN(the_class)						\
	bool the_class :: OnGuiMessage(UINT nMsg,WPARAM wParam, LPARAM lParam)	\
	{														\
		UINT DlgID = (UINT)((wParam>>14)&0x1ff);			\
		UINT CtrlID=(UINT)(wParam&0x3fff);					\
		UINT CtrlMsg=(UINT)((wParam>>23)&0xff);				\
		UINT nFlag = (UINT)((wParam>>31)&0x1);				\
		if (1==nFlag)										\
		{													\
			return OnGuiMessageDefault(nMsg,wParam,lParam);	\
		}													\
															\
		switch(DlgID)										\
		{													
#define GUI_DIALOG_COMMAND_BEGIN(dlgid)						\
		case dlgid:											\
			{												

#define GUI_COMMAND(cmdid,event,func)						\
				if((cmdid == CtrlID) && (event == CtrlMsg))	\
				{											\
					func(lParam);							\
					return true;							\
				}
#define GUI_COMMAND_RANGE(cmdid_first,cmdid_last,event,func)\
				if((CtrlID >= (cmdid_first))				\
					&& (CtrlID <= (cmdid_last))				\
					&& (event == CtrlMsg))					\
				{											\
					func(CtrlID,lParam);					\
					return true;							\
				}
#define GUI_DIALOG_COMMAND_END								\
			}												\
			break;											
#define GUI_COMMAND_END										\
		case -1:											\
		default:											\
			return OnGuiMessageDefault(nMsg,wParam,lParam);	\
			break;											\
		}													\
		return OnGuiMessageDefault(nMsg,wParam,lParam);		\
	}	

#define GUI_COMMAND_DECLARE									\
	bool OnGuiMessage(UINT nMsg,WPARAM wParam, LPARAM lParam)



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

	CClientState();
	void RegisterModule(CClientModule * pModule,int nMinSrvCmd = -1,int nMaxSrvCmd = -1,int nMinGuiDlg = -1,int nMaxGuiDlg = -1);
	void OnServerMessageDefault(int cmd,cReader* pReader,size_t len);
	bool OnGuiMessageDefault(UINT nMsg,WPARAM wParam, LPARAM lParam);

public:
	virtual ~CClientState();
	int GetState(){return m_nState;}
	virtual int OnLoop(DWORD dwTime);						// 更新每桢数据,返回>=0的数据表示结束转换到下个状态
	virtual bool OnWndMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual bool OnMessageBox(UINT nCode,UINT nClickBt=0);
	virtual bool OnInit();
};