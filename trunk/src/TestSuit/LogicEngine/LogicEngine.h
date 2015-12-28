#pragma once

#if defined( LOGICENGINE_EXPORTS )
#	define LOGIC_ENGINE_API	extern "C" __declspec( dllexport )
#	define LOGIC_ENGINE	__declspec( dllexport )
#else
#	define LOGIC_ENGINE_API	extern "C" __declspec( dllimport )
#	define LOGIC_ENGINE	__declspec( dllimport )
#endif

#define MAX_NAME	32
#define MAX_DESC	256
#define USE_VECTOR_ALLOCATED_BLOCK
#include <map>
#include <vector>
#include <bitset>
#include <string>
#include <assert.h>
#include "Formula.h"
// 整个世界是一棵树。节点类型分为树节点和页节点。页节点存储的是真正的属性，树节点只是一个容器。
// 所有的树节点都用一个类实现。递归调用
// !!! 模板 !!!	1、无差异/有差异模板	同一实现。 需要说明差异可能存在的成分 Unit_Element。如无任何不同，则为无差异模板
// !!! 实例 !!!	2、无差异/有差异实例	无差异实例可以直接引用模板	有差异实例是否有引用模板的价值?
// 叶节点才需要实际的类? 叶节点是派生实际的类还是包含实际的类?

typedef std::vector<bool> VectorDirty;

// 世界节点类型
enum Node_Type
{
	Node_Root,				// 根节点
// 定义层
	Node_UnitManager,		//	基本属性			☆	网格
	Node_Relation,			//		关系定义	☆	网格

// 行为管理
	Node_Beharior,			//	行为管理表			☆	网格
	Node_Operate,			//	操作管理表			☆	网格
	Node_ContactManager,	//	联系管理表			☆	网格

	Node_UnitTree,			//	个体结构管理器
	Node_Unit,				//		个体结构
	Node_UnitAttrib,		//			数据属性	☆	网格
	Node_UnitTemplate,		//			数据模板	☆	网格
	Node_UnitLimite,		//			限制模板	☆	网格
	Node_UnitEffect,		//			影响模板	☆	网格
	Node_BuffEffect,		//			BUFF影响	☆	网格
	Node_UnitFsm,			//			状态机

	Node_ContactTree,		//	联系结构管理器
	Node_Contact,			//		联系结构
	Node_ContactAttrib,		//			数据属性	☆	网格
	Node_ContactData,		//			数据模板	☆	网格

	Node_UnitCalc,			//			数据计算		
//	Node_UnitFsm,			//			状态机
	Node_UnitOperation,		//			操作		☆	网格
	
// 实例层
	Node_Unit_IntanceMgr,	//	个体实例管理器
	Node_Unit_Intance,
	Node_Type_Max,
};

// 属性域性质
enum FieldFlag
{
	Field_Template,		// 需要由数据模板指定。
	Field_Instance,		// 每个实例都不同的。
	Field_Restored,		// 需要从保存的数据或者代表不同实例的数据中恢复。
	Field_FlagMax,
};

// 属性域类型
enum FieldType
{
	Field_String,
	Field_Enumerate,
	Field_Integer,
	Field_Boolean,
	Field_Percent,
	Field_Float,
	Field_IdField,
	Field_Set,
	Field_TypeMax,
	Field_Dynamic = Field_TypeMax,

	//Field_Dynamic
	//	存储,读取,输入,输出时 与 Field_Integer 完全相同.
	//	在编辑器里的设置与Field_IdField相同,但是由程序设置回调.
};

//////////////////////////////////////////////////////
//	单元定义 基本属性
//		行为		由玩家操作产生/由AI产生/无行为
//		容积/体积	可包含/不可包含	Unit_0D 不能包含
//		生命周期	
//////////////////////////////////////////////////////
enum UnitType
{
	Unit_Unit,		// 单元
	Unit_Base,		// 基类
//	Unit_Relating,	// 关联
	Unit_Type_Max,	// 单元类型最大值
};

enum UnitDim
{
	Unit_0D,
	Unit_1D,
	Unit_2D,
	Unit_3D,
	Unit_Dim_Max,
};


// 单元定义
struct UnitDef
{
	int m_nID;
	char m_szName[MAX_NAME];
	char m_szAlias[MAX_NAME];
	UnitType m_eType;
	int m_nBaseClass;
	UnitDim m_ePosDim;
	BOOL m_bUseAngle;
	UnitDim m_eSpaceDim;
	BOOL m_bHasTemplate;
	BOOL m_bHasLimit;
	BOOL m_bHasRelation;
	BOOL m_bHasBuff;
	BOOL m_bHasFsm;
};

// 单元属性的最大个数
const int MAX_UNIT_PROP_CNT = 512;

enum BehaviorDirection
{
	Player2NPC,
	NPC2Player,
	RD_Max,
};
enum BehaviorField
{
	Behavior_ID = 1,
	Behavior_Name,
	Behavior_Alias,
	Behavior_CalcID,
	Behavior_Para1,
	Behavior_Para1Type,
	Behavior_Para2,
	Behavior_Para2Type,
};
//行为定义
struct BehaviorDef
{
	int	m_nID;
	char m_szName[MAX_NAME];
	char m_szAlias[MAX_NAME];
	int m_nCalcID;
	char m_szPara1[MAX_NAME];
	FieldType m_ePara1;
	char m_szPara2[MAX_NAME];
	FieldType m_ePara2;
};

enum EffectPhase
{
	Phase_OrigValue,	//裸值
	Phase_PhaseValue,	//中间值
	Phase_CurrValue,	//当前值
	Phase_Max,
};

enum OperateField
{
	Operate_ID = 1,
	Operate_Name,
	Operate_Alias,
};

//操作定义
struct OperateDef
{
	int	m_nID;
	char m_szName[MAX_NAME];
	char m_szAlias[MAX_NAME];
};

//联系定义
struct ContactDef
{
	int	m_nID;
	char m_szName[MAX_NAME];
	char m_szAlias[MAX_NAME];
};

enum RelationType
{
	Relation_Attach,	// 物品附加
	Relation_Buff,		// 临时BUF
	Relation_Once,		// 即时生效
	Relation_Max,
};

struct Relation
{
	UINT			nID;
	char			szName[MAX_NAME];
	UINT			nDestUnitID;		//作用目标
	UINT			nOperUnitID;		//作用者
	UINT			nHandlerUnitID;		//使用者
	RelationType	eType;				//关系类型
};

