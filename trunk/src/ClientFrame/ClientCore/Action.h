#pragma once

enum ActionRetCode
{
	Action_Once_End,	// һ�β�����ϡ�
	Action_Time_Out,	// ����ʱ�䵽��
	Action_Broken,		// �����������ϡ�
};

class Action
{
public:
	virtual int GetType() const = 0;
	virtual const char * GetName()const = 0;
	virtual int Do() = 0;
	virtual void * GetParams() const = 0;
	virtual void SetParams(void * pParams) = 0;
};

class Idle : public Action
{
public:
	int GetType()const{return 1;}
	const char * GetName()const {return "Idle";}
	int Do();
	void * GetParams() const {return NULL;}
	void SetParams(void * pParams){}
};