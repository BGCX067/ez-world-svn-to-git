#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <strstream>

#include "LogicEngine.h"
#include "TreeNode.h"
#include "LeafNode.h"
#include "theworld.h"

CWorldDefine g_WD;

////////////////////////////////////////////////////////////////////////////
// 布尔型定义
TCHAR * lpszBoolean[] = {_T("FALSE"),_T("TRUE")};


////////////////////////////////////////////////////////////////////////////
// 单元的基本属性	Node_UnitBase

// 枚举定义
TCHAR * lpszUnitType[] = {	
							g_WD.Str(DSEnum,_T("UnitType"),0),	// 单元
							g_WD.Str(DSEnum,_T("UnitType"),1)	// 基类
						};
TCHAR * lpszUnitDim[] = {
							g_WD.Str(DSEnum,_T("UnitDim"),0),	// 无
							g_WD.Str(DSEnum,_T("UnitDim"),1),	// 一维
							g_WD.Str(DSEnum,_T("UnitDim"),2),	// 二维
							g_WD.Str(DSEnum,_T("UnitDim"),3)	// 三维
						};

// 属性域定义,
LeafField UnitManagerFields[] = {
			{g_WD.Str(DSTable,_T("UnitManager"),0),	Field_Integer,		0,			1,				4,			NULL,		1,0},	// "ID"
			{g_WD.Str(DSTable,_T("UnitManager"),1),	Field_String,		0,			0,				MAX_NAME,	NULL,		0,0},	// "单元名"
			{g_WD.Str(DSTable,_T("UnitManager"),2),	Field_String,		0,			0,				MAX_NAME,	NULL,		0,0},	// "英文别名"
			{g_WD.Str(DSTable,_T("UnitManager"),3),	Field_Enumerate,	Unit_Unit,	Unit_Type_Max,	4,			lpszUnitType,0,0},	// "类别"
			{g_WD.Str(DSTable,_T("UnitManager"),4),	Field_Integer,		0,			1,				4,			NULL,		0,0},	// "父单元"
			{g_WD.Str(DSTable,_T("UnitManager"),5),	Field_Enumerate,	Unit_0D,	Unit_Dim_Max,	4,			lpszUnitDim,0,0},	// "位置类型"
			{g_WD.Str(DSTable,_T("UnitManager"),6),	Field_Boolean,		0,			1,				4,			lpszBoolean,0,0},	// "使用角度"
			{g_WD.Str(DSTable,_T("UnitManager"),7),	Field_Enumerate,	Unit_0D,	Unit_Dim_Max,	4,			lpszUnitDim,0,0},	// "空间类型"
			{g_WD.Str(DSTable,_T("UnitManager"),8),	Field_Boolean,		0,			1,				4,			lpszBoolean,0,0},	// "数据模板"
			{g_WD.Str(DSTable,_T("UnitManager"),9),	Field_Boolean,		0,			1,				4,			lpszBoolean,0,0},	// "限制模板"
			{g_WD.Str(DSTable,_T("UnitManager"),10),Field_Boolean,		0,			1,				4,			lpszBoolean,0,0},	// "影响模板"
			{g_WD.Str(DSTable,_T("UnitManager"),11),Field_Boolean,		0,			1,				4,			lpszBoolean,0,0},	// "BUFF模板"	
			{g_WD.Str(DSTable,_T("UnitManager"),12),Field_Boolean,		0,			1,				4,			lpszBoolean,0,0},	// "状态机"
			NULL
		};

cLeafNodeDesc_Imp g_UnitManagerDesc(Node_UnitManager,"UnitManager",FALSE,TRUE,UnitManagerFields);

////////////////////////////////////////////////////////////////////////////
// 单元的数据属性	Node_UnitAttrib

// 枚举定义
TCHAR * lpszFieldType[] = {
							g_WD.Str(DSEnum,_T("FieldType"),0),	// 字符串
							g_WD.Str(DSEnum,_T("FieldType"),1),	// 枚举
							g_WD.Str(DSEnum,_T("FieldType"),2),	// 整数
							g_WD.Str(DSEnum,_T("FieldType"),3),	// 布尔
							g_WD.Str(DSEnum,_T("FieldType"),4),	// 百分比
							g_WD.Str(DSEnum,_T("FieldType"),5),	// 浮点数
							g_WD.Str(DSEnum,_T("FieldType"),6),	// 映射
							g_WD.Str(DSEnum,_T("FieldType"),7)	// 集合
							};
TCHAR * lpszFieldFlag[] = {
							g_WD.Str(DSEnum,_T("FieldFlag"),0),	// 需要模板
							g_WD.Str(DSEnum,_T("FieldFlag"),1),	// 实例字段
							g_WD.Str(DSEnum,_T("FieldFlag"),2),	// 需要保存
							};

