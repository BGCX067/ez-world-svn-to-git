#pragma once
#include <tchar.h>
#include "EzModule.h"

struct LogicInterfaceConfig : public iInitParam
{
};

enum FunctionCallCode
{
	fc_success = 0,	// 函数调用成功
	fc_not_found,	// 函数名没找到
	fc_params_error,// 输入参数解析错误
	fc_continue,
};

class iFunctionCall
{
public:
	virtual int Call(int nCallID,const char * lpszFunction,const char * pInParams,size_t nInParams,char * pOutParams,size_t * nOutParams) = 0;
};

class iLogicInterface : public iModule,public iFunctionCall
{
public:
	virtual ~iLogicInterface(){}
	virtual BOOL Initialize(const char * lpszDll,iFunctionCall * pHostFunc) = 0;
};
