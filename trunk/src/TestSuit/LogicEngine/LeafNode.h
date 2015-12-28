#pragma once
#include <set>
#include <list>
// cLeafNode_Imp 与 cAttribObject 的不同之处在于 cLeafNode_Imp 是 2D 表
// 1、构造时指定 m_pFields 和 m_nFields,
// 2、构造时无	 m_pFields 和 m_nFields,	

struct Field
{
	LeafField * f;
	int idx;
	
	bool operator==(const Field &of) const
	{
		return lstrcmpi(f->m_lpszName,of.f->m_lpszName) == 0;
	}
	bool operator<(const Field& of)const
	{
		return strcmp(f->m_lpszName,of.f->m_lpszName)<0;
	}
};
class cLeafNodeDesc_Imp : public cLeafNodeDesc
{
protected:
	Node_Type	m_eType;
	TCHAR		m_szName[MAX_NAME];
	BOOL		m_bFiled;
	BOOL		m_bExclusive;

	UINT		m_nFields;
	LeafField * m_pFields;
	UINT		m_nTempLength;				// 模板行缓存长度
	UINT		m_nInstLength;				// 实例缓存长度
	
	std::vector<Field> m_vecFields;			// 查找加速
	UINT		m_nIDField;					// 特别对"ID"字段加速

public:
	cLeafNodeDesc_Imp(Node_Type type, 
				TCHAR * lpszName, 
				BOOL bFiled,
				BOOL bExclusive, 
				LeafField * pFields
			);
	virtual ~cLeafNodeDesc_Imp();

	virtual Node_Type  GetType()const {return m_eType;}					// 节点类型
	virtual const TCHAR * GetDescName()const {return m_szName;}			// 节点描述名字
	virtual BOOL IsLeaf()const {return TRUE;}							// 是否叶节点
	virtual BOOL IsFiled()const {return m_bFiled;}						// 是否保存到文件
	virtual BOOL IfExclusive()const {return m_bExclusive;}				// 此节点类型在父节点是否唯一

	virtual UINT GetFieldCount()const {return m_nFields;}				// 属性域的数量。可扩展的网格，数量为 0xffffffff
	virtual const LeafField * GetField(UINT n)const ;	
	virtual UINT FindField(const TCHAR * lpszField)const ;
	virtual UINT FindFieldByAlias(const TCHAR * lpszFieldAlias) const;
	virtual UINT GetRowLength()const {return m_nTempLength;} 
	virtual UINT GetInstLength()const {return m_nInstLength;} 
	virtual UINT GetBaseFields()const{return 0;}						// 取得基类字段数(不可改变的)

	virtual UINT GetIDField() const{return m_nIDField;}
};

class cTemplateLeafNodeDesc : public cLeafNodeDesc_Imp
{
	//struct FieldEnum
	//{
	//	UINT nNum;
	//	TCHAR * pString;
	//};

	cTemplateLeafNodeDesc * m_pBaseDesc;
	cLeafNode	*	m_pSourceNode;
	TCHAR	*		m_pFieldNames;	// 属性域的名字统一分配
	TCHAR	*		m_pRefFields;
	std::map<UINT,FieldEnum *>	m_mapFieldEnum;
	std::map<UINT,FieldId	*>	m_mapFieldID;
public:
	cTemplateLeafNodeDesc(cNodeDesc * pDesc,cLeafNode * pSource,cLeafNode * pBase);
	~cTemplateLeafNodeDesc();

	virtual UINT GetFieldCount()const ;							// 属性域的数量。可扩展的网格，数量为 0xffffffff
	virtual const LeafField * GetField(UINT n)const ;	
	virtual UINT FindField(const TCHAR * lpszField)const ;
	virtual UINT FindFieldByAlias(const TCHAR * lpszFieldAlias) const;
	virtual UINT GetRowLength()const ;
	virtual UINT GetInstLength()const ;
	virtual UINT GetBaseFields()const{return m_pSourceNode->GetBaseRows();} 
	virtual UINT GetIDField() const;

	void SetEnumString(UINT nRow,const TCHAR * lpszEnum,UINT nNum);	// 设置数据属性叶节点的枚举字符串
	const TCHAR * GetEnumString(UINT nRow,UINT& nNum)const ;		// 取得数据属性叶节点的枚举字符串