// 属性域定义,
LeafField UnitAttribFields[] = {
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),0),	Field_String,	0,				0,				MAX_NAME,	NULL,0,0},			// "字段名"
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),1),	Field_String,	0,				0,				MAX_NAME,	NULL,0,0},			// "英文别名"
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),2),	Field_Boolean,	0,				1,				4,			lpszBoolean,0,0},	// "需要模板"
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),3),	Field_Boolean,	0,				1,				4,			lpszBoolean,0,0},	// "实例字段"
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),4),	Field_Boolean,	0,				1,				4,			lpszBoolean,0,0},	// "需要保存"
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),5),	Field_Enumerate,Field_String,	Field_TypeMax,	4,			lpszFieldType,0,0},	// "类型"
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),6),	Field_Integer,	-1,				-1,				4,			NULL,0,0},			// "最小值"
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),7),	Field_Integer,	-1,				-1,				4,			NULL,0,0},			// "最大值"
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),8),	Field_Integer,	-1,				-1,				4,			NULL,0,0},			// "长度"
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),9),	Field_Integer,	-1,				-1,				4,			NULL,0,0},			// "索引键"
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),10),	Field_Integer,	-1,				-1,				4,			NULL,0,0},			// "上限字段"
			//增加字段间的计算联系
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),11),	Field_Boolean,	0,				1,				4,			lpszBoolean,0,0},	// "是否衍生字段"
			{g_WD.Str(DSTable,_T("UnitAttribDefine"),12),	Field_String,	0,				0,				MAX_NAME,	NULL,0,0},			// "关联属性"
			NULL
	};
cLeafNodeDesc_Imp g_UnitAttribDefineDesc(Node_UnitAttrib,"UnitAttribDefine",FALSE,TRUE,UnitAttribFields);

////////////////////////////////////////////////////////////////////////////
//	数据模板定义	Node_Template
cLeafNodeDesc_Imp g_UnitTemplateDesc(Node_UnitTemplate,"UnitTemplate",FALSE,TRUE,NULL);

////////////////////////////////////////////////////////////////////////////
//	状态机定义	Node_UnitFsm

// 属性域定义,
LeafField UnitFsmFields[] = {
			{g_WD.Str(DSTable,_T("UnitFsm"),0),	Field_Integer,		0,				1,				4,			NULL,1,0},			// "ID"
			{g_WD.Str(DSTable,_T("UnitFsm"),1),	Field_String,		0,				0,				MAX_NAME,	NULL,0,0},			// "状态名"
			{g_WD.Str(DSTable,_T("UnitFsm"),2),	Field_Boolean,		0,				1,				4,			lpszBoolean,0,0},	// "临时状态"
			{g_WD.Str(DSTable,_T("UnitFsm"),3),	Field_Integer,		0,				1,				4,			NULL,		0,0},	// "剩余时间"
			{g_WD.Str(DSTable,_T("UnitFsm"),4),	Field_Integer,		0,				1,				4,			NULL,0,0},			// "###"	隐藏的属性
			NULL
	};
cLeafNodeDesc_Imp g_UnitTsmDesc(Node_UnitFsm,"UnitFsm",FALSE,TRUE,UnitFsmFields);

////////////////////////////////////////////////////////////////////////////
//	限制模板定义	Node_UnitLimite

TCHAR * lpszLimitOp[] = {
							g_WD.Str(DSEnum,_T("LimitOp"),0),	// 大于等于
							g_WD.Str(DSEnum,_T("LimitOp"),1),	// 大于
							g_WD.Str(DSEnum,_T("LimitOp"),2),	// 等于
							g_WD.Str(DSEnum,_T("LimitOp"),3),	// 小于等于
							g_WD.Str(DSEnum,_T("LimitOp"),4),	// 小于
							g_WD.Str(DSEnum,_T("LimitOp"),5),	// 不等于
							g_WD.Str(DSEnum,_T("LimitOp"),6),	// 无标记
							g_WD.Str(DSEnum,_T("LimitOp"),7)	// 有标记
						};
TCHAR * lpszLimitDest[] = {
							g_WD.Str(DSEnum,_T("LimitDest"),0),	// 使用者
							g_WD.Str(DSEnum,_T("LimitDest"),1)	// 作用目标
						};
TCHAR * lpszLimitDestType[] = {
							g_WD.Str(DSEnum,_T("LimitDestType"),0),	// 基本属性
							g_WD.Str(DSEnum,_T("LimitDestType"),1)	// 自定义
						};
TCHAR * lpszLimitValueType[] = {
							g_WD.Str(DSEnum,_T("LimitValueType"),0),// 常数数值
							g_WD.Str(DSEnum,_T("LimitValueType"),1)	// 上限百分比
						};

