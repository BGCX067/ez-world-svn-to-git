#pragma once
#include	<crtdbg.h>
#include	<errno.h>
#include   <tchar.h>

#define MAX_NAME	32

class cInstNode;

//影响公式定义
struct FormulaName
{
	TCHAR	m_szName[MAX_NAME];	// 公式名称
};

//最终值公式定义
struct FieldFormula
{
	UINT	m_nUintID;			// 单元ID
	UINT	m_nFieldID;			// 字段ID
	TCHAR	m_szName[MAX_NAME];	// 公式指针
};

enum FormulaType
{
	EffectFormula,
	FieldFormula,
	BehaviorFormula,
	SkillFormula,
};


class cManager
{
public:
	virtual ~cManager(){}
};

class cBaseFormula
{
//0.【公式实体】基类
public:
	cBaseFormula(){}
	virtual ~cBaseFormula(){}
	//设置管理者
	virtual void SetManager(cManager * pm){m_pm = pm;}
	//将自己所有的公式交给他的管理者托管
	virtual void Init(){}

protected:
	//管理者指针
	cManager * m_pm;
};

class cEffectFormulaManager : public cManager
{
//1.【影响方式】公式管理者接口
public:
	//得到影响方式公式的数量
	virtual UINT GetEffectOpCnt() = 0;
	//调用公式
	virtual float Formula(const UINT nEffectOpID,float v,float v_max,double a) = 0;
};

class cFieldFormulaManager : public cManager
{
//2.【字段最终值】公式管理者接口
public:
	virtual const TCHAR	* GetFieldFormula(const UINT nUnitID,const UINT nFieldID) = 0;
	virtual float Formula(const TCHAR * lpszFunc,cInstNode * pInstNode,int * v) = 0;
};

class cSkillFormulaManager : public cManager
{
//3.【技能伤害】公式管理者接口
public:
	//根据公式ID执行脚本
	virtual float Formula(const TCHAR * lpszFunc,cInstNode * pSrcInstNode,cInstNode * pDstInstNode,double dwMin,double dwMax) = 0;
};

class cFormulaManager
{
//4.【计算公式】总管接口(兼职资源总管)
public:
	//得到【影响方式  】公式管理者
	virtual cEffectFormulaManager * GetEffectFormulaManager() = 0;
	//得到【字段最终值】公式管理者
	virtual cFieldFormulaManager * GetFieldFormulaManager() = 0;	
	//得到【技能伤害  】公式管理者
	virtual cSkillFormulaManager * GetSkillFormulaManager() = 0;
	//设置LUA脚本的路径
	virtual BOOL SetLuaScriptPath(TCHAR * lpszPath) = 0;
	virtual void DumpMemory()
	{
		_CrtMemState s1;
		_CrtMemCheckpoint(&s1);
		OutputDebugString(_T("==========formula memory dump!\n"));
		_CrtMemDumpStatistics(&s1);
	}
};

typedef cFormulaManager * (FnGetFormulaManager)();