	void SetFieldID(UINT nRow,FieldId * pFieldId);					// 设置数据属性叶节点的属性域对应
	void GetFieldID(UINT nRow,FieldId * pFieldId)const ;			// 取得数据属性叶节点的属性域对应

	void Build();

	const cLeafNode *	GetSourceNode()const {return m_pSourceNode;}
};

class cLeafRow_Imp : public cLeafRow
{
#ifdef _DEBUG
	FieldValue	*	m_pFieldValues;
#endif
	cLeafNodeDesc *	m_pNodeDesc;
	void * m_pBuffer;

	LimitField **	m_ppLimitFields;
	EffectValue **	m_ppEffectValues;
	UINT			m_nLimitFields;
	UINT			m_nEffectValues;

	BuffEffectValue ** m_ppBuffEffect;
	UINT			m_nBuffEffects;

public:
	
	cLeafRow_Imp();
	~cLeafRow_Imp();

	//void BuildRelation(const cLeafNode * pLimiteNode,const cLeafNode * pEffectNode);
	void BuildRelation(const cLeafNode * pLimiteNode,const cLeafNode * pEffectNode,const cLeafNode * pBuffEffect);

#ifdef _DEBUG
	void Attach(cLeafNodeDesc * pDesc, BYTE * pBuffer,FieldValue * pFieldValues);
#else
	void Attach(cLeafNodeDesc * pDesc, BYTE * pBuffer);
#endif
	
	virtual UINT GetColumns()const {return m_pNodeDesc->GetFieldCount();}			// 取得列数 属性表横为列。
	virtual UINT FindField(const TCHAR * lpszField)const {return m_pNodeDesc->FindField(lpszField);}

	virtual const TCHAR * GetString(UINT col)const ;						// 取得指定位置的值
	virtual int GetInteger(UINT col)const ;									// 取得指定位置的值
	virtual float GetFloat(UINT col)const ;									// 取得指定位置的值
	virtual void SetString(UINT col,const TCHAR * string);					// 设置指定位置的值
	virtual void SetInteger(UINT col,int nValue);							// 设置指定位置的值
	virtual void SetFloat(UINT col,float fValue);							// 设置指定位置的值

	virtual const TCHAR * GetString(const TCHAR * lpszField)const ;			// 取得指定位置的值
	virtual int GetInteger(const TCHAR * lpszField)const ;					// 取得指定位置的值
	virtual float GetFloat(const TCHAR * lpszField)const ;					// 取得指定位置的值
	virtual void SetString(const TCHAR * lpszField,const TCHAR * string);	// 设置指定位置的值
	virtual void SetInteger(const TCHAR * lpszField,int nValue);			// 设置指定位置的值
	virtual void SetFloat(const TCHAR * lpszField,float fValue);			// 设置指定位置的值

	virtual const void * GetBuffer()const {return m_pBuffer;}				// 取得数值缓存


	UINT GetLimites()const{return m_nLimitFields;}												// 取得限制数量
	const LimitField ** GetLimitePtr()const{return (const LimitField **)m_ppLimitFields;}		// 取得限制指针
	UINT GetEffects()const{return m_nEffectValues;}												// 取得效果数量
	const EffectValue ** GetEffectPtr()const{return (const EffectValue **)m_ppEffectValues;}	// 取得效果指针
	UINT GetBuffs()const{return m_nBuffEffects;}												// 取得BUFF数量
	const BuffEffectValue ** GetBuffPtr()const{return (const BuffEffectValue **)m_ppBuffEffect;}// 取得BUFF指针
};

struct Key
{
	UINT nCols;		// 对应的列
	UINT nIndices;	// 索引的数量
};
struct Index
{
	UINT nValue;		// 值
	UINT nStart;		// 开始行号
	UINT nCount;		// 数量
	UINT nIndexFirst;	// 对应的下列的索引
	UINT nIndexNum;		// 对应的下列的索引
	UINT nKey;			// 对应的键
};