// 限制/作用目标
enum DestElement
{
	Dest_Attrib,			// 数据属性
	Dest_Limit,				// 限制
	Dest_Effect,			// 影响

	Dest_Pos,				// 位置
	Dest_Space,				// 物品空间
	Dest_State,				// 状态
	Dest_Max,
};

enum Limit_Dest
{	
	Limit_Handler,	// 对使用者的限制
	Limit_DestUnit,	// 对做用目标的限制
	Limit_DestMax,
};


// 目前暂是只做并列 AND 的条件限制
// 限制类型
enum Limit_Op
{
	Limit_None = -1,	// 无操作
	Limit_GreaterEqual,	// 大于等于
	Limit_Greater,		// 大于
	Limit_Equal,		// 等于
	Limit_Lessqual,		// 小于等于
	Limit_Less,			// 小于
	Limit_NotEqual,		// 不等于
	Limit_NoFlag,		// 无标记
	Limit_HasFlag,		// 有标记
	Limit_Max,

	//Limit_And,		// 与
	//Limit_Or,			// 或
	//Limit_Not,		// 取反
};

enum Limit_DestType
{
	LimitType_Prop,		// 基本属性
	LimitType_Custom,	// 自定义
	LimitType_Max,
};

enum Limit_ValueType
{
	LimitValue_Constant,// 数值方式
	LimitValue_Precent, // 上限百分比
	LimitValue_Max,		
};

enum DestType
{
	DT_UnitId,
	DT_SocietyRelate,
	DT_Max,
};

// 空间位置限制	nIndex = 空间分量
// 数据属性限制 nIndex = 属性域索引
// 当前状态限制 nIndex = 状态机索引
struct LimitField
{
	int			nTemplateID;	// 模板ID
	int			nReleationID;	// 关系类型
	Limit_Dest	eDest;			// 限制目标
	Limit_DestType eDestType;	// 目标类型
	int			nDestField;		// 目标属性	
	Limit_Op	eOp;			// 限制操作
	int			nValue;			// 限制值
	// 20080603 Add
	UINT		nGroup;			// 组索引
	char		szName[256];	// 组说明
	char		szDestField[40];// 目标属性,名称索引20080815
	Limit_ValueType eValueType;	// 数值类型
};

enum Effect_Dest
{	
	Effect_Handler,	// 对使用者的限制
	Effect_DestUnit,	// 对做用目标的限制
	Effect_DestMax,
};

enum
{
	eValueAdd = 0,	// 数值加
	ePercentAdd,	// 百分比加
	eValueSub,		// 数值减
	ePercentSub,	// 百分比减
	eChangeState,	// 改变状态机
	eSetFlag,		// 设置标记
	eCover,			// 覆盖
	eZero,			// 清零
	eDataOpMax,
};

// 影响方式
enum EffectOp
{
	Effect_Add,				//数值迭加
	Effect_PersentAdd,		//百分比迭加
	Effect_FieldPersentAdd,	//以上限字段为基础的百分比迭加
	Effect_Replace,			//替换
	Effect_Prefix,			//字符串前缀
	Effect_Postfix,			//字符串后缀
	Effect_AddFlag,			//增加标记
	Effect_DelFlag,			//删除标记
	Effect_Max,
};


enum EDestUnit
{
	eDestUnitHandler,
	eDestUnitTarget,
	eDestMax
};

// 属性影响域
struct EffectValue
{
	int			nTemplateID;	// 模板名称ID
	int			nRelationID;	// 关系类型ID
	DestElement	eDest;			// 目标元素
	EDestUnit			nDestUnit;		// 目标单元
	int			nOperField;		// 目标属性
	int			eOp;			// 影响操作
	float		nValue;
	int			nActParam;		// 行为参数1
	EffectPhase ePhase;			// 影响阶段
	char		szOperField[40];// 目标属性,名称索引20080815
};

enum BuffEffectType
{
	BuffEffect_InTime,			// 及时生效
	BuffEffect_Persistence,		// 持续生效
	BuffEffect_Cycle,			// 周期生效
	BuffEffect_Max,
};

enum BuffEffectSpaceType
{
	BuffEffectSpace_None,
	BuffEffectSpace_Rect,
	BuffEffectSpace_Sector,
	BuffEffectSpace_FrontRect,
	BuffEffectSpace_Max,
};

enum BuffAddType
{
	BAT_None,					// 无法释放
	BAT_CanAdd,					// 可叠加
	BAT_TimeAdd,				// 延续
	BAT_Replace,				// 替换
	BAT_Quene,					// 排队
	BAT_Kill,					// 抵消
	BAT_Control,				// 抑制
	BAT_Post,					// 推后
	BAT_Max,
};

enum TimeRule
{
	TR_None,					// 无时间限制
	TR_Earlyer,					// 更早时间
	TR_Later,					// 更晚时间
	TR_Max,
};

enum LeverRule
{
	LR_None,					// 级别无关
	LR_SameLever,				// 同级
	LR_HigherLever,				// 高级
	LR_LowerLever,				// 低级
	LR_Max,
};

enum SourceRule
{
	SR_None,					// 无限制
	SR_SameInst,				// 同一实例
	SR_Max,
};

enum EffectValueType
{
	EVT_Constant,
	EVT_Script,
	EVT_State,
	EVT_Max,
};

enum EVParmType
{
	EVPT_Null,
	EVPT_Operater,
	EVPT_Dest,
	EVPT_Max,
};

enum EVParamDefType
{
	EVPDT_Reserve,
	EVPDT_Integer,
	EVPDT_Custom,
	EVPDT_Max,
};

enum BuffEffect_Field
{
	BEF_TemplateId = 1,
	BEF_RelateID,
	BEF_Dest,
	BEF_DestUnit,
	BEF_OperField,
	BEF_Op,
	BEF_EVType,
	BEF_Value,
	BEF_EVPT1,
	BEF_EVPT2,
	BEF_EVPT3,
	BEF_EVPT4,
	BEF_ActParam,
	BEF_Phase,
	BEF_InstId,
	BEF_OwerType,
	BEF_Lever,
	BEF_EffectType,
	BEF_PersisTime,
	BEF_CycleInterval,
	BEF_DelayTime,
	BEF_ActionTouch,
	BEF_SpaceType,
	BEF_SpaceParam1,
	BEF_SpaceParam2,
	BEF_IfMove,
	BEF_UseDesc,
	BEF_AddType,
	BEF_TimeRule,
	BEF_LeverRule,
	BEF_NumRule,
	BEF_SourceRule,
	BEF_Group,
	BEF_GroupDesc,
	BEF_RefField,
	BEF_IfBash,
	BEF_IfLucky,
	BEF_Icon,
	BEF_BuffDesc,
};

