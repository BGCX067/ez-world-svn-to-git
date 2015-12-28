#pragma once

class cLimit;
class cEffect;

// 一元操作 二元操作		
enum OperationType
{
	Operation_Unitary,	//一元操作	操作目标是自己
	Operation_Duality,	//二元操作	操作目标是其他个体(可以是包括自己的群体)
	Operation_Ternary,	//三元操作	通过某种个体去操作另外的个体
	Operation_MaxType,	//最多的操作元
};

// 单目标操作 范围操作
enum OperationDest
{
	Operation_Target,	//有明确目标的
	Operation_Range,	//只有目标范围的
};

// 范围类型
enum OperationRange
{
	Operation_Round,		// 圆形
	Operation_Hemicycle,	// 半圆
	Operation_Sector,		// 扇形
	Operation_Rectangle,	// 矩形
	Operation_RushRect,		// 前冲矩形
};

// 范围定义	2D空间
struct Range
{
	OperationRange eRange;
	union
	{
		// 圆/半圆
		DWORD r;
		// 矩形/前冲矩形
		struct
		{
			DWORD w,h;
		};
		// 扇形
		struct
		{
			DWORD r,angle;
		};
	};
};

#define MAX_OP_NAME		16


struct Operation
{
	char			m_szName[MAX_OP_NAME];			// 属性名
	OperationType	m_Type;
	OperationDest	m_Dest;
	Range			m_Range;						// 操作影响空间范围
	UINT			m_nClassID[Operation_MaxType];	// 操作元的类ID
	cLimit	*		m_pLimites;						// 操作元的限制
	cEffect	*		m_pEffects;						// 操作元的效果
};

class LOGIC_ENGINE cUnitObject;
class LOGIC_ENGINE cOperationManager
{
	Operation *		m_pOperations;
	UINT			m_nOperations;
	char			m_szFile[MAX_PATH];		
public:
	cOperationManager();
	~cOperationManager();

	BOOL Load(const char * lpctstrFile);
	BOOL Create(const char * lpctstrFile);
	const char * GetFile(){return m_szFile;}
	BOOL Op(UINT nID,cUnitObject * u1,cUnitObject ** u2,UINT n2,cUnitObject ** u3,UINT n3);
	Operation * GetOperation(UINT nID);
	Operation * GetOperation(const char * lpcstrOperation);
};