#define ROOT	_T("Root")
#define		STYLES	_T("Styles")
#define			STYLE	_T("Style")
#define				ID			_T("ID")
#define				BGCOLOR		_T("BgColor")
#define				FRCOLOR		_T("FrColor")
#define				FONTNAME	_T("FontName")
#define				FONTSIZE_	_T("FontSize")
#define				FONTBOLD	_T("FontBold")
#define				FONTITALIC	_T("FontItalic")
#define				FONTUNDERLINE	_T("FontUnderline")
#define				FONTSTRIKETHRU	_T("FontStrikethru")
#define		DESCES	_T("Desces")
#define			DESC	_T("Desc")
#define				COMMENT		_T("Comment")
#define		COLUMES	_T("Columes")
#define			COLUME	_T("Colum")
#define				WIDTH		_T("Width")
#define		ROWES	_T("Rowes")
#define			ROW		_T("Row")
#define				HEIGHT		_T("Height")
#define		CELLES	_T("Celles")
#define			CELL	_T("Cell")
#define				STYLEID		_T("StyleID")
#define				COUNT		_T("Count")
class cLeafNode_Imp : public cLeafNode
{
protected:
	// 属性
	cLeafNodeDesc *	m_pNodeDesc;
	TCHAR			m_szName[MAX_NAME];
	TCHAR			m_szAlias[MAX_NAME];
	cBaseNode	*	m_pParent;

	// 数据存储
	BYTE		*	m_pBuffer;
	cLeafRow_Imp *	m_pRows;
	UINT			m_nRows;
	UINT			m_nPreAllocRows;
#ifdef _DEBUG
	FieldValue	*	m_pFieldValues;	// For Debug
#endif
	// 索引
	Index		**	m_ppIndices;	// 索引数据
	Key			*	m_pKeys;		// 索引域
	UINT			m_nIndices;

	std::map<UINT,struct FiledFormat> m_mapFormat;		// 格式列表
	std::map<UINT,std::string> m_mapRowDesc;			// 行说明字段
	std::map<UINT,UINT>	m_mapFieldType;					// 单元格格式,其中key=(row<<16|col),value=m_mapFormat[type]
	std::map<UINT,DWORD> m_mapColWidth;
	std::map<UINT,DWORD> m_mapRowHeight;

	int GetFieldIndex(const TCHAR * lpszString)const ;
	int DichotomySearch(UINT nWhich, int nArrayCount,int nStart,int nValue)const ;
	cLeafNodeDesc * FindNodeDesc();

public:
	cLeafNode_Imp(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent);
	virtual ~cLeafNode_Imp();

	void BuildRelation();

	virtual const cNodeDesc * GetDesc() const;								// 节点性质描述
	virtual const TCHAR * GetName() const{return m_szName;}				// 节点名字
	virtual const TCHAR * GetAlias() const{return m_szAlias;}				// 节点名字
	virtual const TCHAR * GetPath(TCHAR * lpszBuf,size_t size) const;	// 取得自己的路径
	virtual const cBaseNode * GetNodeByPath(const TCHAR * lpszPath) const;	// 取得指定路径的节点
	virtual const cBaseNode * GetParent() const{return m_pParent;}

	virtual UINT GetRows() const{return m_nRows;}							// 取得行数 属性表为1行
	virtual UINT GetColumns() const{return m_pNodeDesc->GetFieldCount();}	// 取得列数 属性表横为列。
	
	virtual const TCHAR * GetString(UINT row,UINT col) const;		// 取得指定位置的值
	virtual int GetInteger(UINT row,UINT col) const;				// 取得指定位置的值
	virtual float GetFloat(UINT row,UINT col) const;				// 取得指定位置的值
	virtual void SetString(UINT row,UINT col,const TCHAR * string);	// 设置指定位置的值
	virtual void SetInteger(UINT row,UINT col,int nValue);			// 设置指定位置的值
	virtual void SetFloat(UINT row,UINT col,float fValue);			// 设置指定位置的值
	
	virtual const TCHAR * GetString(UINT row,const TCHAR * lpszField) const;		// 取得指定位置的值
	virtual int GetInteger(UINT row,const TCHAR * lpszField) const;					// 取得指定位置的值
	virtual float GetFloat(UINT row,const TCHAR * lpszField) const;					// 取得指定位置的值
	virtual void SetString(UINT row,const TCHAR * lpszField,const TCHAR * string);	// 设置指定位置的值
	virtual void SetInteger(UINT row,const TCHAR * lpszField,int nValue);			// 设置指定位置的值
	virtual void SetFloat(UINT row,const TCHAR * lpszField,float fValue);			// 设置指定位置的值