// BUFF影响域
struct BuffEffectValue
{
	int			nTemplateID;	// 模板名称ID
	int			nRelationID;	// 关系类型ID
	DestElement	eDest;			// 目标元素
	EDestUnit			nDestUnit;		// 目标单元。 0使用者 1作用目标
	int			nOperField;		// 目标属性	
	int			eOp;			// 影响操作
	EffectValueType eEvType;	// 影响值类型
	int			nValue;		// 影响值	
	int			nParam1;		// 影响值参数1
	int			nParam2;		// 影响值参数2
	int			nParam3;		// 影响值参数3
	int			nParam4;		// 影响值参数4
	int			nActParam;		// 行为参数1
	EffectPhase ePhase;			// 影响阶段

	int			nInstID;		// 释放者实例ID	
	int			nOwerTypeID;	// 自身类型
	int			nLever;			// 等级

	BuffEffectType eBuffEffectType;	// 作用类型
	int			nPersistenceTime;	// 持续时间
	int			nCycleInterval;	// 周期间隔
	int			nDelayTime;		// 延迟时间

	int			nActionTouch;	// 触发条件
	BuffEffectSpaceType eSpaceType;	// 空间类型
	int			nSpaceParam1;	// 空间参数1
	int			nSpaceParam2;	// 空间参数2

	BOOL		bIfMove;		// 是否跟随移动
	BOOL		bUseDesc;		// 是否使用递减表

	BuffAddType	eAddType;		// 叠加方式
	TimeRule	eTimeRule;		// 时间规则
	LeverRule	eLeverRule;		// 级别规则
	int			nNumRule;		// 数量规则
	SourceRule	eSourceRule;	// 来源规则
	UINT		nGroup;			// 组索引
	char		szName[256];	// 组说明
	char		szRefField[40];	// 目标属性参照  参照说明20080815
	BOOL		bIfBash;		// 是否重击
	BOOL		bIf2Enemy;		// 是否对敌
	BOOL		bIfLucky;		// 是否幸运一击
	int			nIcon;			// buff图标
	CHAR		szBuffDesc[128];// buff说明
};

//////////////////////////////////////////////////////
//	单元定义 数据定义
// 1、最大值或者最小值的限制不是具体值而是其他字段
// 2、让使用者定义最终类？还是允许使用者从已定义的类中再继承？暂时定为最终类。
// 3、可视数据结构定义 玩家可见数据结构定义
//////////////////////////////////////////////////////

// todo:字符串类型的,可能来源于某个叶节点的某一列.
// todo:整个域的说明,

enum LeafNodeField
{
	LeafNode_Name = 1,	
	LeafNode_Alias,
	LeafNode_Template,
	LeafNode_Instance,
	LeafNode_Restore,
	LeafNode_Type,
	LeafNode_Min,
	LeafNode_Max,
	LeafNode_Length,
	LeafNode_Index,
	LeafNode_Limit,
	LeafNode_IsDeriveField,
	LeafNode_RefField,
	LeafNode_Desc,
};

enum ContactDataField
{
	ContactData_Name = 1,	
	ContactData_Alias,
	ContactData_Type,
	ContactData_Min,
	ContactData_Max,
	ContactData_Length,
	ContactData_Index,
};

enum LimitNodeField
{
	LimitNode_TempName = 1,
	LimitNode_RelateType,
	LimitNode_LimitUnit,
	LimitNode_DestType,
	LimitNode_DestField,
	LimitNode_LimitOp,
	LimitNode_LimitValue,
	LimitNode_GrpIndex,
	LimitNode_GrpDesc,
	LimitNode_RefField,
};

struct FieldId
{
	char * pLeafPath;
	int nDispField;
	int nValField;	// -1表示不是字段，是索引。
};

struct FieldEnum
{
	UINT nNum;
	char * pString;
};

#define FIELD_TEMPLATE	1
#define FIELD_INSTANCE	(1<<1)
#define FIELD_RESTORED	(1<<2)

// 既要描述单元的数据属性.也要描述其他表格的字段.
struct LeafField
{
	// 基本字段属性
	char *			m_lpszName;				// 属性名
	FieldType		m_eType;				// 属性域类型
	int				m_nMin;					// 最大值
	int				m_nMax;					// 最小值
	int				m_nLength;				// 字符串长度/ 枚举数量
	
	union
	{
		char **	m_ppStrings;			// 枚举/布尔字符串
		FieldId *	m_pFieldId;				// 字段ID映射
	};
	int				m_nKeyOrder;			// 索引键编号 0不是索引键
	int				m_nLimitField;			// 限制自己的字段。(0为无，使用时减1)
	int				m_nLimitDest;			// 自己限制的字段。(0为无，使用时减1)
	
	// 单元字段属性
	char *			m_lpszAlias;			// 英文别名
	DWORD			m_dwFlags;				// 数据域属性
	int				m_nInstanceOffset;		// 在实例缓存中的偏移量
	int				m_nTemplateOffset;		// 在模板缓存中的偏移量

	int				m_bIsDeriveField;		// 是否是衍生字段
	char *			m_lpszRefField;			// 关联的属性

};

// 联系的属性定义
struct ContactAttrib
{
	// 联系字段属性
	char			m_lpszName[MAX_NAME];	// 字段名
	char			m_lpszAlias[MAX_NAME];	// 字段别名
	FieldType		m_eType;				// 属性域类型
	int				m_nMin;					// 最大值
	int				m_nMax;					// 最小值
	int				m_nLength;				// 字符串长度/ 枚举数量
	int				m_nKeyOrder;			// 索引键编号 0不是索引键
};

struct InstValue
{
	void * m_pInst;
	void * m_pTemplate;
};