// 限制属性域定义,
LeafField UnitLimiteFields[] = {
			{g_WD.Str(DSTable,_T("UnitLimite"),0),	Field_Dynamic,		0,				0,				4,			NULL,	1,0},			// "模板名称"
			{g_WD.Str(DSTable,_T("UnitLimite"),1),	Field_Dynamic,		0,				0,				4,			NULL,	2,0},			// "关系类型"
			{g_WD.Str(DSTable,_T("UnitLimite"),2),	Field_Enumerate,	Limit_Handler,	Limit_DestMax,	4,			lpszLimitDest,0,0},		// "限制目标"
			{g_WD.Str(DSTable,_T("UnitLimite"),3),	Field_Enumerate,	LimitType_Prop,	LimitType_Max,	4,			lpszLimitDestType,0,0},	// "目标类型"
			{g_WD.Str(DSTable,_T("UnitLimite"),4),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},			// "目标属性"
			{g_WD.Str(DSTable,_T("UnitLimite"),5),	Field_Enumerate,	Limit_GreaterEqual,	Limit_Max,	4,			lpszLimitOp,0,0},		// "限制操作"
			{g_WD.Str(DSTable,_T("UnitLimite"),6),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},			// "限制值"
			{g_WD.Str(DSTable,_T("UnitLimite"),7),	Field_Integer,		0,				0,				4,			NULL,	0,0},			// "组索引"
			{g_WD.Str(DSTable,_T("UnitLimite"),8),	Field_String,		0,				0,				256,		NULL,	0,0},			// "组说明"
			{g_WD.Str(DSTable,_T("UnitLimite"),9),	Field_String,		0,				0,				40,			NULL,	0,0},			// "目标属性参照"
			{g_WD.Str(DSTable,_T("UnitLimite"),10),	Field_Enumerate,	LimitValue_Constant,LimitValue_Max,4,		lpszLimitValueType,	0,0},	// "数值类型"
			NULL
	};
cLeafNodeDesc_Imp g_UnitLimiteDesc(Node_UnitLimite,"UnitLimite",FALSE,TRUE,UnitLimiteFields);

////////////////////////////////////////////////////////////////////////////
//	影响模板定义	Node_UnitEffect

TCHAR * lpszEffectPhase[] = {
								g_WD.Str(DSEnum,_T("EffectPhase"),0),	// 裸值
								g_WD.Str(DSEnum,_T("EffectPhase"),1),	// 阶段值
								g_WD.Str(DSEnum,_T("EffectPhase"),2)	// 当前值
							};
FieldId f_EffectOp = {
								g_WD.Str(DSFID,_T("EffectOp"),0),1,0	// _T("世界定义/操作管理")
							};
TCHAR * lpszDestUnit[] = {
								g_WD.Str(DSEnum,_T("DestUnit"),0),	// 使用者
								g_WD.Str(DSEnum,_T("DestUnit"),1)	// 作用目标
							};

// 影响属性域定义,
LeafField UnitEffectFields[] = {
			{g_WD.Str(DSTable,_T("UnitEffect"),0),	Field_Dynamic,		0,				0,				4,			NULL,	1,0},	// "模板名称"
			{g_WD.Str(DSTable,_T("UnitEffect"),1),	Field_Dynamic,		0,				0,				4,			NULL,	2,0},	// "关系类型"
			{g_WD.Str(DSTable,_T("UnitEffect"),2),	Field_Dynamic,		0,				0,				4,			NULL,	3,0},	// "目标元素"
			{g_WD.Str(DSTable,_T("UnitEffect"),3),	Field_Enumerate,	Limit_Handler,	Limit_DestMax,	4,			lpszDestUnit,	4,0},	// "目标单元"
			{g_WD.Str(DSTable,_T("UnitEffect"),4),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},	// "目标属性"
			{g_WD.Str(DSTable,_T("UnitEffect"),5),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},	// "影响操作"
			{g_WD.Str(DSTable,_T("UnitEffect"),6),	Field_Float,		0,				0,				8,			NULL,	0,0},	// "影响值"
			{g_WD.Str(DSTable,_T("UnitEffect"),7),	Field_Integer,		0,				0,				4,			NULL,	0,0},	// "行为参数"
			{g_WD.Str(DSTable,_T("UnitEffect"),8),	Field_Enumerate,	Phase_OrigValue,Phase_Max,		4,			lpszEffectPhase,0,0},	// "影响阶段"
			{g_WD.Str(DSTable,_T("UnitEffect"),9),	Field_String,		0,				0,				40,			NULL,	0,0},	// "目标属性参照"
			NULL
	};
