#pragma once


class cTreeNodeDesc_imp : public cTreeNodeDesc
{
	Node_Type		m_eType;
	TCHAR		*	m_lpszDescName;
	BOOL			m_bFiled;
	BOOL			m_bExclusive;
	UINT			m_nChildTypes;
	cNodeDesc **	m_ppNodeDesc;
public:
	cTreeNodeDesc_imp(Node_Type type,
					TCHAR * lpszDescName,
					BOOL bFiled,
					BOOL bExclusive,
					cNodeDesc ** ppNodeDesc);
	virtual ~cTreeNodeDesc_imp();

	virtual Node_Type  GetType()const {return m_eType;}					// 节点类型
	virtual const TCHAR * GetDescName()const {return m_lpszDescName;}	// 节点描述名字
	virtual BOOL IsLeaf()const {return FALSE;}							// 是否叶节点
	virtual BOOL IsFiled()const {return m_bFiled;}						// 是否保存到文件
	virtual BOOL IfExclusive()const {return m_bExclusive;}				// 此节点类型在父节点是否唯一

	virtual UINT GetChildTypeAllowedCount()const {return m_nChildTypes;}// 允许包含的节点类型数量
	virtual const cNodeDesc * GetChildTypeAllowed(UINT n)const ;		// 允许包含的节点类型
	virtual BOOL IfChildTypeAllowed(Node_Type type)const ;				// 是否允许包含指定的节点类型
};

class cTreeNode_Imp : public cTreeNode
{
protected:
	cTreeNodeDesc*	m_pNodeDesc;
	TCHAR			m_szName[MAX_NAME];
	TCHAR			m_szAlias[MAX_NAME];
	UINT			m_nChildren;
	cBaseNode	**	m_ppChildren;
	UINT			m_nAlloc;
	cBaseNode	*	m_pParent;

public:
	cTreeNode_Imp(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent);
	cTreeNode_Imp(cNodeDesc * pDesc,cBaseNode * pParent)
		:m_pNodeDesc((cTreeNodeDesc*)pDesc)
		,m_nChildren(0)
		,m_ppChildren(NULL)
		,m_nAlloc(0)
		,m_pParent(pParent)
	{
	}
	virtual ~cTreeNode_Imp();

	virtual cNodeDesc * GetDesc() const{return m_pNodeDesc;}			// 节点性质描述
	virtual const TCHAR * GetName() const{return m_szName;}				// 节点名字
	virtual const TCHAR * GetAlias() const{return m_szAlias;}				// 节点名字
	virtual const TCHAR * GetPath(TCHAR * lpszBuf,size_t size) const;	// 取得自己的路径
	virtual const cBaseNode * GetNodeByPath(const TCHAR * lpszPath) const;	// 取得指定路径的节点
	virtual const cBaseNode * GetParent() const{return m_pParent;}

	virtual UINT GetChildren() const{return m_nChildren;}				// 子节点数量
	virtual cBaseNode * GetChildNode(UINT n) const{return n<m_nChildren?m_ppChildren[n]:NULL;}// 取得子节点
	virtual BOOL AddChildNode(cBaseNode * pNode);						// 填加子节点
	virtual cBaseNode * NewChildNode(Node_Type type,TCHAR * lpszName,TCHAR * lpszAlias);	// 新建子节点
	virtual BOOL DelChildNode(cBaseNode * pNode);						// 删除子节点
	virtual const cUnit * GetUnit() const{return NULL;}
};

class cUnitTreeNode : public cTreeNode_Imp, public cUnit
{
	// 单元的基本定义
	UnitDef *		m_pUnitDef;
	// 可能包含的关系
	Relation **		m_ppRelations;
	UINT			m_nRelations;
	// 域改变事件回调
	cFieldEvent **	m_ppFieldEvents;
	UINT			m_nFields;

	BOOL ChildExist(const cNodeDesc * pDesc)const ;

public:
	cUnitTreeNode(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent);
	~cUnitTreeNode();
	void SetUnitDef(UnitDef *pDef) {m_pUnitDef=pDef;}
	virtual const cUnit * GetUnit()const{return this;}

	virtual const cTreeNode * GetTreeNode()const{return this;}
	virtual const UnitDef * GetUnitDef()const{return m_pUnitDef;}				// 取得自己的关联单元声明
	virtual UINT GetRelations()const{return m_nRelations;}
	virtual const Relation * GetRelation(UINT n)const{return m_ppRelations[n];}
	virtual void RefreshUnitTreeNode();								// 根据单元定义刷新单元节点
	virtual const cLeafNode * GetLeaf(Node_Type type)const;
	virtual void SetFieldEvent(cFieldEvent * pEven,UINT nCol);
	virtual const cFieldEvent * GetFieldEvent(UINT nCol) const;
	virtual const cLeafNodeDesc * GetAttribDesc() const;
};

class cContactTreeNode : public cTreeNode_Imp, public cContact
{
	// 单元的基本定义
	ContactDef *		m_pContactDef;
	// 域改变事件回调
	cFieldEvent **	m_ppFieldEvents;
	UINT			m_nFields;

	BOOL ChildExist(const cNodeDesc * pDesc)const ;

public:
	cContactTreeNode(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent);
	~cContactTreeNode(){}

	virtual const cTreeNode * GetTreeNode()const{return this;}
	virtual const ContactDef * GetContactDef()const{return m_pContactDef;}				// 取得自己的关联联系声明
	virtual void RefreshContactTreeNode();												// 根据联系定义刷新单元联系节点
	virtual const cLeafNode * GetLeaf(Node_Type type)const;
	virtual void SetFieldEvent(cFieldEvent * pEven,UINT nCol);
	virtual const cFieldEvent * GetFieldEvent(UINT nCol) const;
	virtual const cLeafNodeDesc * GetAttribDesc() const;
};

class cUnitMgrTreeNode : public cTreeNode_Imp
{
	static int compare( const void *c1, const void *c2);
public:
	cUnitMgrTreeNode(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent)
		:cTreeNode_Imp(pDesc,lpszName,lpszAlias,pParent)
	{
	}
	~cUnitMgrTreeNode(){}
	void SortChildren();
};