class cNodeDesc
{
public:
	virtual ~cNodeDesc(){}
	virtual Node_Type  GetType() const = 0;						// 节点类型
	virtual const char * GetDescName() const = 0;				// 节点描述名字
	virtual BOOL IsLeaf() const = 0;							// 是否叶节点
	virtual BOOL IsFiled() const = 0;							// 是否保存到文件
	virtual BOOL IfExclusive() const = 0;						// 此节点类型在父节点是否唯一
};
class cTreeNodeDesc : public cNodeDesc
{
public:
	virtual ~cTreeNodeDesc(){}
	virtual UINT GetChildTypeAllowedCount() const = 0;					// 允许包含的节点类型数量
	virtual const cNodeDesc * GetChildTypeAllowed(UINT n) const = 0;	// 允许包含的节点类型
	virtual BOOL IfChildTypeAllowed(Node_Type type) const = 0;			// 是否允许包含指定的节点类型
};
class cLeafNodeDesc : public cNodeDesc
{
public:
	virtual ~cLeafNodeDesc(){}
	virtual UINT GetFieldCount() const = 0;						// 属性域的数量。可扩展的网格，数量为 0xffffffff
	virtual const LeafField * GetField(UINT n) const = 0;	
	virtual UINT FindField(const char * lpszField) const = 0;
	virtual UINT FindFieldByAlias(const char * lpszFieldAlias) const = 0;
	virtual UINT GetRowLength() const = 0;
	virtual UINT GetInstLength() const = 0;
	virtual UINT GetBaseFields() const = 0;						// 取得基类字段数(不可改变的)
	virtual UINT GetIDField() const = 0;						// 取得特别加速的名为"ID"的字段的索引
};

struct NodeTable
{
	cNodeDesc * pNodeDesc;
	char *	lpszNodeName;
	char *	lpszNodeAlias;
};

class cBaseNode
{
public:
	virtual ~cBaseNode(){}
	virtual const cNodeDesc * GetDesc() const = 0;								// 节点性质描述
	virtual const char * GetName() const = 0;									// 节点名字
	virtual const char * GetAlias() const = 0;									// 节点名字
	virtual const char * GetPath(char * lpszBuf,size_t size) const = 0;		// 取得自己的路径
	virtual const cBaseNode * GetNodeByPath(const char * lpszPath) const = 0;	// 取得指定路径的节点
	virtual const cBaseNode * GetParent() const = 0;							// 取得父节点
};

class cUnit;
class cTreeNode : public cBaseNode
{
public:
	virtual ~cTreeNode(){}
	virtual UINT GetChildren() const = 0;							// 子节点数量
	virtual cBaseNode * GetChildNode(UINT n) const = 0;				// 取得子节点
	virtual BOOL AddChildNode(cBaseNode * pNode) = 0;				// 填加子节点
	virtual cBaseNode * NewChildNode(Node_Type type,char * lpszName,char * lpszAlias) = 0;	// 新建子节点
	virtual BOOL DelChildNode(cBaseNode * pNode) = 0;				// 删除子节点
	virtual const cUnit * GetUnit() const = 0;
};

struct FieldValue
{
	char * fv;
	union
	{
		int * n;
		float * f;
		char * s;
	};
};

class cLeafRow
{
public:
	virtual ~cLeafRow(){}
	virtual UINT GetColumns() const = 0;										// 取得列数 属性表横为列。
	virtual UINT FindField(const char * lpszField) const = 0;

	virtual const char * GetString(UINT col) const= 0;							// 取得指定位置的值
	virtual int GetInteger(UINT col) const = 0;									// 取得指定位置的值
	virtual float GetFloat(UINT col) const = 0;									// 取得指定位置的值
	virtual void SetString(UINT col,const char * string) = 0;					// 设置指定位置的值
	virtual void SetInteger(UINT col,int nValue) = 0;							// 设置指定位置的值
	virtual void SetFloat(UINT col,float fValue) = 0;							// 设置指定位置的值

	virtual const char * GetString(const char * lpszField) const = 0;			// 取得指定位置的值
	virtual int GetInteger(const char * lpszField) const = 0;					// 取得指定位置的值
	virtual float GetFloat(const char * lpszField) const = 0;					// 取得指定位置的值
	virtual void SetString(const char * lpszField,const char * string) = 0;	// 设置指定位置的值
	virtual void SetInteger(const char * lpszField,int nValue) = 0;			// 设置指定位置的值
	virtual void SetFloat(const char * lpszField,float fValue) = 0;			// 设置指定位置的值

	virtual const void * GetBuffer() const = 0;									// 取得数值缓存

	virtual UINT GetLimites() const = 0;										// 取得限制数量
	virtual const LimitField ** GetLimitePtr()const = 0;						// 取得限制指针
	virtual UINT GetEffects()const = 0;											// 取得效果数量
	virtual const EffectValue ** GetEffectPtr()const = 0;						// 取得效果指针
	virtual UINT GetBuffs()const = 0;											// 取得BUFF数量
	virtual const BuffEffectValue ** GetBuffPtr()const = 0;						// 取得BUFF指针
	int GetBuffLine(const BuffEffectValue * pbufeffvalue)const
	{
		for(int i=0;i<(int)GetBuffs();i++)
		{
			if(GetBuffPtr()[i]==pbufeffvalue)
				return i;
		}
		return -1;
	}
};

#define MAX_INDEX_FIELD	16

class cQuery
{
	UINT nCount;
	int nValues[MAX_INDEX_FIELD];
public:	
	cQuery()
	{
		nCount = 0;
	}
	cQuery(int value)
	{
		nCount = 1;
		nValues[0] = value;
	}
	cQuery(int value1,int value2)
	{
		nCount = 2;
		nValues[0] = value1;
		nValues[1] = value2;
	}
	cQuery(int value1,int value2,int value3)
	{
		nCount = 3;
		nValues[0] = value1;
		nValues[1] = value2;
		nValues[2] = value3;
	}
	~cQuery()
	{
		nCount = 0;
	}

	void Push(int value)
	{
		if(nCount < (MAX_INDEX_FIELD-1))
		{
			nValues[nCount] = value;
			nCount ++;
		}
	}

	UINT GetCount()
	{
		return nCount;
	}
	int GetValue(UINT n)
	{
		return n<MAX_INDEX_FIELD?nValues[n]:-1;
	}
};

struct ResultRows
{
	UINT nKey;		// 如果
	UINT nFirst;
	UINT nCount;
};

struct Font
{
	std::string	szFontName;
	INT		nFontSize;
	DWORD	dwForeColor;
	DWORD	dwBackColor;
	bool	bFontBold;
	bool	bFontItalic;
	bool	bFontUnderline;
	bool	bFontStrikethru;
};

struct FiledFormat
{
	Font	font;
	UINT	nID;			//ID=0 表示的是表格格式

	BOOL isEquie(struct FiledFormat & other)
	{
		if(font.dwBackColor==other.font.dwBackColor
			&& font.nFontSize==other.font.nFontSize
			&& font.dwForeColor==other.font.dwForeColor
			&& font.bFontBold==other.font.bFontBold
			&& font.bFontItalic==other.font.bFontItalic
			&& font.bFontUnderline==other.font.bFontUnderline
			&& font.bFontStrikethru==other.font.bFontStrikethru
			&& !strcmp(font.szFontName.c_str(),other.font.szFontName.c_str()))
		{
			return TRUE;
		}
		return FALSE;
	}
};