cLeafNodeDesc_Imp g_UnitEffectDesc(Node_UnitEffect,"UnitEffect",FALSE,TRUE,UnitEffectFields);


////////////////////////////////////////////////////////////////////////////
//	BUFF影响模板定义	Node_BuffEffect

TCHAR * lpszEffectType[] = {
							g_WD.Str(DSEnum,_T("EffectType"),0),	// 及时生效
							g_WD.Str(DSEnum,_T("EffectType"),1),	// 持续生效
							g_WD.Str(DSEnum,_T("EffectType"),2)		// 周期生效
							};
TCHAR * lpszSpaceType[] = {
							g_WD.Str(DSEnum,_T("SpaceType"),0),		// 无
							g_WD.Str(DSEnum,_T("SpaceType"),1),		// 矩形
							g_WD.Str(DSEnum,_T("SpaceType"),2),		// 扇形
							g_WD.Str(DSEnum,_T("SpaceType"),3)		// 前冲矩形
							};
TCHAR * lpszBuffAddType[] = {
							g_WD.Str(DSEnum,_T("BuffAddType"),0),		// 无法释放
							g_WD.Str(DSEnum,_T("BuffAddType"),1),		// 可叠加
							g_WD.Str(DSEnum,_T("BuffAddType"),2),		// 延续
							g_WD.Str(DSEnum,_T("BuffAddType"),3),		// 替换
							g_WD.Str(DSEnum,_T("BuffAddType"),4),		// 排队
							g_WD.Str(DSEnum,_T("BuffAddType"),5),		// 抵消
							g_WD.Str(DSEnum,_T("BuffAddType"),6),		// 抑制
							g_WD.Str(DSEnum,_T("BuffAddType"),7)		// 推后
							};
TCHAR * lpszTimeRule[] = {
							g_WD.Str(DSEnum,_T("TimeRule"),0),		// 无限制
							g_WD.Str(DSEnum,_T("TimeRule"),1),		// 更早时间
							g_WD.Str(DSEnum,_T("TimeRule"),2)		// 更晚时间
							};
TCHAR * lpszLeverRule[] = {
							g_WD.Str(DSEnum,_T("LeverRule"),0),		// 级别无关
							g_WD.Str(DSEnum,_T("LeverRule"),1),		// 同级
							g_WD.Str(DSEnum,_T("LeverRule"),2),		// 更高级
							g_WD.Str(DSEnum,_T("LeverRule"),3)		// 更低级
							};
TCHAR * lpszSourceLever[] = {
							g_WD.Str(DSEnum,_T("SourceLever"),0),		// 无限制
							g_WD.Str(DSEnum,_T("SourceLever"),1)		// 同一释放者
							};
TCHAR * lpszEffectValueType[] = {
							g_WD.Str(DSEnum,_T("EffectValueType"),0),		// 常数
							g_WD.Str(DSEnum,_T("EffectValueType"),1),		// 脚本
							g_WD.Str(DSEnum,_T("EffectValueType"),2)		// 状态机
							};
TCHAR * lpszEVParam[] = {
							g_WD.Str(DSEnum,_T("EVParam"),0),		// NULL
							g_WD.Str(DSEnum,_T("EVParam"),1),		// 使用者实体
							g_WD.Str(DSEnum,_T("EVParam"),2)		// 作用目标实体
							};