	virtual HRESULT ImportFromExcel(const TCHAR * lpszFile);		
	virtual HRESULT ExportToExcel(const TCHAR * lpszFile) const;
	virtual HRESULT ExportNode(const TCHAR * lpszFile,int nFields,int * pnFields) const;
	virtual BOOL SetEnumString(UINT nRow,const TCHAR * lpszEnum,UINT nNum){return FALSE;}	// 设置数据属性叶节点的枚举字符串
	virtual const TCHAR * GetEnumString(UINT nRow,UINT& nNum) const{return NULL;}			// 取得数据属性叶节点的枚举字符串
	virtual void SetFieldID(UINT nRow,FieldId * pFieldId){}			// 设置数据属性叶节点的属性域对应
	virtual void GetFieldID(UINT nRow,FieldId * pFieldId) const{}	// 取得数据属性叶节点的属性域对应

	virtual UINT FindRow(UINT nField,int nValue) const;
	virtual UINT FindRow(const TCHAR * lpcstrField,int nValue) const;
	virtual UINT FindRow(UINT nField,const char * lpcstrValue) const;
	virtual UINT FindRow(const TCHAR * lpcstrField,const TCHAR * lpcstrValue) const;
	virtual UINT FindRowByID(int nValue) const;

	virtual void BuildIndex();							// 重新建立索引	
	virtual UINT GetIndices() const{return m_nIndices;}	// 取得索引数量
	virtual ResultRows Query(cQuery q,BOOL bLastResult = TRUE)const ;			// 根据索引寻找行号
	virtual const cLeafRow * Result(ResultRows * r,UINT n) const;
	virtual const cLeafRow * GetRow(UINT nRow) const{return nRow<m_nRows?m_pRows+nRow:NULL;} // 根据行号取得行

	virtual void ValidRow(UINT nRow,BOOL bAllocMore);
	virtual void SetRows(UINT nRow);					// 设置行数

	virtual void OnAttribDefChanged();					// 倒腾数据
	virtual UINT GetBaseRows() const{return 0;}	
	virtual BOOL LoadLeafGridFormat(TCHAR *szPath);		//加载网格的格式(含说明字段)
	virtual BOOL SaveLeafGridFormat(TCHAR *szPath);		//保存网格的格式(含说明字段)
	virtual std::map<UINT,struct FiledFormat>& GetFieldFormat() {return m_mapFormat;}
	virtual std::map<UINT,std::string>& GetRowDesc() {return m_mapRowDesc;}
	virtual std::map<UINT,UINT>& GetFieldType() {return m_mapFieldType;}
	virtual std::map<UINT,DWORD>& GetColWidth() {return m_mapColWidth;}
	virtual std::map<UINT,DWORD>& GetRowHeight() {return m_mapRowHeight;}
	virtual void SetFieldFormat(std::map<UINT,struct FiledFormat>& mapFormat){m_mapFormat = mapFormat;}
	virtual void SetRowDesc(std::map<UINT,std::string>& mapRowDesc){m_mapRowDesc = mapRowDesc;}
	virtual void SetFieldType(std::map<UINT,UINT>& mapFieldType) {m_mapFieldType = mapFieldType;}
	virtual void SetColWidth(std::map<UINT,DWORD>& mapCol) {m_mapColWidth = mapCol;}
	virtual void SetRowHeight(std::map<UINT,DWORD>& mapRow) {m_mapRowHeight = mapRow;}

	virtual void* GetRefNode() {return NULL;}
	//////////////////////20080625公式依赖问题////////////////////////////////
	virtual BOOL IsDeriveField(UINT nRow){return FALSE;}
	virtual std::string& GetDependField(UINT nRow)
	{
		static std::string str("");
		return str;
	}
	virtual void SetDependField(UINT nRow,std::string & strValue){}
	virtual void RemoveDeriveField(UINT nRow){}
	//////////////////////20080625公式依赖问题////////////////////////////////
private:
	TCHAR * MkPathByName(TCHAR * lpName);
	void RefreshUnitTemplateNode();
	void RefreshContactDataNode();
};