class cLeafNode : public cBaseNode
{
public:
	virtual ~cLeafNode(){}
	virtual UINT GetRows() const = 0;													// 取得行数 属性表为1行
	virtual UINT GetColumns() const = 0;												// 取得列数 属性表横为列。

	virtual const char * GetString(UINT row,UINT col) const = 0;						// 取得指定位置的值
	virtual int GetInteger(UINT row,UINT col) const = 0;								// 取得指定位置的值
	virtual float GetFloat(UINT row,UINT col) const = 0;								// 取得指定位置的值
	virtual void SetString(UINT row,UINT col,const char * string) = 0;					// 设置指定位置的值
	virtual void SetInteger(UINT row,UINT col,int nValue) = 0;							// 设置指定位置的值
	virtual void SetFloat(UINT row,UINT col,float fValue) = 0;							// 设置指定位置的值

	virtual const char * GetString(UINT row,const char * lpszField) const = 0;		// 取得指定位置的值
	virtual int GetInteger(UINT row,const char * lpszField) const = 0;					// 取得指定位置的值
	virtual float GetFloat(UINT row,const char * lpszField) const = 0;					// 取得指定位置的值
	virtual void SetString(UINT row,const char * lpszField,const char * string) = 0;	// 设置指定位置的值
	virtual void SetInteger(UINT row,const char * lpszField,int nValue) = 0;			// 设置指定位置的值
	virtual void SetFloat(UINT row,const char * lpszField,float fValue) = 0;			// 设置指定位置的值

	virtual HRESULT ImportFromExcel(const char * lpszFile) = 0;
	virtual HRESULT ExportToExcel(const char * lpszFile) const = 0;
	virtual HRESULT ExportNode(const char * lpszFile,int nFields,int * pnFields) const = 0;
	virtual BOOL SetEnumString(UINT nRow,const char * lpszEnum,UINT nNum) = 0;		// 设置数据属性叶节点的枚举字符串
	virtual const char * GetEnumString(UINT nRow,UINT& nNum) const = 0;			// 取得数据属性叶节点的枚举字符串
	virtual void SetFieldID(UINT nRow,FieldId * pFieldId)= 0;						// 设置数据属性叶节点的属性域对应
	virtual void GetFieldID(UINT nRow,FieldId * pFieldId) const = 0;				// 取得数据属性叶节点的属性域对应

	virtual UINT FindRow(UINT nField,int nValue) const = 0;
	virtual UINT FindRow(const char * lpcstrField,int nValue) const = 0;
	virtual UINT FindRow(UINT nField,const char * lpcstrValue) const = 0;
	virtual UINT FindRow(const char * lpcstrField,const char * lpcstrValue)const  = 0;
	virtual UINT FindRowByID(int nValue) const = 0;

	virtual void BuildIndex() = 0;								// 重新建立索引	
	virtual UINT GetIndices() const = 0;						// 取得索引数量
	virtual ResultRows Query(cQuery q,BOOL bLastResult = FALSE) const = 0;				// 根据索引寻找行号
	virtual const cLeafRow * Result(ResultRows * r,UINT n) const = 0;	// 根据结果取得行
	virtual const cLeafRow * GetRow(UINT nRow) const = 0;		// 根据行号取得行
	const void *GetRowBuffer(UINT nRow)
	{
		assert(nRow>=0);
		return GetRow(nRow)->GetBuffer();
	}
	virtual void ValidRow(UINT nRow,BOOL bAllocMore)=0;			// 改变当前行数
	virtual void SetRows(UINT nRow) = 0;						// 设置行数

	virtual void OnAttribDefChanged() = 0;						// 定义改变时倒腾数据
	virtual UINT GetBaseRows() const = 0;						// 取得基类行数(不可改变的)

	virtual BOOL LoadLeafGridFormat(char *szPath) = 0;			//加载网格的格式(含说明字段)
	virtual BOOL SaveLeafGridFormat(char *szPath) = 0;			//保存网格的格式(含说明字段)
	virtual std::map<UINT,struct FiledFormat>& GetFieldFormat() = 0;
	virtual std::map<UINT,std::string>& GetRowDesc() = 0;
	virtual std::map<UINT,UINT>& GetFieldType() = 0;
	virtual std::map<UINT,DWORD>& GetColWidth() = 0;
	virtual std::map<UINT,DWORD>& GetRowHeight() = 0;
	virtual void SetFieldFormat(std::map<UINT,struct FiledFormat>&) = 0;
	virtual void SetRowDesc(std::map<UINT,std::string>&) = 0;
	virtual void SetFieldType(std::map<UINT,UINT>&) = 0;
	virtual void SetColWidth(std::map<UINT,DWORD>&) = 0;
	virtual void SetRowHeight(std::map<UINT,DWORD>&) = 0;

	virtual void* GetRefNode() = 0;

	//////////////////////20080625公式依赖问题////////////////////////////////
	virtual BOOL IsDeriveField(UINT nRow) = 0;
	virtual std::string& GetDependField(UINT nRow) = 0;
	virtual void SetDependField(UINT nRow,std::string & strValue) = 0;
	virtual void RemoveDeriveField(UINT nRow) = 0;
	//////////////////////20080625公式依赖问题////////////////////////////////
};

enum FsmState
{
	Fsm_ID = 1,
	Fsm_Name,
	Fsm_Temp,
	Fsm_RemainTime,
	Fsm_RestoreTo,
	Fsm_Entry,
	Fsm_Outry,
};

struct stState
{
	UINT nID;
	char szName[MAX_NAME];
	BOOL bTemp;
	DWORD dwRemainTime;
	UINT nRestoreTo;
	char szEntry[MAX_DESC];
	char szOutry[MAX_DESC];
};

struct stDictate
{
	UINT nID;
	char szName[MAX_NAME];
	UINT nSourceID;
	UINT nDestID;
};

#define MAX_STATE	32
class cFsmNode
{
public:
	virtual ~cFsmNode(){}
	//状态
	virtual	UINT GetStateCount() = 0;
	virtual const stState * GetState(UINT nStateID) = 0;
	virtual void AddState(UINT nSequence,const stState * pState) = 0;
	virtual void DelState(UINT nStateID) = 0;
	virtual UINT GetCurrentStateID() = 0;
	virtual const stState * GetCurrentState() = 0;
	virtual const stState * GetFirstState() = 0;
	virtual const stState * GetNextState(UINT nStateID) = 0;