// 影响属性域定义,
LeafField BuffEffectFields[] = {
			{g_WD.Str(DSTable,_T("BuffEffect"),0),	Field_Dynamic,		0,				0,				4,			NULL,	1,0},	// "模板名称"
			{g_WD.Str(DSTable,_T("BuffEffect"),1),	Field_Dynamic,		0,				0,				4,			NULL,	2,0},	// "关系类型"
			{g_WD.Str(DSTable,_T("BuffEffect"),2),	Field_Dynamic,		0,				0,				4,			NULL,	3,0},	// "目标元素"
			{g_WD.Str(DSTable,_T("BuffEffect"),3),	Field_Enumerate,	Limit_Handler,	Limit_DestMax,	4,			lpszDestUnit,	4,0},	// "目标单元"
			{g_WD.Str(DSTable,_T("BuffEffect"),4),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},	// "目标属性"
			{g_WD.Str(DSTable,_T("BuffEffect"),5),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},	// "影响操作"
			//
			{g_WD.Str(DSTable,_T("BuffEffect"),6),	Field_Enumerate,	EVT_Constant,	EVT_Max,		4,			lpszEffectValueType,0,0},// "影响值类型"	
			{g_WD.Str(DSTable,_T("BuffEffect"),7),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},	// "影响值"
			{g_WD.Str(DSTable,_T("BuffEffect"),8),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},	// "影响值参数1"
			{g_WD.Str(DSTable,_T("BuffEffect"),9),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},	// "影响值参数2"
			{g_WD.Str(DSTable,_T("BuffEffect"),10),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},	// "影响值参数3"
			{g_WD.Str(DSTable,_T("BuffEffect"),11),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},	// "影响值参数4"
			{g_WD.Str(DSTable,_T("BuffEffect"),12),	Field_Integer,		0,				0,				4,			NULL,	0,0},	// "行为参数"
			{g_WD.Str(DSTable,_T("BuffEffect"),13),	Field_Enumerate,	Phase_OrigValue,Phase_Max,		4,			lpszEffectPhase,0,0},	// "影响阶段"
			{g_WD.Str(DSTable,_T("BuffEffect"),14),	Field_Integer,		0,				0,				4,			NULL,	0,0},	// "释放者实例"
			{g_WD.Str(DSTable,_T("BuffEffect"),15),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},	// "自身类型"	例如：一刀两断不同等级的BUFF不同，但是类型都属于【一刀两断】
			{g_WD.Str(DSTable,_T("BuffEffect"),16),	Field_Integer,		0,				0,				4,			NULL,	0,0},	// "等级"
			{g_WD.Str(DSTable,_T("BuffEffect"),17),	Field_Enumerate,	BuffEffect_InTime,BuffEffect_Max,4,			lpszEffectType,	0,0},	// "作用类型"	即时生效|持续生效|周期生效
			{g_WD.Str(DSTable,_T("BuffEffect"),18),	Field_Integer,		0,				0,				4,			NULL,	0,0},	// "持续时间"
			{g_WD.Str(DSTable,_T("BuffEffect"),19),	Field_Integer,		0,				0,				4,			NULL,	0,0},	// "周期间隔"
			{g_WD.Str(DSTable,_T("BuffEffect"),20),	Field_Integer,		0,				0,				4,			NULL,	0,0},	// "延迟时间"
			{g_WD.Str(DSTable,_T("BuffEffect"),21),	Field_Dynamic,		0,				0,				4,			NULL,	0,0},	// "触发条件"
			{g_WD.Str(DSTable,_T("BuffEffect"),22),	Field_Enumerate,	BuffEffectSpace_None,BuffEffectSpace_Max,4,	lpszSpaceType,	0,0},	// "空间类型"	矩形|扇形|前冲矩形
			{g_WD.Str(DSTable,_T("BuffEffect"),23),	Field_Integer,		0,				0,				4,			NULL,	0,0},			// "空间参数1"	矩形&前冲矩形(宽)||扇形(半径)
			{g_WD.Str(DSTable,_T("BuffEffect"),24),	Field_Integer,		0,				0,				4,			NULL,	0,0},			// "空间参数2"	矩形&前冲矩形(长)||扇形(角度)
			{g_WD.Str(DSTable,_T("BuffEffect"),25),	Field_Boolean,		0,				1,				4,			lpszBoolean,0,0},		// "是否跟随移动"
			{g_WD.Str(DSTable,_T("BuffEffect"),26),	Field_Boolean,		0,				1,				4,			lpszBoolean,0,0},		// "是否使用递减"
			{g_WD.Str(DSTable,_T("BuffEffect"),27),	Field_Enumerate,	BAT_None,		BAT_Max,		4,			lpszBuffAddType,0,0},	// "叠加方式"
			{g_WD.Str(DSTable,_T("BuffEffect"),28),	Field_Enumerate,	TR_None,		TR_Max,			4,			lpszTimeRule,0,0},		// "时间规则"
			{g_WD.Str(DSTable,_T("BuffEffect"),29),	Field_Enumerate,	LR_None,		LR_Max,			4,			lpszLeverRule,0,0},		// "级别规则"
			{g_WD.Str(DSTable,_T("BuffEffect"),30),	Field_Integer,		0,				0,				4,			NULL,	0,0},			// "数量规则"
			{g_WD.Str(DSTable,_T("BuffEffect"),31),	Field_Enumerate,	SR_None,		SR_Max,			4,			lpszSourceLever,0,0},	// "来源规则"
			{g_WD.Str(DSTable,_T("BuffEffect"),32),	Field_Integer,		0,				0,				4,			NULL,	0,0},			// "组索引"
			{g_WD.Str(DSTable,_T("BuffEffect"),33),	Field_String,		0,				0,				256,		NULL,	0,0},			// "组说明"
			{g_WD.Str(DSTable,_T("BuffEffect"),34),	Field_String,		0,				0,				40,			NULL,	0,0},			// "目标属性参照"
			{g_WD.Str(DSTable,_T("BuffEffect"),35),	Field_Boolean,		0,				1,				4,			lpszBoolean,0,0},		// "能否重击"
			{g_WD.Str(DSTable,_T("BuffEffect"),36),	Field_Boolean,		0,				1,				4,			lpszBoolean,0,0},		// "对敌有效"
			{g_WD.Str(DSTable,_T("BuffEffect"),37),	Field_Boolean,		0,				1,				4,			lpszBoolean,0,0},		// "能否幸运一击"
			{g_WD.Str(DSTable,_T("BuffEffect"),38),	Field_Integer,		0,				0,				4,			NULL,	0,0},			// "图标"
			{g_WD.Str(DSTable,_T("BuffEffect"),39),	Field_String,		0,				0,				128,		NULL,	0,0},			// "BUFF说明"
			NULL
	};
