#pragma once

#include <vector>
#include <list>
#include <bitset>
#include "blockallocator.h"
#ifdef DEFINE_BLOCK_ALLOCATED_LIST
enum {eOtherBlockSize=2};
#endif

typedef int NodeAdder[eDataOpMax];		
//#undef DEFINE_BLOCK_ALLOCATED_LIST
class cInstNode_Imp : public cInstNode
{
	const cUnitTreeNode * m_pUnit;	// 来源单元
	const cLeafRow * m_pRow;		// 来源行
	int m_nID;						// 实例ID
	int m_nTemplateID;				// 模板ID
	int m_nMemoryID[2];				// 内存池标志
	void * m_pUserData;
	cAttribLeafNode * m_pAttrib;
	cLeafNodeDesc *	m_pNodeDesc;
	BYTE * m_pBuffer;				// 裸值缓存
	BYTE * m_pCurrValueBuffer;		// 当前值缓存

	BOOL	m_bUpdate;				// 是否随时间更新
	DWORD	m_dwLastUpdate;			// 上次更新的时间
	cFormulaManager * m_pFormulaManager;	// 公式管理者	
	cState	*m_pState;
	//yuanke.lou at 2008.5.1.0:15 add
	TCHAR	m_szInstName[16];
	bool	m_bIfUsed;
	Attachment m_ParentAttach;

	BOOL m_bInOnBuffEnd;

#ifndef DEFINE_BLOCK_ALLOCATED_LIST
	// 挂在自己名下的联系
	std::list<Attachment> m_lstAttachment;
	// 作用在自己身上的效果
	std::list<InstEffect> m_lstEffects; 
	// 作用在自己身上的冷却时间
	std::list<EffectCold> m_lstColds;
	// 专用于技能的影响buff
	std::list<BuffEffect> m_lstBuffs;
#else
	// 挂在自己名下的联系
	block_allocated_list<Attachment,eOtherBlockSize> m_lstAttachment;
	// 作用在自己身上的效果
	block_allocated_list<InstEffect,eOtherBlockSize> m_lstEffects; 
	// 作用在自己身上的冷却时间
	block_allocated_list<EffectCold,eOtherBlockSize> m_lstColds;
	// 专用于技能的影响buff
	block_allocated_list<BuffEffect,eOtherBlockSize> m_lstBuffs;
	typedef block_allocated_list<Attachment,eOtherBlockSize>::iterator Iter_ListAttach;
	typedef block_allocated_list<Attachment,eOtherBlockSize>::const_iterator Const_Iter_ListAttach;
	typedef block_allocated_list<InstEffect,eOtherBlockSize>::iterator Iter_ListInstEffect;
	typedef block_allocated_list<InstEffect,eOtherBlockSize>::const_iterator Const_Iter_ListInstEffect;
	typedef block_allocated_list<EffectCold,eOtherBlockSize>::iterator Iter_ListEffectCold;
	typedef block_allocated_list<EffectCold,eOtherBlockSize>::const_iterator Const_Iter_ListEffectCold;
	typedef block_allocated_list<BuffEffect,eOtherBlockSize>::iterator Iter_ListBuffEffect;
	typedef block_allocated_list<BuffEffect,eOtherBlockSize>::const_iterator Const_Iter_ListBuffEffect;
#endif
	typedef std::vector<cInstPropListener *> VectorListener;
	typedef VectorListener::iterator Iter_VectorListener;
	typedef std::vector<EffectValue> VectorEffectValue;
	typedef VectorEffectValue::iterator Iter_VectorEffectValue;
	// 监视自己的老板们
	VectorListener m_PropChangeListener; 
	// 不同于影响模板的影响实例
	VectorEffectValue m_vecEffInst;
	// 用于角色的属性是否被改变的标记.
	mutable VectorDirty m_vecFieldDirty;

	BOOL LimiteAllow(const cInstNode * pDest,LimitField * pLimit)const;
	void DelTempEffects(DWORD dwCurTime);

	void BuildEffectValue(int nUnitID,NodeAdder * pNodeAdder);
	void GetEffectValue(int nUnitID,int nField,int * pnValue) const;
	void GetEffectValue(EffectValue * pEffect,int * pnValue) const;
	void GetBufferValue(BuffEffect * pEffect,int * pnValue) const;
	void ApplyEffectToAttrib(EffectValue * pEffect);

	void OnEffectBegin(cLeafRow_Imp * pRow);	// 
	void OnEffectEnd(cLeafRow_Imp * pRow);
public:
	cInstNode_Imp();
	~cInstNode_Imp();
	BOOL Init(const cUnit * pUnit,		// 单元指针							
			int nTemplateID,			// 数据模板ID
			int nInstanceID,
			cFormulaManager *pFormula=NULL);
	void Destroy();