	//状态转换
	virtual UINT SwitchState(const UINT nState) = 0;

	// transition
		
};
class cInstNode;
enum StateChangeType
{
	eSct_TimeOut,
	eSct_Force,
	eSct_Max,
};
class cState
{
public:
	virtual ~cState(){}
	virtual UINT GetDefaultState() = 0;
	virtual UINT ChangeState(cInstNode *pInst,UINT nFromState,UINT nToState,StateChangeType eType=eSct_Force) = 0;
};
class cFieldEvent
{
public:
	virtual void Event(cInstNode * pInstNode,int nCol)const = 0;
};

class cUnit
{
public:
	virtual ~cUnit(){}
	virtual const cTreeNode * GetTreeNode() const = 0;
	virtual const UnitDef * GetUnitDef() const  = 0;						// 取得自己的关联单元声明
	virtual UINT GetRelations() const  = 0;
	virtual const Relation * GetRelation(UINT n) const  = 0;
	virtual void RefreshUnitTreeNode() = 0;	
	virtual const cLeafNode * GetLeaf(Node_Type type) const  = 0;
	virtual void SetFieldEvent(cFieldEvent * pEven,UINT nCol)  = 0;
	virtual const cFieldEvent * GetFieldEvent(UINT nCol) const  = 0;
	virtual const cLeafNodeDesc * GetAttribDesc() const  = 0;
};

class cContact
{
public:
	virtual ~cContact(){}
	virtual const cTreeNode * GetTreeNode() const = 0;
	virtual const ContactDef * GetContactDef() const  = 0;						// 取得自己的关联联系声明
	virtual void RefreshContactTreeNode() = 0;	
	virtual const cLeafNode * GetLeaf(Node_Type type) const  = 0;
	virtual const cLeafNodeDesc * GetAttribDesc() const  = 0;
};

struct Attachment
{
	int nRelationID;		// 关系ID
	int nInstOperID;		// 作用者实例ID
	int nInstHandID;		// 使用者实例ID
};

struct EffectCold
{
	int nUnitID;
	int nTemplateID;
	int nSerialID;
	int nSerialLevel;
	DWORD dwColdTime;
	DWORD dwColdRemain;
};

struct InstEffect
{
	int nUnitID;
	int nTemplateID;
	int nSerialID;
	int nSerialLevel;
	const cLeafRow * pRow;
	DWORD dwEffectTime;
	DWORD dwEffectRemain;
};

enum BufferCasterType
{
	eBct_NPC	= 0,
	eBct_Player	= 1,
	eBct_Prop	= 2,
	eBct_Brother= 3,
	eBct_Pet= 4,
	eBct_Max,
};

struct BuffEffect;
class CBuffEvent
{
public:
	virtual void OnBuffErased(const BuffEffect *pBuffer)=0;
};
///////////////////////技能BUFF专用///////////////////////
///< 2009-05-21.m_eCasterType,m_nCasterID目前只是在非立即生效,
///<	且有害buffer下记录释放者信息,其他情况下暂可不使用
///<
struct BuffEffect
{
	DWORD				m_dwEffectTime;		// 作用时间
	DWORD				m_dwRemainTime;		// 剩余时间
	DWORD				m_dwBeginTime;		// BUFF开始生效时间
	DWORD				m_dwLastUpdateTime;	// 最后更新时间
	BOOL				m_bEnable;			// 是否生效
	float				m_nLastValue;		// 生效之前的值
	float				m_nFactor;			// 重击倍率
	float				m_nValue;			// 影响值
	const				BuffEffectValue* 	m_pValue;			// BUFF影响数据

	UINT				m_nInstID;			// BUFF产生实例的ID 用于判断buff是否可以叠加以及传递给脚本参数
	UINT				m_nSkillInstID;		// 技能实例ID		uncostbuff判决时候是要删除的buff
	CBuffEvent			*m_pBuffEvent;


	//以下为上层逻辑使用看看能不能归并起来
	UINT				m_nSkillID;			// 技能模板ID
	BufferCasterType	m_eCasterType;		// 参照BufferCasterType(0:NPC 1:PLAYER)
	UINT				m_nCasterID;		// if m_nCasterType=1 then m_nCasterID=[DBID of caster] else m_nCasterID=[charid]
	UINT				icostid;			//cost到哪个实例上了


};

///////////////////////技能BUFF专用///////////////////////

class cRemain
{
public:
	virtual void GetEffectTime(const cInstNode * pInstNode,InstEffect * pIE) = 0;
	virtual void GetColdTime(const cInstNode * pInstNode,EffectCold * pEC) = 0;
	virtual void OnEffectTime(const cInstNode * pInstNode,InstEffect * pIE,DWORD dwRemain,DWORD dwCurrent) = 0;
	virtual void OnColdTime(const cInstNode * pInstNode,EffectCold * pEC,DWORD dwRemain,DWORD dwCurrent) = 0;
	virtual void OnEffectEnd(const cInstNode * pInstNode,int nUnitID,int nTemplateID) = 0;

	///////////////////////技能BUFF专用///////////////////////
	/* 以下内容主要为了服务端更新客户端buff图标剩余时间所用 */
	virtual void OnBuffBegin(const cInstNode * pInstNode,BuffEffect **ppBuff) = 0;
	virtual void OnBuffTime(const cInstNode * pInstNode,BuffEffect **ppBuff,DWORD dwRemain,DWORD dwCurrent) = 0;
	virtual void OnBuffTimeUpt(const cInstNode * pInstNode,BuffEffect **ppBuff) = 0;
	virtual void OnBuffEnd(const cInstNode * pInstNode,BuffEffect **ppBuff) = 0;
	///////////////////////技能BUFF专用///////////////////////
	
};
class cWorld;
class cInstNode;
typedef BOOL (CALLBACK *pFncGetInstsRelate)(const cInstNode *pThis,const cInstNode *pThat,int nRelate);

class cInstPropListener
{
public:
	virtual void FncPropChange(const cInstNode *pInst,int nProp) = 0;
};

enum FieldDirty
{
	eFd_Yes = 0,
	eFd_No,
};

class cBuffEffectFilter
{
public:
	virtual BOOL IfAccordWith(BuffEffect * pBuff)const = 0;
};

#define MAX_BUFFER_EFFECTS	56

class cInstNode
{
public:
	virtual ~cInstNode(){}