cLeafNodeDesc_Imp g_BuffEffectDesc(Node_BuffEffect,"BuffEffect",FALSE,TRUE,BuffEffectFields);


////////////////////////////////////////////////////////////////////////////
//	单元定义	Node_Unit

cNodeDesc * g_pUnitDefDescLeaf[] = {
				&g_UnitAttribDefineDesc,
				&g_UnitTemplateDesc,
				&g_UnitLimiteDesc,
				&g_UnitEffectDesc,
				&g_BuffEffectDesc,
				&g_UnitTsmDesc,
				NULL
		};
cTreeNodeDesc_imp g_UnitDefDesc(Node_Unit,"UnitDefine",FALSE,FALSE,(cNodeDesc **)&g_pUnitDefDescLeaf);


////////////////////////////////////////////////////////////////////////////
//	单元定义管理器	Node_UnitStructMgr

cNodeDesc * g_pUnitDefMgrDescLeaf[] = {
				&g_UnitDefDesc, 
				NULL
		};
cTreeNodeDesc_imp g_UnitTreeDesc(Node_UnitTree,"UnitTree",FALSE,TRUE,(cNodeDesc **)&g_pUnitDefMgrDescLeaf);


////////////////////////////////////////////////////////////////////////////
// 单元关系列表	Node_Relation

// 枚举定义
TCHAR * lpszRelationType[] = {
							g_WD.Str(DSEnum,_T("RelationType"),0),		// 物品附加
							g_WD.Str(DSEnum,_T("RelationType"),1),		// 临时BUF
							g_WD.Str(DSEnum,_T("RelationType"),2)		// 即时生效
						};

FieldId f_Unit = {
							g_WD.Str(DSFID,_T("Unit"),0),				// _T("世界定义/单元管理"),
							1,0
				};

// 属性域定义,
LeafField RelationFields[] = {
			{g_WD.Str(DSTable,_T("UnitRelationDefine"),0),	Field_Integer,		0,				1,				4,			NULL,1,0},	// "ID"
			{g_WD.Str(DSTable,_T("UnitRelationDefine"),1),	Field_String,		0,				0,				MAX_NAME,	NULL,0,0},	// "关系名"
			{g_WD.Str(DSTable,_T("UnitRelationDefine"),2),	Field_IdField,		0,				0,				4,			(TCHAR**)&f_Unit,0,0},	// "作用目标"
			{g_WD.Str(DSTable,_T("UnitRelationDefine"),3),	Field_IdField,		0,				0,				4,			(TCHAR**)&f_Unit,0,0},	// "作用者"
			{g_WD.Str(DSTable,_T("UnitRelationDefine"),4),	Field_IdField,		0,				0,				4,			(TCHAR**)&f_Unit,0,0},	// "使用者"
			{g_WD.Str(DSTable,_T("UnitRelationDefine"),5),	Field_Enumerate,	Relation_Attach,Relation_Max,	4,			lpszRelationType,0,0},	// "关系类型"
			NULL
	};
cLeafNodeDesc_Imp g_RelationDefineDesc(Node_Relation,"UnitRelationDefine",FALSE,TRUE,RelationFields);


////////////////////////////////////////////////////////////////////////////
// 行为管理列表	Node_Behavior

// 枚举定义
FieldId f_Calc = {
					g_WD.Str(DSFID,_T("Calc"),0),				// _T("世界定义/计算方式"),
					1,0};

// 属性域定义,
LeafField BehaviorFields[] = {
	{g_WD.Str(DSTable,_T("BehaviorDefine"),0),	Field_Integer,		0,				1,				4,			NULL,1,0},	// "ID"
	{g_WD.Str(DSTable,_T("BehaviorDefine"),1),	Field_String,		0,				0,				MAX_NAME,	NULL,0,0},	// "行为名"
	{g_WD.Str(DSTable,_T("BehaviorDefine"),2),	Field_String,		0,				0,				MAX_NAME,	NULL,0,0},	// "英文别名"
	{g_WD.Str(DSTable,_T("BehaviorDefine"),3),	Field_IdField,		0,				0,				4,			(TCHAR**)&f_Unit,0,0},	// "源单元"
	{g_WD.Str(DSTable,_T("BehaviorDefine"),4),	Field_IdField,		0,				0,				4,			(TCHAR**)&f_Unit,0,0},	// "目标单元"
	NULL
};
cLeafNodeDesc_Imp g_BehaviorDefineDesc(Node_Beharior,"BehaviorDefine",FALSE,TRUE,BehaviorFields);