	virtual const cUnit * GetUnit() const {return m_pUnit;}				// 取得单元节点
	virtual const cLeafRow * GetRow() const{return m_pRow;}				// 取得单元数据模板行
	virtual const void * GetBuffer() const{return m_pBuffer;}			// 取得实例缓存
	virtual int GetID()const{return m_nID;}								// 取得实例ID
	virtual int GetTemplateID()const{return m_nTemplateID;}
	virtual int GetUnitID()const {return m_pUnit->GetUnitDef()->m_nID;}	// 取得单元节点ID
	virtual const void * GetRowBuffer()const{return m_pRow?m_pRow->GetBuffer():NULL;}	// 取得单元数据模板行缓存
	virtual const cLeafNodeDesc * GetDesc() const;

	virtual const TCHAR * GetString(UINT col,BOOL bEffect = TRUE)const;					// 取得指定位置的值
	virtual int GetInteger(UINT col,BOOL bEffect = TRUE)const;							// 取得指定位置的值
	virtual float GetFloat(UINT col,BOOL bEffect = TRUE)const;							// 取得指定位置的值
	virtual const TCHAR * GetString(const TCHAR * lpszField,BOOL bEffect = TRUE)const;	// 取得指定位置的值
	virtual int GetInteger(const TCHAR * lpszField,BOOL bEffect = TRUE)const;			// 取得指定位置的值
	virtual float GetFloat(const TCHAR * lpszField,BOOL bEffect = TRUE)const;			// 取得指定位置的值

	virtual void SetString(UINT col,const TCHAR * string);					// 设置指定位置的值
	virtual void SetInteger(UINT col,int nValue,bool blimitfunction=true);							// 设置指定位置的值
	virtual void SetFloat(UINT col,float fValue,bool blimitfunction=true);							// 设置指定位置的值
	virtual void SetString(const TCHAR * lpszField,const TCHAR * string);	// 设置指定位置的值
	virtual void SetInteger(const TCHAR * lpszField,int nValue);			// 设置指定位置的值
	virtual void SetFloat(const TCHAR * lpszField,float fValue);			// 设置指定位置的值
	
	virtual UINT GetEffects()const{return m_vecEffInst.size();} 
	virtual const EffectValue * GetEffectsPtr()const{return m_vecEffInst.size()?&m_vecEffInst.front():NULL;} 
	virtual void ApplyEffectToEffect(EffectValue * pEffect);
	virtual void CancelInstEffect(EffectValue * pEffect);
	virtual void ConvertBufferToEffect(EffectValue * pEffect,BuffEffectValue * pBuff,int nParam);

	virtual UINT GetAttchments()const;							// 取得关联实例数量
	virtual const Attachment * GetAttchment(UINT n)const;		// 取得关联实例
	virtual const Attachment * GetParent()const;				// 取得父关联
	virtual int Attach(const Attachment * attach);				// 把指定实例关联到本节点
	virtual void Detach(const Attachment * attach);				// 把指定实例关联到本节点
	virtual bool IsOnAttach(const Attachment * attach);

	virtual UINT GetTempEffects()const;							// 取得临时效果数量
	virtual const InstEffect * GetTempEffect(UINT n)const;		// 取得临时效果
	//virtual int CostTemp(const Attachment * attach,BOOL bColdTime = TRUE);				// 使用指定实例作用到本节点(临时作用)
	//virtual int CostForever(const Attachment * attach,BOOL bColdTime = TRUE);			// 使用指定实例作用到本节点(永久作用)
	
	///////////////////////技能BUFF专用///////////////////////
	virtual BuffEffect * CostBuff(BuffEffect * pBuff);
	virtual int TransferBuffer(cInstNode *pThatInst);			// 将自身的BUFFER转加给指定实例
	virtual void UncostBuff(UINT nSkillInstID,const BuffEffectValue * pBuffVaue);
	virtual void ClearBuffs(int nMaxCount,cBuffEffectFilter * pFilter);		// 清除附加在实例上的符合条件的BUFF
	virtual void ClearAllBuff();											// 清除附加在实例上的所有BUFF

	virtual const void * GetCurrentValueBuff(){return m_pCurrValueBuffer;}
	virtual void ClearDirtyField(UINT nField=0);				// 清除更新标志，默认全部清除
	virtual VectorDirty &QueryDirtyField() {return m_vecFieldDirty;}
	virtual void CalculateAll(bool blimitfunction);
	void SkillBuffUpdate(DWORD dwCurTime);
	BuffEffect * ReAdjustBuffInfo(BuffEffect * pBuff);
	///////////////////////技能BUFF专用///////////////////////