	virtual const cUnit * GetUnit() const = 0;							// 取得单元节点
	virtual const cLeafRow * GetRow() const = 0;						// 取得单元数据模板行
	virtual const void * GetBuffer() const= 0;							// 取得实例缓存
	virtual int GetID()const = 0;										// 取得实例ID
	virtual int GetTemplateID()const = 0;								// 取得模板ID
	virtual int GetUnitID()const = 0;									// 取得单元节点ID
	virtual const void * GetRowBuffer() const= 0;						// 取得单元数据模板行缓存
	virtual const cLeafNodeDesc * GetDesc() const= 0;					// 取得单元数据描述

	virtual const char * GetString(UINT col,BOOL bEffect = TRUE)const = 0;					// 取得指定位置的值
	virtual int GetInteger(UINT col,BOOL bEffect = TRUE)const = 0;							// 取得指定位置的值
	virtual float GetFloat(UINT col,BOOL bEffect = TRUE)const = 0;							// 取得指定位置的值
	virtual const char * GetString(const char * lpszField,BOOL bEffect = TRUE)const = 0;	// 取得指定位置的值
	virtual int GetInteger(const char * lpszField,BOOL bEffect = TRUE)const = 0;			// 取得指定位置的值
	virtual float GetFloat(const char * lpszField,BOOL bEffect = TRUE)const = 0;			// 取得指定位置的值

	virtual void SetString(UINT col,const char * string) = 0;					// 设置指定位置的值
	virtual void SetInteger(UINT col,int nValue,bool blimitfunction=true) = 0;							// 设置指定位置的值
	virtual void SetFloat(UINT col,float fValue,bool blimitfunction=true) = 0;							// 设置指定位置的值
	virtual void SetString(const char * lpszField,const char * string) = 0;	// 设置指定位置的值
	virtual void SetInteger(const char * lpszField,int nValue) = 0;			// 设置指定位置的值
	virtual void SetFloat(const char * lpszField,float fValue) = 0;			// 设置指定位置的值

	virtual UINT GetEffects()const = 0;
	virtual const EffectValue * GetEffectsPtr()const = 0;
	virtual void ApplyEffectToEffect(EffectValue * pEffect) = 0;
	virtual void CancelInstEffect(EffectValue * pEffect) = 0;
	virtual void ConvertBufferToEffect(EffectValue * pEffect,BuffEffectValue * pBuff,int nParam) = 0;

	virtual UINT GetAttchments()const = 0;							// 取得关联实例数量
	virtual const Attachment * GetAttchment(UINT n)const = 0;		// 取得关联实例
	virtual const Attachment * GetParent()const = 0;				// 取得父关联
	virtual int Attach(const Attachment * attach) = 0;				// 把指定实例关联到本节点
	virtual void Detach(const Attachment * attach) = 0;				// 把指定实例关联到本节点
	virtual bool IsOnAttach(const Attachment * attach) = 0;

	virtual UINT GetTempEffects()const = 0;							// 取得临时效果数量
	virtual const InstEffect * GetTempEffect(UINT n)const = 0;		// 取得临时效果
	//virtual int CostTemp(const Attachment * attach,BOOL bColdTime = TRUE) = 0;		// 使用指定实例作用到本节点(临时作用)
	//virtual int CostForever(const Attachment * attach,BOOL bColdTime = TRUE) = 0;	// 使用指定实例作用到本节点(永久作用)

	///////////////////////技能BUFF专用///////////////////////
	virtual BuffEffect * CostBuff(BuffEffect * pBuff) = 0;		// 技能BUFF作用到本实例
	virtual int TransferBuffer(cInstNode *pThatInst) = 0;		// 将自身的BUFFER转加给指定实例
	virtual void UncostBuff(UINT nSkillInstID,const BuffEffectValue * pBuffVaue) = 0;	// 解除做用到本实例的BUF
	virtual void ClearBuffs(int nMaxCount,cBuffEffectFilter * pFilter) = 0;		// 清除附加在实例上的符合条件的BUFF
	virtual void ClearAllBuff() = 0;											// 清除附加在实例上的所有BUFF

	virtual const void * GetCurrentValueBuff() = 0;				// 得到当前实例当前值的缓存
	virtual void ClearDirtyField(UINT nField=0) = 0;			// 清除更新标志，默认全部清除
	virtual VectorDirty &QueryDirtyField() = 0;
	virtual void CalculateAll(bool blimitfunction) = 0;
	///////////////////////技能BUFF专用///////////////////////

	virtual int LimiteAllow(const Attachment * attach) const = 0;
	virtual int HandleAllow(int nRelationID,cInstNode * pHandler)const = 0;
	virtual BOOL ColdAllow(int nRelationID,cInstNode * pOper)const = 0;	

	virtual void Update(DWORD dwCurTime) = 0;						// 更新实例

	// 1.从cUnit中，得到此单元可能的联系ID和联系名。
	// 2.从联系中，可以得到作用单元，使用者单元的单元ID和名。
	virtual UINT GetLimiteDestFeilds(int nRelationID,Limit_Dest eDest,LimitField ** ppLimits) const = 0;	// 3.获得每个联系对目标单元，使用单元的限制
	virtual UINT GetEffectDestFeilds(int nRelationID,Effect_Dest eDest,EffectValue ** ppEffets) const = 0;
	virtual UINT GetInstEffectFeilds(EffectValue ** ppEffets) const = 0;
	virtual UINT GetEffectDestFeilds(int nRelationID,EffectValue ** ppEffets,UINT * pnCount = NULL) const = 0;		// 4.获得每个联系目标单元的加成
	virtual UINT GetEffectBuffFeilds(EDestUnit nDestUnit,BuffEffectValue ** ppEffets,UINT * pnCount = NULL) const = 0;		// 5.获得每个联系目标单元的BUFF加成
	virtual UINT GetBufferEffects(BuffEffect *ppBuffers,UINT *pnCount = NULL) const = 0;	// 获得其他实例附加的BUFFER影响
	virtual LimitField * GetLimiteValue(int nRelationID,int nFields) const = 0;								// 获得指定联系对限制目标域的限制值

	virtual const LimitField * GetLimiteFeild(int nIndex) const = 0;

	virtual BOOL IfUpdate()const = 0;
	virtual void Update(BOOL bUpdate) = 0;