////////////////////////////////////////////////////////////////////////////
// 操作管理列表	Node_Operate

// 属性域定义,
LeafField OperateFields[] = {
	{g_WD.Str(DSTable,_T("OperateDefine"),0),	Field_Integer,		0,				1,				4,			NULL,1,0},	// "ID"
	{g_WD.Str(DSTable,_T("OperateDefine"),1),	Field_String,		0,				0,				MAX_NAME,	NULL,0,0},	// "操作名"
	{g_WD.Str(DSTable,_T("OperateDefine"),2),	Field_String,		0,				0,				MAX_NAME,	NULL,0,0},	// "英文别名"
	NULL
};
cLeafNodeDesc_Imp g_OperateDefineDesc(Node_Operate,"OperateDefine",FALSE,TRUE,OperateFields);

////////////////////////////////////////////////////////////////////////////
// 联系管理列表	Node_ContactManager

// 属性域定义,
LeafField ContactManagerFields[] = {
	{g_WD.Str(DSTable,_T("ContactManager"),0),	Field_Integer,		0,				1,				4,			NULL,1,0},	// "ID"
	{g_WD.Str(DSTable,_T("ContactManager"),1),	Field_String,		0,				0,				MAX_NAME,	NULL,0,0},	// "联系名"
	{g_WD.Str(DSTable,_T("ContactManager"),2),	Field_String,		0,				0,				MAX_NAME,	NULL,0,0},	// "英文别名"
	NULL
};
cLeafNodeDesc_Imp g_ContactManagerDesc(Node_ContactManager,"ContactManager",FALSE,TRUE,ContactManagerFields);


////////////////////////////////////////////////////////////////////////////
// 联系定义列表	Node_ContactAttrib

// 属性域定义,
LeafField ContactFields[] = {
	{g_WD.Str(DSTable,_T("ContactDefine"),0),	Field_String,		0,				0,				MAX_NAME,	NULL,0,0},	// "字段名"
	{g_WD.Str(DSTable,_T("ContactDefine"),1),	Field_String,		0,				0,				MAX_NAME,	NULL,0,0},	// "英文别名"
	{g_WD.Str(DSTable,_T("ContactDefine"),2),	Field_Enumerate,	Field_String,	Field_TypeMax,	4,			lpszFieldType,0,0},	// "类型"
	{g_WD.Str(DSTable,_T("ContactDefine"),3),	Field_Integer,		-1,				-1,				4,			NULL,0,0},	// "最小值"
	{g_WD.Str(DSTable,_T("ContactDefine"),4),	Field_Integer,		-1,				-1,				4,			NULL,0,0},	// "最大值"
	{g_WD.Str(DSTable,_T("ContactDefine"),5),	Field_Integer,		-1,				-1,				4,			NULL,0,0},	// "长度"
	{g_WD.Str(DSTable,_T("ContactDefine"),6),	Field_Integer,		0,				1,				4,			NULL,0,0},	// "索引键"
	NULL
};
cLeafNodeDesc_Imp g_ContactDefineDesc(Node_ContactAttrib,"ContactDefine",FALSE,TRUE,ContactFields);

////////////////////////////////////////////////////////////////////////////
//	联系数据定义	Node_ContactData
cLeafNodeDesc_Imp g_ContactDataDesc(Node_ContactData,"ContactData",FALSE,TRUE,NULL);

////////////////////////////////////////////////////////////////////////////
//	联系结构	Node_Contact

cNodeDesc * g_pContactDescLeaf[] = {
		&g_ContactDefineDesc,
		&g_ContactDataDesc,
		NULL
};
cTreeNodeDesc_imp g_ContactDefDesc(Node_Contact,"Contact",FALSE,FALSE,(cNodeDesc **)&g_pContactDescLeaf);

////////////////////////////////////////////////////////////////////////////
//	联系结构管理器	Node_ContactTree

cNodeDesc * g_pContactDefMgrDescLeaf[] = {
	&g_ContactDefDesc, 
		NULL
};
cTreeNodeDesc_imp g_ContactTreeDesc(Node_ContactTree,"ContactTree",FALSE,TRUE,(cNodeDesc **)&g_pContactDefMgrDescLeaf);


