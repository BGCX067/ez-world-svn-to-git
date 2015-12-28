#pragma once

#define LOGIC_FLAG *(DWORD*)"LOGC"
#define LOGIC_VER	1

#define NODE_FLAG *(DWORD*)"NODE"
#define NODE_VER  1

struct LogicHeader
{
	DWORD dwFlag;
	UINT nVersion;
};

struct NodeDesc
{
	Node_Type	m_eType;
	TCHAR		m_szName[MAX_NAME];
	BOOL		m_bFiled;
	BOOL		m_bExclusive;
	BOOL		m_bLeaf;
};
struct NodeDescEx
{
	Node_Type	m_eType;
	TCHAR		m_szName[MAX_NAME];
	TCHAR		m_szAlias[MAX_NAME];
	BOOL		m_bFiled;
	BOOL		m_bExclusive;
	BOOL		m_bLeaf;
};

struct TreeNodeDesc
{
	UINT nChildren;
};
struct LeafNodeDesc
{
	UINT nRows;
	UINT nRowLength;
};

struct LeafFieldDesc
{
	TCHAR		m_szName[MAX_NAME];
	TCHAR		m_szAlias[MAX_NAME];	// 英文别名

	FieldType	m_eType;				// 属性域类型
	int			m_nMin;					// 最大值
	int			m_nMax;					// 最小值
	int			m_nLength;				// 字符串长度/ 枚举数量
	UINT		m_nEnumItems;			// 枚举/布尔字符串
	int			m_nKeyOrder;			// 索引键编号 0不是索引键
	int			m_nLimitField;			// 限制自己的字段。(0为无，使用时减1)
	int			m_nLimitDest;			// 自己限制的字段。(0为无，使用时减1)

	DWORD		m_dwFlags;				// 数据域属性
	int			m_nInstanceOffset;		// 在实例缓存中的偏移量
	int			m_nTemplateOffset;		// 在模板缓存中的偏移量
	int			m_bIsDeriveField;		// 是否是衍生字段
	TCHAR 		m_szRefField[MAX_NAME];	// 关联的属性
};

struct FieldIdDesc
{
	TCHAR szLeafPath[MAX_PATH];
	int nDispField;
	int nValField;	// -1表示不是字段，是索引。
};