	virtual void SetUserData(void * pUserData) = 0;
	virtual void * GetUserData() const = 0;
	virtual void SetInstName(char * szName) = 0;
	virtual char* GetInstName() = 0;
	virtual bool  GetUsedState() const = 0;
	//virtual void SetFormulaManager(void * pFormulaManager) = 0;
	virtual BOOL IfInheritTheUnit(UINT nUnitID) const = 0;								// 是否继承于该单元
	virtual BOOL GetRelate2Other(const cInstNode *pOther,UINT nRelate) const = 0;		// 是否和指定的实例有该种关系
	virtual VOID RegistPropListen(cInstPropListener *pListener) = 0;					// 设置属性变动监听
	virtual VOID RegistStateListen(cState *pState) = 0;									// 状态管理

};

#define IEXCEL_NOFILE	(-1)	// 文件不存在
#define IEXCEL_NOCOL	(-2)	// 没有列数据
#define IEXCEL_NOROW	(-3)	// 没有行数据
#define EEXCEL_FILEERR	(-1)	// 文件不能创建

class cWorld
{
public:
	virtual cBaseNode * LoadFromFile(const char * lpszFile,cBaseNode * pParent) = 0;		// 从文件装载
	virtual BOOL SaveToFile(const char * lpszFile,cBaseNode * pNode,BOOL bBin = FALSE) = 0;// 保存到文件
	virtual BOOL LoadInstInfoFromFile(const char * lpszFile) = 0;							// 从文件加载实例
	virtual BOOL SaveInstInfoToFile(const char * lpszFile) = 0;							// 保存实例到文件
	virtual cBaseNode * CreateWorld() = 0;													// 创建一个新世界定义
	virtual void DestroyWorld(cBaseNode ** ppRoot) = 0;										// 释放世界定义
	virtual void Update(DWORD dwCurTime) = 0;												// 更新
	
	virtual void RefreshUnitTree() = 0;										// 单元管理表变化时，单元数据树刷新
	virtual void RefreshTemplate(cBaseNode * pAttrib) = 0;					// 单元属性定义改变时，单元数据模板刷新(继承类会跟着被刷新)
	virtual void RefreshContactTree() = 0;									// 联系管理表变化时，单元联系树刷新
	virtual void RefreshContact(cBaseNode * pAttrib) = 0;

	virtual cLeafNode * GetUnitTemplate(int nID) = 0;						// 用单元ID取得单元的数据模板	
	virtual cLeafNode * GetUnitTemplate(const char * lpszUnit) = 0;		// 用单元名取得单元的数据模板
	virtual cLeafNode * GetUnitAttrib(int nID) = 0;							// 用单元ID取得单元的属性定义
	virtual cLeafNode * GetUnitAttrib(const char * lpszUnit) = 0;			// 用单元名取得单元的属性定义
	virtual cLeafNode * GetUnitBuffer(int nID) = 0;							// 用单元ID取得单元的buffer定义
	virtual cLeafNode * GetUnitBuffer(const char * lpszUnit) = 0;			// 用单元名取得单元的buffer定义
	virtual cLeafNode * GetContactTemplate(int nID) = 0;					// 用联系ID取得联系的数据模板	
	virtual cLeafNode * GetContactTemplate(const char * lpszUnit) = 0;		// 用联系名取得联系的数据模板
	virtual cLeafNode * GetContactAttrib(int nID) = 0;						// 用联系ID取得联系的属性定义
	virtual cLeafNode * GetContactAttrib(const char * lpszUnit) = 0;		// 用联系名取得联系的属性定义
	virtual cLeafNode * GetUnitManager() = 0;								// 取得单元管理叶节点
	virtual cLeafNode * GetRelation() = 0;									// 取得单元关系叶节点
	virtual cBaseNode * GetRoot() = 0;										// 取得根节点
	virtual cLeafNode * GetContactManager() = 0;							// 取得联系管理叶节点
	virtual void * GetFormulaCenter() = 0;									// 取得公式管理
	
	virtual int EnumLeafNode(cLeafNode ** ppNode) = 0;						// 枚举世界定义中的所有叶节点

	virtual UINT GetUnits() const = 0;										// 取得当前世界定义中单元的数量
	virtual const cUnit * GetUnit(UINT n) const = 0;						// 取得当前世界定义中单元
	virtual const cUnit * GetUnit(int nID) const = 0;						// 用单元ID取得单元
	virtual const cUnit * GetUnit(const char * lpszUnit) const = 0;		// 用单元ID取得单元
	virtual BOOL IfDeriveFrom(const cUnit * pSrc,const cUnit * pDst)const = 0;			// 判断一个单元是否继承自另外一个单元(可能是间接继承)

	virtual cInstNode * CreateInstance(										// 创建单元的实例节点
				const cUnit * pUnit,	// 单元指针							
				int nTemplateID,			// 数据模板ID
				int nInstanceID =-1 ) = 0;	// 实例ID
	
	cInstNode * CreateInstance(int unitid,int nTemplateID,int nInstanceID =-1 )
	{
		const cUnit *punit=GetUnit(unitid);
		assert(punit);
		return CreateInstance(punit,nTemplateID,nInstanceID);
	}

	virtual cInstNode * GetInstance(int nID)const = 0;				// 用实例ID取得实例指针
	virtual void DestroyInstance(int nID) = 0;								// 用实例ID删除实例指针
	virtual int GetNextInstHandle(int nID)const = 0;						// 取得实例句柄。hHandle == NULL为第一个
	virtual void DestroyAllInstance() = 0;									// 删除所有的实例指针

	virtual BOOL ExportSrcHeader(const char * lpszPath) = 0;				// 输出C++头文件
	virtual BOOL BuildFormulaSrc(const char * lpszPath) = 0;				// 输出单元中的字段公式源文件
	virtual BOOL BuildLuaFrame(const char * lpszPath) = 0;					// 输出LUA脚本框架

	virtual const void * GetTemplateBuf(int nUnitID,UINT nCol,int nValue)const = 0;	// 取得指定单元ID,指定行的缓存

	virtual void SetRemain(cRemain * pRemain) = 0;			// 设置时效回调
	virtual cRemain * GetRemain()const = 0;					// 取得时效回调

	virtual cBaseNode * ImportFromExcel(cBaseNode * pParent,const char * lpszPath,const char * lpszNode,const char * lpszAlias,const char * lpszType) = 0;
	virtual BOOL ExportToExcel(cBaseNode * pNode,const char * lpszPath) = 0;

	virtual void RegistRelateListener(pFncGetInstsRelate pListener) = 0;		// 设置关系回调
	
	virtual BOOL ExportStringTable(const char * lpszFile) = 0;
	virtual BOOL ImportStringTable(const char * lpszFile) = 0;
};

typedef cWorld * (FnGetTheWorld)();
LOGIC_ENGINE_API cWorld * GetTheWorld();