class cAttribLeafNode : public cLeafNode_Imp
{
protected:
	// Node_UnitAttrib 特例 自己是其他叶节点的描述
	cTemplateLeafNodeDesc * m_pTargetDesc;
	cLeafNode		* m_pBaseClassDesc;	// 基类的字段描述
	// 单元之间继承导致公式也被继承的问题
	std::map<UINT,std::string> m_mapDepend;
	
	// 字段之间依赖关系构建
	std::map<UINT,std::set<UINT> > m_mapConverseDepend;	// 逆向依赖
	std::map<UINT,std::set<UINT> > m_mapPositiveDepend;	// 正向依赖
	std::vector<UINT>	m_vecOrderField;	// 按照依赖关系建立的有序字段表
	void GetBase();

public:
	cAttribLeafNode(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent);
	virtual ~cAttribLeafNode();

	virtual UINT GetRows()const;															// 取得行数 属性表为1行

	virtual const TCHAR * GetString(UINT row,UINT col) const;				// 取得指定位置的值
	virtual int GetInteger(UINT row,UINT col) const;						// 取得指定位置的值
	virtual float GetFloat(UINT row,UINT col) const;						// 取得指定位置的值
	virtual void SetString(UINT row,UINT col,const TCHAR * string);	// 设置指定位置的值
	virtual void SetInteger(UINT row,UINT col,int nValue);			// 设置指定位置的值
	virtual void SetFloat(UINT row,UINT col,float fValue);			// 设置指定位置的值

	virtual BOOL SetEnumString(UINT nRow,const TCHAR * lpszEnum,UINT nNum);			// 设置数据属性叶节点的枚举字符串
	virtual const TCHAR * GetEnumString(UINT nRow,UINT& nNum)const;					// 取得数据属性叶节点的枚举字符串
	virtual void SetFieldID(UINT nRow,FieldId * pFieldId);							// 设置数据属性叶节点的属性域对应
	virtual void GetFieldID(UINT nRow,FieldId * pFieldId) const;							// 取得数据属性叶节点的属性域对应

	virtual UINT FindRow(UINT nField,int nValue) const;
	virtual UINT FindRow(const TCHAR * lpcstrField,int nValue) const;
	virtual UINT FindRow(UINT nField,const char * lpcstrValue) const;
	virtual UINT FindRow(const TCHAR * lpcstrField,const TCHAR * lpcstrValue) const;

	virtual void BuildIndex();							// 重新建立索引	
	virtual ResultRows Query(cQuery q,BOOL bLastResult = FALSE)const ;			// 根据索引寻找行号
	virtual const cLeafRow * GetRow(UINT nRow)const ;	// 根据行号取得行

	virtual void ValidRow(UINT nRow,BOOL bAllocMore);
	virtual void SetRows(UINT nRow);				// 设置行数
	
	virtual UINT GetBaseRows() const;

	void ImportEnum(HANDLE hXmlNode,UINT nRow);
	void ExportEnum(HANDLE hXmlNode,UINT nRow) const;
	cLeafNodeDesc * GetTargetDesc() const{return m_pTargetDesc;}
	BOOL IsDeriveField(UINT nRow)
	{
		if(m_mapDepend.find(nRow) != m_mapDepend.end())
			return TRUE;
		return FALSE;
	}

	std::string& GetDependField(UINT nRow){return m_mapDepend[nRow];}
	void SetDependField(UINT nRow,std::string & strValue);
	virtual void RemoveDeriveField(UINT nRow)
	{
		if(m_mapDepend.find(nRow) != m_mapDepend.end())
			m_mapDepend.erase(nRow);
	}

	// 字段间正反依赖关系整理(20081120)
	bool IfHasConverseDepend(UINT nField);
	bool IfHasPositiveDepend(UINT nField);
	bool IfHasPositiveDepend(UINT nField1,UINT nField2);
	int GetDependLevel(UINT nField);
	std::set<UINT> &GetConverseDependFields(UINT nField);
	std::set<UINT> &GetPositiveDependFields(UINT nField);
	UINT * GetOrderFields() {return &m_vecOrderField.front();}
	void BuildOrderLst();

};

