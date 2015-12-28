#pragma once

enum Result_Action
{
	Ra_NOTHING,			// 确认后无事发生
	Ra_QUIT,			// 确认后退出游戏
	Ra_RESTART,			// 确认后返回第一个状态
	Ra_DISCONNECT,		// 确认后断开连接
};
enum Result_Code
{
	Rc_Disconnected = -3,	// 断开了连接
	Rc_Connect_Fail = -2,	// 连接服务器失败
	Rc_Status_Fail = -1,	// 切换状态失败
	Rc_Successed = 0,
};

struct Result
{
	Result_Action action;
	Result_Code code;
	Result(Result_Action _a,Result_Code _c)
		:action(_a)
		,code(_c)
	{
	}
};