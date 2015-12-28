#pragma once
#include <tchar.h>
#include "EzModule.h"

struct LogicInterfaceConfig : public iInitParam
{
};

enum FunctionCallCode
{
	fc_success = 0,	// �������óɹ�
	fc_not_found,	// ������û�ҵ�
	fc_params_error,// ���������������
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