class cFsmNode_Imp : public cFsmNode
{
public:
	cFsmNode_Imp():m_nStateCnt(0)
		,m_nCurrentID(0),m_nPos(0)
	{
		m_mapState.clear();
		memset(&m_nRow[0],0x0,sizeof(UINT)*MAX_STATE);
	}
	virtual ~cFsmNode_Imp(){}
	//状态
	virtual	UINT GetStateCount() {return m_nStateCnt;}
	virtual const stState * GetState(UINT nStateID);
	virtual void AddState(UINT nSequence,const stState * pState);
	virtual void DelState(UINT nStateID);
	virtual UINT GetCurrentStateID() {return m_nCurrentID;}
	virtual const stState * GetCurrentState() {return &m_mapState[m_nCurrentID];}
	virtual const stState * GetFirstState();
	virtual const stState * GetNextState(UINT nStateID);

	//状态转换
	virtual UINT SwitchState(const UINT nState);

private:
	UINT	m_nStateCnt;
	UINT	m_nCurrentID;
	UINT	m_nPos;
	std::map<UINT,struct stState>	m_mapState;
	UINT	m_nRow[MAX_STATE];
};

class cFsmLeafNode : public cLeafNode_Imp
{
public:
	cFsmLeafNode(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent);
	//cFsmLeafNode(cNodeDesc * pDesc,TCHAR * lpszName,cBaseNode * pParent):cLeafNode_Imp(pDesc,lpszName,pParent)
	//{
	//	m_pFsmNode = new cFsmNode_Imp;
	//}
	virtual ~cFsmLeafNode()
	{
		if(m_pFsmNode)
			delete m_pFsmNode;
	}

	virtual void * GetRefNode(){return (void *)m_pFsmNode;}

private:
	cFsmNode_Imp	*m_pFsmNode;
};

class cContactDataLeafNodeDesc : public cLeafNodeDesc_Imp
{
	struct FieldEnum
	{
		UINT nNum;
		TCHAR * pString;
	};

	cContactDataLeafNodeDesc * m_pBaseDesc;
	cLeafNode	*	m_pSourceNode;
	TCHAR	*		m_pFieldNames;	// 属性域的名字统一分配
	std::map<UINT,FieldEnum *>	m_mapFieldEnum;
	std::map<UINT,FieldId	*>	m_mapFieldID;
public:
	cContactDataLeafNodeDesc(cNodeDesc * pDesc,cLeafNode * pSource,cLeafNode * pBase);
	~cContactDataLeafNodeDesc();

	void SetEnumString(UINT nRow,const TCHAR * lpszEnum,UINT nNum);	// 设置数据属性叶节点的枚举字符串
	const TCHAR * GetEnumString(UINT nRow,UINT& nNum)const ;		// 取得数据属性叶节点的枚举字符串

	void SetFieldID(UINT nRow,FieldId * pFieldId);					// 设置数据属性叶节点的属性域对应
	void GetFieldID(UINT nRow,FieldId * pFieldId)const ;			// 取得数据属性叶节点的属性域对应

	void Build();

	const cLeafNode *	GetSourceNode()const {return m_pSourceNode;}
};

class cContactAttribLeafNode : public cLeafNode_Imp
{
protected:
	//  特例 自己是其他叶节点的描述
	cContactDataLeafNodeDesc * m_pTargetDesc;

public:
	cContactAttribLeafNode(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent);
	virtual ~cContactAttribLeafNode();

	virtual BOOL SetEnumString(UINT nRow,const TCHAR * lpszEnum,UINT nNum);			// 设置数据属性叶节点的枚举字符串
	virtual const TCHAR * GetEnumString(UINT nRow,UINT& nNum)const;					// 取得数据属性叶节点的枚举字符串
	virtual void SetFieldID(UINT nRow,FieldId * pFieldId);							// 设置数据属性叶节点的属性域对应
	virtual void GetFieldID(UINT nRow,FieldId * pFieldId) const;					// 取得数据属性叶节点的属性域对应

	cLeafNodeDesc * GetTargetDesc() const{return m_pTargetDesc;}

};
