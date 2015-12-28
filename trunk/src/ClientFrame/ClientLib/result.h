#pragma once

enum Result_Action
{
	Ra_NOTHING,			// ȷ�Ϻ����·���
	Ra_QUIT,			// ȷ�Ϻ��˳���Ϸ
	Ra_RESTART,			// ȷ�Ϻ󷵻ص�һ��״̬
	Ra_DISCONNECT,		// ȷ�Ϻ�Ͽ�����
};
enum Result_Code
{
	Rc_Disconnected = -3,	// �Ͽ�������
	Rc_Connect_Fail = -2,	// ���ӷ�����ʧ��
	Rc_Status_Fail = -1,	// �л�״̬ʧ��
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