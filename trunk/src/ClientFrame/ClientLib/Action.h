#pragma once

enum ActionRetCode
{
	Action_Once_End,	// 一次播放完毕。
	Action_Time_Out,	// 持续时间到。
	Action_Broken,		// 被其他请求打断。
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