	virtual int LimiteAllow(const Attachment * attach)const;
	virtual int HandleAllow(int nRelationID,cInstNode * pHandler)const;
	virtual BOOL ColdAllow(int nRelationID,cInstNode * pOper)const;	

	virtual void Update(DWORD dwCurTime);						// 更新实例
	
	virtual UINT GetLimiteDestFeilds(int nRelationID,Limit_Dest eDest,LimitField ** ppLimits) const;
	virtual UINT GetEffectDestFeilds(int nRelationID,Effect_Dest eDest,EffectValue ** ppEffets) const;
	virtual UINT GetInstEffectFeilds(EffectValue ** ppEffets) const;
	virtual UINT GetEffectDestFeilds(int nRelationID,EffectValue ** ppEffets,UINT * pnCount = NULL) const;
	virtual UINT GetEffectBuffFeilds(EDestUnit nDestUnit,BuffEffectValue ** ppEffets,UINT * pnCount = NULL) const;
	virtual UINT GetBufferEffects(BuffEffect *ppBuffers,UINT *pnCount = NULL) const;
	virtual LimitField * GetLimiteValue(int nRelationID,int nFields) const;

	virtual const LimitField * GetLimiteFeild(int nIndex) const;

	virtual BOOL IfUpdate()const{return m_bUpdate;}
	virtual void Update(BOOL bUpdate){m_bUpdate = bUpdate;}

	virtual void SetUserData(void * pUserData){m_pUserData = pUserData;}
	virtual void * GetUserData() const{return m_pUserData;}

	//yuanke.lou at 2008.5.1.0:15 add
	virtual void SetInstName(TCHAR * szName){_tcscpy(&m_szInstName[0],szName);}
	virtual TCHAR* GetInstName() { return &m_szInstName[0];}
	virtual bool  GetUsedState() const
	{
		return m_bIfUsed;
	}
	//virtual void SetFormulaManager(void * pFormulaManager){m_pFormulaManager = (cFormulaManager *)pFormulaManager;}
	virtual BOOL IfInheritTheUnit(UINT nUnitID) const;
	virtual BOOL GetRelate2Other(const cInstNode *pOther,UINT nRelate) const;
	static BOOL IfAInheritB(int nAUnitID,int nBUnitID);

	virtual VOID RegistPropListen(cInstPropListener *pListener){m_PropChangeListener.push_back(pListener);}
	virtual VOID RegistStateListen(cState *pState) {m_pState = pState;}

private:
	void RefreshOrigValue2CurrValue(){memcpy(m_pCurrValueBuffer,m_pBuffer,sizeof(m_pCurrValueBuffer));}
	UINT GetUnitIDByRelateAndDestUnit(int nReleation,int nDestUnit)const;
	int CalculateInteger(UINT col,int * pnValue,bool blimitfunction,BOOL bCaculateSub=TRUE);
	float CalculateFloat(UINT col,int * pnValue,bool blimitfunction,BOOL bCaculateSub=TRUE);
	void SetCurrValueInteger(UINT col,int nValue);
	void SetCurrValueFloat(UINT col,float nValue);
	int SimpleComputeI(int nOrigValue,int *pFormula) const;
	float SimpleComputeF(float fOrigValue,int *pFormula) const;
	void BufferTakeEffect(BuffEffect &buff);
	void BufferCalcEffect(BuffEffect &buff);
	void GetBuffValue(BuffEffect *buff);
	BuffEffect * ImmediatelyApplyBuffer(BuffEffect * pBuff,cRemain * pRemain);

	///< 下面是buffer叠加规则
	BuffEffect * ApplyBATRule_None(BuffEffect * pBuff);		// 不可释放
	BuffEffect * ApplyBATRule_CanAdd(BuffEffect * pBuff);	// 可叠加
	BuffEffect * ApplyBATRule_TimeAdd(BuffEffect * pBuff);	// 延续(时间叠加)
	BuffEffect * ApplyBATRule_Replace(BuffEffect * pBuff);	// 替换
	BuffEffect * ApplyBATRule_Quene(BuffEffect * pBuff);	// 排队
	BuffEffect * ApplyBATRule_Kill(BuffEffect * pBuff);		// 抵消
	BuffEffect * ApplyBATRule_Control(BuffEffect * pBuff);	// 抑制
	BuffEffect * ApplyBATRule_Post(BuffEffect * pBuff);		// 推后
};