////////////////////////////////////////////////////////////////////////////
//	世界定义	Node_Root

cNodeDesc * g_pRootDescLeaf[] = {
				&g_UnitManagerDesc,
				&g_RelationDefineDesc,
				&g_BehaviorDefineDesc,
				&g_OperateDefineDesc,
				&g_ContactManagerDesc,
				&g_UnitTreeDesc,
				&g_ContactTreeDesc,
				NULL
		};
cTreeNodeDesc_imp g_RootDesc(Node_Root,"WorldDefine",TRUE,TRUE,(cNodeDesc **)&g_pRootDescLeaf);								// Node_Root



////////////////////////////////////////////////////////////////////////////
//	节点描述指针				节点名				节点类型

NodeTable g_NodeTable[] = {
	{&g_RootDesc,				g_WD.Str(DSTree,_T("tree"),Node_Root),			_T("Root")},			// _T("世界定义")
	
	{&g_UnitManagerDesc,		g_WD.Str(DSTree,_T("tree"),Node_UnitManager),	_T("UnitManager")},		// _T("单元管理")
	{&g_RelationDefineDesc,		g_WD.Str(DSTree,_T("tree"),Node_Relation),		_T("Relation")},		// _T("关系定义")

	{&g_BehaviorDefineDesc,		g_WD.Str(DSTree,_T("tree"),Node_Beharior),		_T("Beharior")},		// _T("行为管理")
	{&g_OperateDefineDesc,		g_WD.Str(DSTree,_T("tree"),Node_Operate),		_T("Operate")},			// _T("操作管理")
	{&g_ContactManagerDesc,		g_WD.Str(DSTree,_T("tree"),Node_ContactManager),_T("ContactManager")},	// _T("联系管理")

	{&g_UnitTreeDesc,			g_WD.Str(DSTree,_T("tree"),Node_UnitTree),		_T("UnitTree")},		// _T("单元数据")
	{&g_UnitDefDesc,			g_WD.Str(DSTree,_T("tree"),Node_Unit),			_T("Unit")},			// _T("单元")
	{&g_UnitAttribDefineDesc,	g_WD.Str(DSTree,_T("tree"),Node_UnitAttrib),	_T("UnitAttrib")},		// _T("数据属性")
	{&g_UnitTemplateDesc,		g_WD.Str(DSTree,_T("tree"),Node_UnitTemplate),	_T("UnitTemplate")},	// _T("数据模板")
	{&g_UnitLimiteDesc,			g_WD.Str(DSTree,_T("tree"),Node_UnitLimite),	_T("UnitLimite")},		// _T("限制模板")
	{&g_UnitEffectDesc,			g_WD.Str(DSTree,_T("tree"),Node_UnitEffect),	_T("UnitEffect")},		// _T("影响模板")
	{&g_BuffEffectDesc,			g_WD.Str(DSTree,_T("tree"),Node_BuffEffect),	_T("BuffEffect")},		// _T("BUFF模板")
	{&g_UnitTsmDesc,			g_WD.Str(DSTree,_T("tree"),Node_UnitFsm),		_T("UnitFsm")},			// _T("状态机")

	{&g_ContactTreeDesc,		g_WD.Str(DSTree,_T("tree"),Node_ContactTree),	_T("ContactTree")},		// _T("单元联系")
	{&g_ContactDefDesc,			g_WD.Str(DSTree,_T("tree"),Node_Contact),		_T("Contact")},			// _T("联系")
	{&g_ContactDefineDesc,		g_WD.Str(DSTree,_T("tree"),Node_ContactAttrib),	_T("ContactAttrib")},	// _T("联系定义")
	{&g_ContactDataDesc,		g_WD.Str(DSTree,_T("tree"),Node_ContactData),	_T("ContactData")},		// _T("联系数据")

	{NULL,						g_WD.Str(DSTree,_T("tree"),Node_UnitCalc),		_T("UnitCalc")},		// _T("数据计算")
	{NULL,						g_WD.Str(DSTree,_T("tree"),Node_UnitOperation),	_T("UnitOperation")},	// _T("操作定义")
	{NULL,						g_WD.Str(DSTree,_T("tree"),Node_Unit_IntanceMgr),_T("UnitIntanceMgr")},	// _T("实例管理器")
	{NULL,						g_WD.Str(DSTree,_T("tree"),Node_Unit_Intance),	_T("UnitIntance")}		// _T("实例")
};	

cWorldImp g_NodeStreamImp;
LOGIC_ENGINE_API cWorld * GetTheWorld()
{
	return &g_NodeStreamImp;
}

cBaseNode * GetWorldRoot()
{
	return g_NodeStreamImp.GetRoot();
}
