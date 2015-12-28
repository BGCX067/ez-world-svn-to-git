#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <strstream>
#include <assert.h>
#include <vector>
#include <malloc.h>
#include <math.h>

#include "LogicEngine.h"
#include "TreeNode.h"
#include "LeafNode.h"
#include "InstNode.h"
#include "theworld.h"

#define OVERLOAD
#include "overload.h"

// #define _CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>
// #ifdef _DEBUG
// #define   new   new(_NORMAL_BLOCK,   __FILE__,   __LINE__)
// #endif
//#include "pool.h"
//CObjectPool<cInstNode_Imp>	m_lstInstPool;

#define PERCENT_RAD	10000

extern cWorldImp g_NodeStreamImp;
static char	szNumber[16];
const std::string g_strSplit(",");		// 关联字段的分割符

extern void SplitString(std::string str,std::vector<std::string>& vecString,std::string spliter);
//#undef DEFINE_BLOCK_ALLOCATED_LIST
cInstNode_Imp::cInstNode_Imp()
	:m_pUnit(NULL)	
	,m_bIfUsed(false)
	,m_pRow(NULL)
	,m_nID(0)
	,m_nTemplateID(0)
	,m_pUserData(NULL)
	,m_pAttrib(NULL)
	,m_pFormulaManager(NULL)
	,m_pState(NULL)
	,m_pNodeDesc(NULL)
	,m_pBuffer(NULL)									// 裸值的缓存
	,m_pCurrValueBuffer(NULL)							// 当前属性值的缓存
	,m_bUpdate(TRUE)									// 是否随时间更新
	,m_dwLastUpdate(NULL)								// 上次更新的时间
	,m_bInOnBuffEnd(FALSE)
{
}

BOOL cInstNode_Imp::Init(const cUnit * pUnit,		// 单元指针							
						 int nTemplateID,			// 数据模板ID
						 int nInstanceID,
						 cFormulaManager *pFormula)
{
	//GetSysEfficiency()->beginRecord("实例初始化1");
	m_pUnit = (cUnitTreeNode*)pUnit;
	m_bIfUsed = false;
	m_pRow = NULL;
	m_nID = nInstanceID;
	m_pUserData = NULL;
	m_pAttrib = NULL;
	m_pFormulaManager = pFormula;
	m_pState = NULL;
	m_pNodeDesc = NULL;
	m_pBuffer = NULL;
	m_pCurrValueBuffer = NULL;
	m_bUpdate = TRUE;
	m_dwLastUpdate = g_NodeStreamImp.GetTime();
	ZeroMemory(&m_ParentAttach,sizeof(Attachment));
	// 释放容器
	m_lstAttachment.clear();
	m_lstEffects.clear();
	m_lstColds.clear();
	m_lstBuffs.clear();
	m_PropChangeListener.clear();
	m_vecEffInst.clear();
	m_vecFieldDirty.clear();
	m_pAttrib = (cAttribLeafNode*)m_pUnit->GetLeaf(Node_UnitAttrib); 
	m_pNodeDesc = m_pAttrib->GetTargetDesc(); 
	UINT nInstLength = m_pNodeDesc->GetInstLength(); 
	m_vecFieldDirty.resize(m_pNodeDesc->GetFieldCount());
	if(nInstLength)
	{
		// 裸值的缓存的空间申请
		m_pBuffer = (BYTE *)g_NodeStreamImp.GetMemory(sizeof(BYTE)*nInstLength,m_nMemoryID[0]);
		m_pCurrValueBuffer = (BYTE *)g_NodeStreamImp.GetMemory(sizeof(BYTE)*nInstLength,m_nMemoryID[1]);
		if (m_pBuffer==NULL || m_pCurrValueBuffer==NULL)
		{
			char szBuffer[256] = {0};
			sprintf(szBuffer,"从内存池中申请空间失败，单元名称[%s]",m_pUnit->GetName());
			_error(szBuffer);
			assert(0);
			return FALSE;
		}
		ZeroMemory(m_pBuffer,sizeof(BYTE)*nInstLength);
		ZeroMemory(m_pCurrValueBuffer,sizeof(BYTE)*nInstLength);
	}
	//GetSysEfficiency()->endRecord("实例初始化1");
	//GetSysEfficiency()->beginRecord("实例初始化2");
	const cLeafNode * pTemplate = m_pUnit->GetLeaf(Node_UnitTemplate); 
	if(pTemplate && nTemplateID)
	{
		m_nTemplateID = nTemplateID;
		//GetSysEfficiency()->beginRecord("实例初始化3");
		UINT nRow = pTemplate->FindRowByID(nTemplateID);
		//GetSysEfficiency()->endRecord("实例初始化3");
		assert(nRow!=-1);
		if(nRow != -1)
		{
			m_pRow = pTemplate->GetRow(nRow);
			const BYTE * pRowBuffer = (const BYTE *)m_pRow->GetBuffer();

			//把模板中的值复制到实例中
			for(UINT n=0;n<m_pNodeDesc->GetFieldCount();n++)
			{
				const LeafField * f= m_pNodeDesc->GetField(n);
				if((f->m_nInstanceOffset>=0) && (f->m_nTemplateOffset>=0))
				{
					switch(f->m_eType)
					{
					case Field_String:
						lstrcpy((char *)m_pBuffer + f->m_nInstanceOffset,(char *)pRowBuffer + f->m_nTemplateOffset);
						break;
					case Field_Enumerate:
					case Field_Boolean:
					case Field_Integer:
					case Field_Dynamic:
					case Field_Percent:
					case Field_IdField:
						*(int*)((char *)m_pBuffer + f->m_nInstanceOffset) = *(int*)((char *)pRowBuffer + f->m_nTemplateOffset);
						break;
					case Field_Float:
						*(float*)((char *)m_pBuffer + f->m_nInstanceOffset) = *(float*)((char *)pRowBuffer + f->m_nTemplateOffset);
						break;
					}
				}
			}
			if(nInstLength)
			{
				// 首先将裸值拷贝给当前值
				memcpy(m_pCurrValueBuffer,m_pBuffer,sizeof(BYTE)*nInstLength);
			}
			return TRUE;
		}
		return FALSE;
	}
	//GetSysEfficiency()->endRecord("实例初始化2");
	return TRUE;
}
void cInstNode_Imp::Destroy()
{

	//这些assert被命中说明系统可能存在内存泄露
	assert(m_lstBuffs.size()==0);
	if(m_lstBuffs.size())
	{
		_error("instance id %d buff not cleared \n",GetID());
		Iter_ListBuffEffect it;
		for(it=m_lstBuffs.begin();it!=m_lstBuffs.end();it++)
		{
			_printf("buffer templateID %d relation=%d value=%d\n",it->m_pValue->nTemplateID,it->m_pValue->nRelationID,it->m_pValue->nValue);
		}
		assert(0);
	}
	//assert(m_lstAttachment.size()==0);
	assert(m_lstEffects.size()==0);
	//assert(m_vecEffInst.size()==0);
	m_vecEffInst.clear();

	//如果当前instance attach 到 其他instance 上面
	if(m_ParentAttach.nInstHandID)
	{
		_printf("instance %d still attached to %d\n",GetID(),m_ParentAttach.nInstHandID);
		cInstNode *pparent=g_NodeStreamImp.GetInstance(m_ParentAttach.nInstHandID);
		assert(pparent);
		if(pparent)
		{
			pparent->Detach(&m_ParentAttach);
		}else
		{
			_error("parent instance %d not found!\n",m_ParentAttach.nInstHandID);
		}
	}

	//如果还有其他instance attach到当前inst
	for(;m_lstAttachment.size();)
	{
		Iter_ListAttach it=m_lstAttachment.begin();
		_printf("instance %d attached to instance %d while destory instance%d\n",it->nInstOperID,it->nInstHandID,GetID());
		Detach(&*it);
	}
	assert(m_lstAttachment.size()==0);



	if(m_pBuffer)
	{
		UINT nInstLength = m_pNodeDesc->GetInstLength();
		g_NodeStreamImp.FreeMemory(nInstLength,m_nMemoryID[0]);
		//delete []m_pBuffer;
		m_pBuffer = NULL;
	}
	if(m_pCurrValueBuffer)
	{
		UINT nInstLength = m_pNodeDesc->GetInstLength();
		g_NodeStreamImp.FreeMemory(nInstLength,m_nMemoryID[1]);
		//delete []m_pCurrValueBuffer;
		m_pCurrValueBuffer = NULL;
	}

	m_PropChangeListener.clear();
	m_vecFieldDirty.clear();


}

cInstNode_Imp::~cInstNode_Imp()
{
	//Destroy();
}

const cLeafNodeDesc * cInstNode_Imp::GetDesc() const
{
	return ((cAttribLeafNode*)m_pUnit->GetLeaf(Node_UnitAttrib))->GetTargetDesc();  
}

const TCHAR * cInstNode_Imp::GetString(UINT col,BOOL bEffect)const					// 取得指定位置的值
{
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if(pField && (pField->m_nInstanceOffset>=0) )
	{
		switch(pField->m_eType)
		{
		case Field_String:
			{
				return (char *)m_pBuffer + pField->m_nInstanceOffset;
			}
			break;
		case Field_Enumerate:
			{
				int nValue = *(int*)((char *)m_pBuffer + pField->m_nInstanceOffset);
				if(pField->m_ppStrings && pField->m_ppStrings[nValue])
					return pField->m_ppStrings[nValue]; 
				else
					return NULL;
			}
			break;
		case Field_Boolean:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Percent:
			{
				int nValue = *(int*)((char *)m_pBuffer + pField->m_nInstanceOffset);
				return _itoa(nValue,szNumber,10);
			};
			break;
		case Field_Float:
			{
				float fValue = *(float*)((char *)m_pBuffer + pField->m_nInstanceOffset);
				_stprintf(szNumber,"%.4f",fValue);
				return szNumber;
			}
			break;
		case Field_IdField:
			{
				int nValue = *(int*)((char *)m_pBuffer + pField->m_nInstanceOffset);
				cLeafNode * pSourceNode = (cLeafNode *)g_NodeStreamImp.GetRoot()->GetNodeByPath(pField->m_pFieldId->pLeafPath);
				if(pSourceNode)
				{
					UINT nRow = pSourceNode->FindRow((UINT)pField->m_pFieldId->nValField,nValue); 
					if(nRow != -1)
						return pSourceNode->GetString(nRow,pField->m_pFieldId->nDispField); 
				}
			}
			break;
		}
	}
	if(pField && (pField->m_nTemplateOffset>=0) && m_pRow)
		return m_pRow->GetString(col); 

	return NULL;
}
int cInstNode_Imp::GetInteger(UINT col,BOOL bEffect)const							// 取得指定位置的值
{
	// todo 有时候会传入非法数据,要查明为什么.
	assert(col<m_pNodeDesc->GetFieldCount());
	if(col>=m_pNodeDesc->GetFieldCount())
		return 0;

	int nRet = 0;
	BYTE *pValue = NULL;
	if (bEffect)
	{
		pValue = m_pCurrValueBuffer;
	}
	else
	{
		pValue = m_pBuffer;
	}
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if(pField && (pField->m_nInstanceOffset>=0) )
	{
		switch(pField->m_eType)
		{
		case Field_String:
			nRet = atoi((char *)pValue + pField->m_nInstanceOffset);
			break;
		case Field_Enumerate:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Boolean:
		case Field_Percent:
		case Field_IdField:
			nRet =  *(int*)((char *)pValue + pField->m_nInstanceOffset);
			break;
		case Field_Float:
			nRet = (int)(*(float*)((char *)pValue + pField->m_nInstanceOffset));
		}
	}
	else if(m_pRow && pField && (pField->m_nTemplateOffset>=0) )
		nRet = m_pRow->GetInteger(col); 

	return nRet;
}
float cInstNode_Imp::GetFloat(UINT col,BOOL bEffect)const							// 取得指定位置的值
{
	float fValue = 0.0f;
	BYTE *pValue = NULL;
	if (bEffect)
	{
		pValue = m_pCurrValueBuffer;
	}
	else
	{
		pValue = m_pBuffer;
	}
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if (!pField) return fValue;

	if(pField && (pField->m_nInstanceOffset>=0) )
	{
		switch(pField->m_eType)
		{
		case Field_String:
		case Field_Enumerate:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Boolean:
		case Field_Percent:
		case Field_IdField:
			fValue = (float)(*(int*)((char *)pValue + pField->m_nInstanceOffset));
			break;
		case Field_Float:
			fValue = (*(float*)((char *)pValue + pField->m_nInstanceOffset));
		}
	}
	else if(pField && (pField->m_nTemplateOffset>=0) )
		return m_pRow->GetFloat(col); 

	return fValue;
}

const TCHAR * cInstNode_Imp::GetString(const TCHAR * lpszField,BOOL bEffect)const	// 取得指定位置的值
{
	UINT nCol = m_pNodeDesc->FindField(lpszField); 
	if(nCol != -1)
		return GetString(nCol,bEffect);
	return NULL;
}
int cInstNode_Imp::GetInteger(const TCHAR * lpszField,BOOL bEffect)const				// 取得指定位置的值
{
	UINT nCol = m_pNodeDesc->FindField(lpszField); 
	if(nCol != -1)
		return GetInteger(nCol,bEffect);
	return 0;
}
float cInstNode_Imp::GetFloat(const TCHAR * lpszField,BOOL bEffect)const				// 取得指定位置的值
{
	UINT nCol = m_pNodeDesc->FindField(lpszField); 
	if(nCol != -1)
		return GetFloat(nCol,bEffect);
	return 0.0f;
}


void cInstNode_Imp::SetString(UINT col,const TCHAR * string)			// 设置指定位置的值
{
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if(pField && (pField->m_nInstanceOffset >=0))
	{
		switch(pField->m_eType)
		{
		case Field_String:
			{
				int nLen = min(lstrlen(string),pField->m_nLength-1);
				lstrcpyn((char *)m_pBuffer + pField->m_nInstanceOffset,string,nLen+1);
				*((char *)m_pBuffer + pField->m_nInstanceOffset + nLen) = 0;

				lstrcpyn((char *)m_pCurrValueBuffer + pField->m_nInstanceOffset,string,nLen+1);
				*((char *)m_pCurrValueBuffer + pField->m_nInstanceOffset + nLen) = 0;
			}
			break;
		case Field_Enumerate:
			{
				int n = 0;
				if(pField->m_ppStrings)
				{
					for(n=0;n<(pField->m_nMax-pField->m_nMin);n++)
					{
						if(!_stricmp(pField->m_ppStrings[n],string))
							break;
					}
				}
				if(n==(pField->m_nMax-pField->m_nMin))
					n = 0;
				*(int*)((char *)m_pBuffer + pField->m_nInstanceOffset) = n;

				*(int*)((char *)m_pCurrValueBuffer + pField->m_nInstanceOffset) = n;
			}
			break;
		case Field_Boolean:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Percent:
			{
				*(int*)((char *)m_pBuffer + pField->m_nInstanceOffset) = _ttoi(string);

				*(int*)((char *)m_pCurrValueBuffer + pField->m_nInstanceOffset) = _ttoi(string);
			}
			break;

		case Field_Float:
			{
				*(float*)((char *)m_pBuffer + pField->m_nInstanceOffset) = (float)_tstof(string);

				*(float*)((char *)m_pCurrValueBuffer + pField->m_nInstanceOffset) = (float)_tstof(string);
			}
			break;
		case Field_IdField:
			{
				int nValue = *(int*)((char *)m_pBuffer + pField->m_nInstanceOffset);
				cLeafNode * pSourceNode = (cLeafNode *)g_NodeStreamImp.GetRoot()->GetNodeByPath(pField->m_pFieldId->pLeafPath);
				if(pSourceNode)
				{
					UINT nRow = pSourceNode->FindRow(pField->m_pFieldId->nDispField,string); 
					if(nRow != -1)
					{
						*(int*)((char *)m_pBuffer + pField->m_nInstanceOffset) = pSourceNode->GetInteger(nRow,(UINT)pField->m_pFieldId->nValField);
						
						*(int*)((char *)m_pCurrValueBuffer + pField->m_nInstanceOffset) = pSourceNode->GetInteger(nRow,(UINT)pField->m_pFieldId->nValField);
					}
				}
			}
			break;		
		}

		const cFieldEvent * e = m_pUnit->GetFieldEvent(col);
		if(e)
			e->Event(this,col); 
	}
}

void cInstNode_Imp::SetInteger(UINT col,int nValue,bool blimitfunction)							// 设置指定位置的值
{
	assert(!(13 == m_pUnit->GetUnitDef()->m_nID && col == 12 && nValue<=1));//可能出现的最大生命值为0的ASSERT，zhaoyangma-20100826
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if(pField && (pField->m_nInstanceOffset>=0) )
	{
		switch(pField->m_eType)
		{
		case Field_String:
			lstrcpy((char *)m_pBuffer + pField->m_nInstanceOffset,_itoa(nValue,szNumber,10));
			break;
		case Field_Enumerate:
		case Field_Boolean:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Percent:
		case Field_IdField:
			*(int*)((char *)m_pBuffer + pField->m_nInstanceOffset) = nValue;
			break;
		case Field_Float:
			*(float*)((char *)m_pBuffer + pField->m_nInstanceOffset) = (float)nValue;
			break;
		}

		const cFieldEvent * e = m_pUnit->GetFieldEvent(col);
		if(e)
			e->Event(this,col); 

		// 重新计算自己
		ClearDirtyField(col);
		CalculateInteger(col,NULL,blimitfunction,FALSE);
		// 有其他字段依赖自己
		if(m_pAttrib->IfHasPositiveDepend(col))
		{
			// 先计算关联的字段
			std::set<UINT> &setPositive = m_pAttrib->GetPositiveDependFields(col);
			std::set<UINT>::iterator iBeg = setPositive.begin();
			while (iBeg != setPositive.end())
			{
				int nDependCol = *iBeg;
				// 清除他的计算标志，让其重新计算
				ClearDirtyField(nDependCol);
				CalculateFloat(nDependCol,NULL,blimitfunction,FALSE);
				iBeg ++;
			}
		}
		m_vecFieldDirty[col] = true;
		int nSize = m_PropChangeListener.size();
		if (nSize)
		{
			for (int i=0;i<nSize;i++)
			{
				m_PropChangeListener[i]->FncPropChange(this,col);
			}
		}
	}
}

void cInstNode_Imp::SetFloat(UINT col,float fValue,bool blimitfunction)							// 设置指定位置的值
{
	assert(!(13 == m_pUnit->GetUnitDef()->m_nID && col == 12 && fValue<=1));//可能出现的最大生命值为0的ASSERT，zhaoyangma-20100826
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if(pField && (pField->m_nInstanceOffset>=0) )
	{
		switch(pField->m_eType)
		{
		case Field_String:
			break;
		case Field_Enumerate:
		case Field_Boolean:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Percent:
		case Field_IdField:
			*(int*)((char *)m_pBuffer + pField->m_nInstanceOffset) = (int)fValue/*(int)(fValue+.5f)*/;
			break;
		case Field_Float:
			*(float*)((char *)m_pBuffer + pField->m_nInstanceOffset) = fValue;
			break;
		}

		const cFieldEvent * e = m_pUnit->GetFieldEvent(col);
		if(e)
			e->Event(this,col); 

		// 重新计算自己
		ClearDirtyField(col);
		CalculateFloat(col,NULL,blimitfunction,FALSE);
		// 有其他字段依赖自己
		if(m_pAttrib->IfHasPositiveDepend(col))
		{
			// 先计算关联的字段
			std::set<UINT> &setPositive = m_pAttrib->GetPositiveDependFields(col);
			std::set<UINT>::iterator iBeg = setPositive.begin();
			while (iBeg != setPositive.end())
			{
				int nDependCol = *iBeg;
				// 清除他的计算标志，让其重新计算
				ClearDirtyField(nDependCol);
				CalculateFloat(nDependCol,NULL,blimitfunction,FALSE);
				iBeg ++;
			}
		}
		m_vecFieldDirty[col] = true;
		int nSize = m_PropChangeListener.size();
		if (nSize)
		{
			for (int i=0;i<nSize;i++)
			{
				m_PropChangeListener[i]->FncPropChange(this,col);
			}
		}
	}	
}

void cInstNode_Imp::SetString(const TCHAR * lpszField,const TCHAR * string)	// 设置指定位置的值
{
	UINT nCol = m_pNodeDesc->FindField(lpszField); 
	if(nCol != -1)
		return SetString(nCol,string);
}
void cInstNode_Imp::SetInteger(const TCHAR * lpszField,int nValue)				// 设置指定位置的值
{
	UINT nCol = m_pNodeDesc->FindField(lpszField); 
	if(nCol != -1)
		return SetInteger(nCol,nValue);
}
void cInstNode_Imp::SetFloat(const TCHAR * lpszField,float fValue)				// 设置指定位置的值
{
	UINT nCol = m_pNodeDesc->FindField(lpszField); 
	if(nCol != -1)
		return SetFloat(nCol,fValue);
}


UINT cInstNode_Imp::GetAttchments()const						// 取得关联实例数量
{
	return m_lstAttachment.size(); 
}
const Attachment * cInstNode_Imp::GetAttchment(UINT n)const		// 取得关联实例
{
	if(n<m_lstAttachment.size())
	{
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
		std::list<Attachment>::const_iterator i = m_lstAttachment.begin(); 
#else
		Const_Iter_ListAttach i = m_lstAttachment.begin(); 
#endif
		for(UINT x=0;x<n;x++)
			i ++;
		return &(*i);
	}
	return NULL;
}
const Attachment * cInstNode_Imp::GetParent()const				// 取得父关联
{
	if(m_ParentAttach.nRelationID)
		return &m_ParentAttach;
	return NULL;
}

int cInstNode_Imp::Attach(const Attachment * attach)			// 把指定实例关联到本节点
{
	assert(GetID()==attach->nInstHandID);
	// 1.检验限制,是否可以通过
	int nRet = LimiteAllow(attach);
	if( nRet!= 0)
		return nRet;
	
	// 2.附加效果
	m_lstAttachment.push_back(*attach);

	// 设置目标实例的父连接
	cInstNode_Imp * pOper = (cInstNode_Imp *)g_NodeStreamImp.GetInstance(attach->nInstOperID);
	assert(pOper->m_bIfUsed==false);
	//设置目标实例为被使用状态
	pOper->m_bIfUsed = true;
	//pOper->m_ParentAttach.nInstHandID = attach->nInstHandID;
	//pOper->m_ParentAttach.nInstOperID = m_nID;
	//pOper->m_ParentAttach.nRelationID = attach->nRelationID;
	pOper->m_ParentAttach=*attach;
	
	return 0;
}
void cInstNode_Imp::Detach(const Attachment * attach)			// 把指定实例关联到本节点
{
	// 寻找需要卸载的效果
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
	std::list<Attachment>::iterator i = m_lstAttachment.begin(); 
#else
	Iter_ListAttach i = m_lstAttachment.begin(); 
#endif
	while(i != m_lstAttachment.end())
	{
		if( (i->nInstHandID == attach->nInstHandID) 
			&&(i->nInstOperID == attach->nInstOperID) 
			&&(i->nRelationID == attach->nRelationID) )
		{
			// 设置目标实例的父连接
			cInstNode_Imp * pOper = (cInstNode_Imp *)g_NodeStreamImp.GetInstance(attach->nInstOperID); 
			//如果实例pOper已被删除的话，不做回收处理
			if (pOper != NULL)
			{
				//设置目标实例为被未使用状态
				pOper->m_bIfUsed = false;
				ZeroMemory(&pOper->m_ParentAttach,sizeof(Attachment));
			}else
			{
				assert(0);
			}
			
			m_lstAttachment.erase(i); 
			return;
		}
		i ++;
	}
	assert(0);
	_error("inst %d not attached while detach!\n",GetID());
}
bool cInstNode_Imp::IsOnAttach(const Attachment * attach)			// 把指定实例关联到本节点
{
	// 寻找需要卸载的效果
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
	std::list<Attachment>::iterator i = m_lstAttachment.begin(); 
#else
	Iter_ListAttach i = m_lstAttachment.begin(); 
#endif
	while(i != m_lstAttachment.end())
	{
		if( (i->nInstHandID == attach->nInstHandID) 
			&&(i->nInstOperID == attach->nInstOperID) 
			&&(i->nRelationID == attach->nRelationID) )
		{
			// 设置目标实例的父连接
			cInstNode_Imp * pOper = (cInstNode_Imp *)g_NodeStreamImp.GetInstance(attach->nInstOperID); 
			//如果实例pOper已被删除的话，不做回收处理
			if (pOper != NULL)
			{
				return true;
			}
		}
		i ++;
	}
	return false;
}

UINT cInstNode_Imp::GetTempEffects()const						// 取得临时效果数量
{
	return m_lstEffects.size();
}
const InstEffect * cInstNode_Imp::GetTempEffect(UINT n)const		// 取得临时效果
{
	if(n<m_lstEffects.size())
	{
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
		std::list<InstEffect>::const_iterator i = m_lstEffects.begin(); 
#else
		Const_Iter_ListInstEffect i = m_lstEffects.begin(); 
#endif
		for(UINT x=0;x<n;x++)
			i ++;
		return &(*i);
	}
	return NULL;
}
//int cInstNode_Imp::CostTemp(const Attachment * attach,BOOL bColdTime)			// 使用指定实例作用到本节点
//{
//	// 1.检验限制,是否可以通过
//	int nRet = LimiteAllow(attach);
//	if( nRet!= 0)
//		return nRet;
//	
//	// 2.取得实例
//	const cInstNode * pOper = g_NodeStreamImp.GetInstance(attach->nInstOperID);
//
//	// 3.附加效果
//	InstEffect _e;
//	cRemain * pRemain = g_NodeStreamImp.GetRemain(); 
//	if(pRemain)
//		pRemain->GetEffectTime(g_NodeStreamImp.GetInstance(attach->nInstOperID),&_e);
//
//	// 4.如果是有序列的效果,同序列高级别替换低级别的.如果无序列,模板ID相同的替换.
//	BOOL bReplace = FALSE;
//#ifndef DEFINE_BLOCK_ALLOCATED_LIST
//	std::list<InstEffect>::iterator i = m_lstEffects.begin(); 
//#else
//	Iter_ListInstEffect i = m_lstEffects.begin(); 
//#endif
//	while(i != m_lstEffects.end())
//	{
//		const InstEffect& e =  *i;
//		if( ((_e.nSerialID == 0) && (_e.nTemplateID == e.nTemplateID))
//			||((_e.nSerialID != 0) && (_e.nSerialID == e.nSerialID) && (_e.nSerialLevel>=e.nSerialLevel)) )
//		{
//			bReplace = TRUE;
//			*i = _e;
//			break;
//		}
//		i++;
//	}
//	if(!bReplace)
//		m_lstEffects.push_back(_e); 
//	
//
//	// 5.改变消耗域
//	OnEffectBegin((cLeafRow_Imp *)_e.pRow);
//
//	// 6.附加冷却时间
//	if(bColdTime)
//	{
//		EffectCold ec;
//		if(pRemain)
//		{
//			pRemain->GetColdTime(g_NodeStreamImp.GetInstance(attach->nInstOperID),&ec);
//			if(ec.dwColdTime > 0)
//				m_lstColds.push_back(ec);
//		}
//	}
//	return 0;
//}
//int cInstNode_Imp::CostForever(const Attachment * attach,BOOL bColdTime)			// 使用指定实例作用到本节点
//{
//	// 1.检验限制,是否可以通过
//	int nRet = LimiteAllow(attach);
//	if( nRet!= 0)
//		return nRet;
//
//	const cInstNode * pOper = g_NodeStreamImp.GetInstance(attach->nInstOperID);
//	cLeafRow_Imp * pRow = (cLeafRow_Imp*)pOper->GetRow();
//	UINT nEffects = pRow->GetEffects();
//	EffectValue ** ppEffects = (EffectValue **)pRow->GetEffectPtr(); 
//	for(UINT n=0;n<nEffects;n++)
//	{
//		switch(ppEffects[n]->eDest)
//		{
//		case Dest_Attrib:
//			ApplyEffectToAttrib(ppEffects[n]);
//			break;
//		case Dest_Limit:
//			// 暂时不考虑
//			break;
//		case Dest_Effect:
//			ApplyEffectToEffect(ppEffects[n]);
//			break;
//		}
//	}	
//
//	// 4.附加冷却时间
//	if(bColdTime)
//	{
//		EffectCold ec;
//		cRemain * pRemain = g_NodeStreamImp.GetRemain(); 
//		if(pRemain)
//		{
//			pRemain->GetColdTime(pOper,&ec);
//			if(ec.dwColdTime > 0)
//				m_lstColds.push_back(ec);
//		}
//	}
//
//	return 0;
//}

BuffEffect * cInstNode_Imp::CostBuff(BuffEffect * pBuff)
{
	// 1.应用叠加规则
	return ReAdjustBuffInfo(pBuff);
}

// 将自身的BUFFER转加给指定实例
int cInstNode_Imp::TransferBuffer(cInstNode *pThatInst)
{
	int nCnt = 0;
	BuffEffect bes[MAX_BUFFER_EFFECTS] = {0};
	nCnt = GetBufferEffects(bes);
	for (int i=0;i<nCnt;i++)
	{
		bes[i].m_bEnable = false;
		pThatInst->CostBuff(&bes[i]);
	}
	return nCnt;
}

void cInstNode_Imp::UncostBuff(UINT nSkillInstID,const BuffEffectValue * pBuffVaue)
{
	cRemain * pRemain = g_NodeStreamImp.GetRemain(); 
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
	std::list<BuffEffect>::iterator i = m_lstBuffs.begin();
#else
	Iter_ListBuffEffect i = m_lstBuffs.begin();
#endif
	while (i != m_lstBuffs.end())
	{
		BuffEffect *pBuffer = &(*i);
		if(pBuffVaue == pBuffer->m_pValue && nSkillInstID == pBuffer->m_nSkillInstID)
		{
			//bool bAssert = false;
			if(pBuffer->m_bEnable)
			{
				// 对于持续生效的要减去生效值
				if(pBuffer->m_pValue->eBuffEffectType != BuffEffect_InTime)
				{
					pBuffer->m_bEnable = FALSE;	// 先失效
					if(pBuffer->m_pValue->eBuffEffectType == BuffEffect_Persistence)
					{
						float fValue = GetFloat(pBuffer->m_pValue->nOperField,FALSE);
						SetFloat(pBuffer->m_pValue->nOperField,fValue);

						if ( (pBuffer->m_pValue->eEvType == EVT_State) && m_pState )
						{
							int nValue = GetInteger(pBuffer->m_pValue->nOperField);
							UINT nDefault = m_pState->GetDefaultState();
							m_pState->ChangeState(this,nValue,nDefault);
						}
					}
					if(pRemain)
					{
						assert(m_bInOnBuffEnd == FALSE);
						m_bInOnBuffEnd = TRUE;
						pRemain->OnBuffEnd(this,&pBuffer);
						m_bInOnBuffEnd = FALSE;
						//bAssert = true;
					}
				}
			}
			//assert(bAssert && _T("数据不安全性异常"));
			assert(m_bInOnBuffEnd == FALSE);
			if(pBuffer->m_pBuffEvent)
				pBuffer->m_pBuffEvent->OnBuffErased(pBuffer);
			m_lstBuffs.erase(i);
			return;
		}
		i++;
	}
	_error("uncost buff not found instanceid=%d skillinst=%d template=%d!\n",GetID(),nSkillInstID,pBuffVaue->nTemplateID);
	assert(0);
}

//不去除还未生效的buff
void cInstNode_Imp::ClearBuffs(int nMaxCount,cBuffEffectFilter * pFilter)
{
	cRemain * pRemain = g_NodeStreamImp.GetRemain(); 
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
	std::list<BuffEffect>::iterator i = m_lstBuffs.begin();
	std::list<BuffEffect>::iterator _i;
#else
	Iter_ListBuffEffect i = m_lstBuffs.begin();
	Iter_ListBuffEffect _i;
#endif
	
	int nRealCount = 0;
	while (i != m_lstBuffs.end())
	{
		BuffEffect *pBuffer = &(*i);

		// 即使还未生效，也应该清除掉。
		BOOL bAccordWith = pFilter?pFilter->IfAccordWith(pBuffer):TRUE;
		if(bAccordWith)
		{
			// 对于持续生效的要减去生效值
			if(pBuffer->m_pValue->eBuffEffectType != BuffEffect_InTime)
			{
				pBuffer->m_bEnable = FALSE;	// 先失效
				if(pBuffer->m_pValue->eBuffEffectType == BuffEffect_Persistence)
				{
					float fValue = GetFloat(pBuffer->m_pValue->nOperField,FALSE);
					SetFloat(pBuffer->m_pValue->nOperField,fValue);

					if ( (pBuffer->m_pValue->eEvType == EVT_State) && m_pState )
					{
						int nValue = GetInteger(pBuffer->m_pValue->nOperField);
						UINT nDefault = m_pState->GetDefaultState();
						m_pState->ChangeState(this,nValue,nDefault);
					}
				}
				//bool bAssert = false;
				if(pRemain)
				{
					assert(m_bInOnBuffEnd == FALSE);
					m_bInOnBuffEnd = TRUE;
					pRemain->OnBuffEnd(this,&pBuffer);
					m_bInOnBuffEnd = FALSE;
					//bAssert = true;
				}

				//assert(bAssert && _T("数据不安全性异常"));
				// 删之前先把迭代器加过去.
				_i = i;
				i++;
				assert(m_bInOnBuffEnd == FALSE);

				if(pBuffer->m_pBuffEvent)
					pBuffer->m_pBuffEvent->OnBuffErased(pBuffer);
				m_lstBuffs.erase(_i); 

				// 如果已经删够了数量就返回.
				nRealCount ++;
				if(nMaxCount>0 && nRealCount>=nMaxCount)
					return;
					
				continue;
			}	
		}
		i++;
	}
}

void cInstNode_Imp::ClearAllBuff()
{
	cRemain * pRemain = g_NodeStreamImp.GetRemain(); 
	if (m_lstBuffs.size() > 0)
	{
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
		std::list<BuffEffect>::iterator i = m_lstBuffs.begin();
#else
		Iter_ListBuffEffect i = m_lstBuffs.begin();
#endif

		int nRealCount = 0;
		while (i != m_lstBuffs.end())
		{
			BuffEffect *pBuffer =  &(*i);
			if(pBuffer->m_bEnable)
			{
				if(pBuffer->m_pValue->eBuffEffectType != BuffEffect_InTime)
				{
					pBuffer->m_bEnable = FALSE;	// 先失效
					if(pBuffer->m_pValue->eBuffEffectType == BuffEffect_Persistence)
					{
						float fValue = GetFloat(pBuffer->m_pValue->nOperField,FALSE);
						SetFloat(pBuffer->m_pValue->nOperField,fValue);

						if ( (pBuffer->m_pValue->eEvType == EVT_State) && m_pState )
						{
							int nValue = GetInteger(pBuffer->m_pValue->nOperField);
							UINT nDefault = m_pState->GetDefaultState();
							m_pState->ChangeState(this,nValue,nDefault);
						}
					}
				}
			}
			else
			{
				// 该处已经得到验证
				//assert(0 && "ClearAllBuff 时存在还未生效的 buffer");	
			}

			if(pRemain)
			{
				assert(m_bInOnBuffEnd == FALSE);
				m_bInOnBuffEnd = TRUE;
				pRemain->OnBuffEnd(this,&pBuffer);
				m_bInOnBuffEnd = FALSE;
			}
			if(pBuffer->m_pBuffEvent)
				pBuffer->m_pBuffEvent->OnBuffErased(pBuffer);
			i ++;
		}

	
		m_lstBuffs.clear();
	}
}


void cInstNode_Imp::GetBuffValue(BuffEffect * pBuff)
{
	if(pBuff->m_pValue->eEvType == EVT_Script)
	{
		//这里先固定传参
		cInstNode * pSrcInst = (cInstNode *)g_NodeStreamImp.GetInstance(pBuff->m_nInstID);
		if(pSrcInst && pBuff->m_pValue->szRefField[0])
		{
			pBuff->m_nValue = m_pFormulaManager->GetSkillFormulaManager()->Formula(pBuff->m_pValue->szRefField,
				pSrcInst,this,pBuff->m_pValue->nParam3,pBuff->m_pValue->nParam4);
		}
		else
		{
			pBuff->m_nValue = 0;
		}
	}
	else if(pBuff->m_pValue->eEvType == EVT_State)
	{//状态机
		pBuff->m_nValue = (float)pBuff->m_pValue->nValue - 1;
	}
	else
	{//常数
		pBuff->m_nValue = (float)pBuff->m_pValue->nValue;
	}
}


BuffEffect * cInstNode_Imp::ImmediatelyApplyBuffer(BuffEffect * pBuff,cRemain * pRemain)
{
	GetBuffValue(pBuff);

	if(pBuff->m_pValue->eEvType != EVT_State)
	{
		BufferTakeEffect(*pBuff);
	}
	else
	{
		int nValue = GetInteger(pBuff->m_pValue->nOperField);
		m_pState->ChangeState(this,nValue,pBuff->m_pValue->nValue-1);
	}

	if(pRemain)
	{
		// 即时Buff不用失效
		assert(m_bInOnBuffEnd == FALSE);
		m_bInOnBuffEnd = TRUE;
		pRemain->OnBuffEnd(this,&pBuff);
		m_bInOnBuffEnd = FALSE;
	}
	return NULL;
}

BuffEffect * cInstNode_Imp::ReAdjustBuffInfo(BuffEffect * pBuff)
{
	if(!pBuff) return NULL;
	cRemain * pRemain = g_NodeStreamImp.GetRemain(); 
	// 对于即时生效的，不做存储直接计算
	if (pBuff->m_pValue->eBuffEffectType == BuffEffect_InTime && pBuff->m_pValue->nDelayTime == 0)
	{
		return ImmediatelyApplyBuffer(pBuff,pRemain);
	}

	///< 应用相应的叠加规则
	switch(pBuff->m_pValue->eAddType)
	{
	case BAT_None:		// 无法释放
		return ApplyBATRule_None(pBuff);
	case BAT_CanAdd:	// 可叠加
		return ApplyBATRule_CanAdd(pBuff);
	case BAT_TimeAdd:	// 延续(时间叠加)
		return ApplyBATRule_TimeAdd(pBuff);
	case BAT_Replace:	// 替换
		return ApplyBATRule_Replace(pBuff);
	case BAT_Quene:		// 排队
		return ApplyBATRule_Quene(pBuff);
	case BAT_Kill:		// 抵消
        return ApplyBATRule_Kill(pBuff);
	case BAT_Control:	// 抑制
		return ApplyBATRule_Control(pBuff);
	case BAT_Post:		// 推后
		return ApplyBATRule_Post(pBuff);
	}
	return NULL;
}

// 不可释放
BuffEffect * cInstNode_Imp::ApplyBATRule_None(BuffEffect * pBuff)
{
	BuffEffect * pRet = NULL;
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
	std::list<BuffEffect>::iterator i = m_lstBuffs.begin(); 
#else
	Iter_ListBuffEffect i = m_lstBuffs.begin(); 
#endif
	while(i != m_lstBuffs.end())
	{
		const BuffEffect& b =  *i;
		// 最基本的前提是同类技能
		if(b.m_pValue->nOwerTypeID == pBuff->m_pValue->nOwerTypeID 
			&& b.m_pValue->nDestUnit == pBuff->m_pValue->nDestUnit
			&& b.m_pValue->eOp == pBuff->m_pValue->eOp
			&& b.m_pValue->eDest == pBuff->m_pValue->eDest
			&& b.m_pValue->nOperField == pBuff->m_pValue->nOperField)
		{
			return NULL;
		}
		i++;
	}

	BuffEffect buff = {0};
	memcpy(&buff,pBuff,sizeof(BuffEffect));
	buff.m_dwBeginTime = g_NodeStreamImp.GetTime()+buff.m_pValue->nDelayTime;
	m_lstBuffs.push_back(buff);
	pRet = &(m_lstBuffs.back());
	return pRet;
}

// 可叠加
BuffEffect * cInstNode_Imp::ApplyBATRule_CanAdd(BuffEffect * pBuff)
{
	BuffEffect * pRet = NULL;
	int nCnt = 0;
	int nMinLevel=0,nMaxLevel=0;
	int nSameInstCnt = 0;
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
	std::list<BuffEffect>::iterator i = m_lstBuffs.begin(); 
#else
	Iter_ListBuffEffect i = m_lstBuffs.begin(); 
#endif
	while(i != m_lstBuffs.end())
	{
		const BuffEffect& b =  *i;
		// 最基本的前提是同类技能
		if(b.m_pValue->nOwerTypeID == pBuff->m_pValue->nOwerTypeID 
			&& b.m_pValue->nDestUnit == pBuff->m_pValue->nDestUnit
			&& b.m_pValue->eOp == pBuff->m_pValue->eOp
			&& b.m_pValue->eDest == pBuff->m_pValue->eDest
			&& b.m_pValue->nOperField == pBuff->m_pValue->nOperField)
		{
			nCnt++;
			nMinLevel = b.m_pValue->nLever<nMinLevel?b.m_pValue->nLever:nMinLevel;
			nMaxLevel = b.m_pValue->nLever>nMaxLevel?b.m_pValue->nLever:nMaxLevel;

			if(b.m_nInstID == pBuff->m_nInstID)
				nSameInstCnt++;
		}
		i++;
	}

	// 级别规则限制
	if(pBuff->m_pValue->eLeverRule != LR_None && 
		((pBuff->m_pValue->eLeverRule==LR_SameLever && nMinLevel==pBuff->m_pValue->nLever)
		|| (pBuff->m_pValue->eLeverRule==LR_HigherLever && nMaxLevel>pBuff->m_pValue->nLever)
		|| (pBuff->m_pValue->eLeverRule==LR_LowerLever && nMinLevel<pBuff->m_pValue->nLever)))
		return NULL;

	// 数量规则限制
	if(pBuff->m_pValue->nNumRule && 
		((pBuff->m_pValue->eSourceRule==SR_None && nCnt+1>pBuff->m_pValue->nNumRule)
		|| (pBuff->m_pValue->eSourceRule==SR_SameInst && nSameInstCnt+1>pBuff->m_pValue->nNumRule)))
		return NULL;

	BuffEffect buff = {0};
	memcpy(&buff,pBuff,sizeof(BuffEffect));
	buff.m_dwBeginTime = g_NodeStreamImp.GetTime()+buff.m_pValue->nDelayTime;
	m_lstBuffs.push_back(buff);
	pRet = &(m_lstBuffs.back());
	return pRet;
}

// 延续(时间叠加)
BuffEffect * cInstNode_Imp::ApplyBATRule_TimeAdd(BuffEffect * pBuff)
{
	BuffEffect * pRet = NULL;
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
	std::list<BuffEffect>::iterator i = m_lstBuffs.begin(); 
#else
	Iter_ListBuffEffect i = m_lstBuffs.begin(); 
#endif
	while(i != m_lstBuffs.end())
	{
		BuffEffect& b =  *i;
		// 最基本的前提是同类技能
		if(b.m_pValue->nOwerTypeID == pBuff->m_pValue->nOwerTypeID 
			&& b.m_pValue->nDestUnit == pBuff->m_pValue->nDestUnit
			&& b.m_pValue->eOp == pBuff->m_pValue->eOp
			&& b.m_pValue->eDest == pBuff->m_pValue->eDest
			&& b.m_pValue->nOperField == pBuff->m_pValue->nOperField)
		{
			// 级别规则限制
			if(pBuff->m_pValue->eLeverRule != LR_None && 
				((pBuff->m_pValue->eLeverRule==LR_SameLever && b.m_pValue->nLever!=pBuff->m_pValue->nLever)
				|| (pBuff->m_pValue->eLeverRule==LR_HigherLever && b.m_pValue->nLever>pBuff->m_pValue->nLever)
				|| (pBuff->m_pValue->eLeverRule==LR_LowerLever && b.m_pValue->nLever<pBuff->m_pValue->nLever))) 
			{
				return NULL;
			}

			b.m_dwRemainTime += pBuff->m_pValue->nPersistenceTime;
			cRemain * pRemain = g_NodeStreamImp.GetRemain();
			if (pRemain && b.m_bEnable)
			{
				BuffEffect *pBuff = &b;
				pRemain->OnBuffTimeUpt(this,&pBuff);
			}
			return NULL;
		}
		i++;
	}
	BuffEffect buff;
	memcpy(&buff,pBuff,sizeof(BuffEffect));
	buff.m_dwBeginTime = g_NodeStreamImp.GetTime();
	buff.m_dwRemainTime += pBuff->m_pValue->nPersistenceTime;
	m_lstBuffs.push_back(buff);
	pRet = &(m_lstBuffs.back());
	return pRet;
}

// 替换
BuffEffect * cInstNode_Imp::ApplyBATRule_Replace(BuffEffect * pBuff)
{
	BuffEffect * pRet = NULL;
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
	std::list<BuffEffect>::iterator i = m_lstBuffs.begin(); 
#else
	Iter_ListBuffEffect i = m_lstBuffs.begin(); 
#endif
	while(i != m_lstBuffs.end())
	{
		BuffEffect& b =  *i;
		// 最基本的前提是同类技能
		if(b.m_pValue->nOwerTypeID == pBuff->m_pValue->nOwerTypeID 
			&& b.m_pValue->nDestUnit == pBuff->m_pValue->nDestUnit
			&& b.m_pValue->eOp == pBuff->m_pValue->eOp
			&& b.m_pValue->eDest == pBuff->m_pValue->eDest
			&& b.m_pValue->nOperField == pBuff->m_pValue->nOperField)
		{
			// 级别规则限制
			if(pBuff->m_pValue->eLeverRule != LR_None && 
				((pBuff->m_pValue->eLeverRule==LR_SameLever && b.m_pValue->nLever!=pBuff->m_pValue->nLever)
				|| (pBuff->m_pValue->eLeverRule==LR_HigherLever && b.m_pValue->nLever>pBuff->m_pValue->nLever)
				|| (pBuff->m_pValue->eLeverRule==LR_LowerLever && b.m_pValue->nLever<pBuff->m_pValue->nLever))) 
			{
				return NULL;
			}
			cRemain * pRemain = g_NodeStreamImp.GetRemain();
			// 先将旧的影响去除
			if (b.m_bEnable)
			{
				b.m_bEnable = FALSE;
				
				// 周期作用的,都是影响到实际值.
				// 持续作用的,都是影响到附加值.只有持续作用的,才需要把当前值恢复.
				if(b.m_pValue->eBuffEffectType != BuffEffect_Cycle)
				{
					float fValue = GetFloat(b.m_pValue->nOperField,FALSE);
					SetFloat(b.m_pValue->nOperField,fValue,(pBuff->m_pValue->nOperField!=b.m_pValue->nOperField));
				}

				// 替换Buff不用让旧的失效,不管他有没有生效,先BufferEnd
				cRemain * pRemain = g_NodeStreamImp.GetRemain();
				assert(m_bInOnBuffEnd == FALSE);
				m_bInOnBuffEnd = TRUE;
				BuffEffect *pcurBuff = &b;
				pRemain->OnBuffEnd(this,&pcurBuff);
				m_bInOnBuffEnd = FALSE;
				b.m_bEnable = TRUE;
			}
			else
			{
				// 替换Buff不用让旧的失效,不管他有没有生效,先BufferEnd
				cRemain * pRemain = g_NodeStreamImp.GetRemain();
				assert(m_bInOnBuffEnd == FALSE);
				m_bInOnBuffEnd = TRUE;
				BuffEffect *pcurBuff = &b;
				pRemain->OnBuffEnd(this,&pcurBuff);
				m_bInOnBuffEnd = FALSE;
			}

			if(b.m_pBuffEvent)
				b.m_pBuffEvent->OnBuffErased(&b);


			// 仍旧是以前的指针
			memcpy(&b,pBuff,sizeof(BuffEffect));
			b.m_dwBeginTime = g_NodeStreamImp.GetTime();
			return &b;
		}
		i++;
	}
	BuffEffect buff = {0};
	memcpy(&buff,pBuff,sizeof(BuffEffect));
	buff.m_dwBeginTime = g_NodeStreamImp.GetTime()+buff.m_pValue->nDelayTime;
	m_lstBuffs.push_back(buff);
	pRet = &(m_lstBuffs.back());
	return pRet;
}

// 排队
BuffEffect * cInstNode_Imp::ApplyBATRule_Quene(BuffEffect * pBuff)
{
	///< 排队和可叠加的判断规则相同,只是生效的规则不同
	return ApplyBATRule_CanAdd(pBuff);
}
// 抵消
BuffEffect * cInstNode_Imp::ApplyBATRule_Kill(BuffEffect * pBuff)
{
	BuffEffect * pRet = NULL;
	return pRet;
}
// 抑制
BuffEffect * cInstNode_Imp::ApplyBATRule_Control(BuffEffect * pBuff)
{
	BuffEffect * pRet = NULL;
	return pRet;
}
// 推后
BuffEffect * cInstNode_Imp::ApplyBATRule_Post(BuffEffect * pBuff)
{
	BuffEffect * pRet = NULL;
	return pRet;
}
// 直接加到实例属性值上
void cInstNode_Imp::ApplyEffectToAttrib(EffectValue * pEffect)
{
	const LeafField * pField = m_pNodeDesc->GetField(pEffect->nOperField);
	
	// 当前类型必须是整数.(暂时不考虑浮点数)
	if( (pField->m_eType != Field_Enumerate) && (pField->m_eType != Field_Integer) 
		&& (pField->m_eType != Field_Boolean) && (pField->m_eType != Field_Percent) )
		return;

	// 目标域必须是实例化的。
	if(pField->m_nInstanceOffset == -1)
		return;
	
	int *pnOrig = (int*)((char *)m_pBuffer + pField->m_nInstanceOffset);
	int nLimitValue = (pField->m_nLimitField)?GetInteger(pField->m_nLimitField-1):0;
	int nTmp = (int)m_pFormulaManager->GetEffectFormulaManager()->Formula(pEffect->eOp-1,(float)*pnOrig,(float)nLimitValue,pEffect->nValue);
	*pnOrig += nTmp;

	//int nLimitValue = pField->m_nLimitField?GetInteger(pField->m_nLimitField-1):0;
	//int *pValue = (int*)((char *)m_pBuffer + pField->m_nInstanceOffset);
	//switch(pEffect->eOp)
	//{
	//case Effect_Add:
	//	*pValue += pEffect->nValue;
	//	break;
	//case Effect_PersentAdd:
	//	*pValue += (int)(*pValue * (pEffect->nValue * 0.01f) + 0.5f);
	//	break;
	//case Effect_FieldPersentAdd:
	//	*pValue += (int)(nLimitValue * (pEffect->nValue * 0.01f) + 0.5f);
	//	break;
	//case Effect_Replace:
	//	*pValue = pEffect->nValue;
	//	break;
	//case Effect_AddFlag:			//增加标记
	//	*pValue |= 1<<(pEffect->nValue-1);
	//	break;
	//case Effect_DelFlag:			//删除标记
	//	*pValue &= ~(1<<(pEffect->nValue-1));
	//	break;
	//}
	//if( (pField->m_nLimitField) && (*pValue > nLimitValue) )
	//	*pValue = nLimitValue;
}

void cInstNode_Imp::ApplyEffectToEffect(EffectValue * pEffect)
{
	// 先在已经有的实例效果中找。作用单元相同，作用域相同
	BOOL bFound = FALSE;
	for(UINT n=0;n<m_vecEffInst.size();n++)
	{
		//找到 合并
		if(	   (m_vecEffInst[n].nDestUnit == pEffect->nDestUnit) 
			&& (m_vecEffInst[n].nOperField == pEffect->nOperField)
			&& (m_vecEffInst[n].eOp == pEffect->eOp)
			&& (m_vecEffInst[n].nActParam == pEffect->nActParam) )	// 特殊用途
		{
			m_vecEffInst[n].nValue += pEffect->nValue;
			return;
		}
	}
	//未找到 创建
	if(pEffect->nRelationID)
		m_vecEffInst.push_back(*pEffect);
}

void cInstNode_Imp::CancelInstEffect(EffectValue * pEffect)
{
	Iter_VectorEffectValue iBeg = m_vecEffInst.begin();
	Iter_VectorEffectValue iEnd = m_vecEffInst.end();
	for(;iBeg!=iEnd;iBeg++)
	{
		//找到 删除
		if(	   (iBeg->nDestUnit == pEffect->nDestUnit) 
			&& (iBeg->nOperField == pEffect->nOperField)
			&& (iBeg->eOp == pEffect->eOp)
			&& (iBeg->nActParam == pEffect->nActParam) )	// 特殊用途
		{
			if(iBeg->nValue>pEffect->nValue)
				iBeg->nValue-=pEffect->nValue;
			else
				m_vecEffInst.erase(iBeg);
			return;
		}
	}
}

void cInstNode_Imp::ConvertBufferToEffect(EffectValue * pEffect,BuffEffectValue * pBuff,int nParam)
{
	// 目前只考虑影响值是常数的情况
	pEffect->nTemplateID = pBuff->nTemplateID;
	pEffect->nRelationID = pBuff->nRelationID;
	pEffect->eDest = pBuff->eDest;
	pEffect->nDestUnit = pBuff->nDestUnit;
	pEffect->nOperField = pBuff->nOperField;
	pEffect->eOp = pBuff->eOp;
	pEffect->ePhase = pBuff->ePhase;
	pEffect->nValue = (float)pBuff->nValue;
	pEffect->nActParam = nParam;
}

void cInstNode_Imp::SkillBuffUpdate(DWORD dwCurTime)
{
	cRemain * pRemain = g_NodeStreamImp.GetRemain(); 
	if(m_lstBuffs.empty())
		return;

	/////////////////////技能BUFF追加/////////////////////
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
	std::list<BuffEffect>::iterator ib = m_lstBuffs.begin();
#else
	Iter_ListBuffEffect ib = m_lstBuffs.begin();
#endif
	while(ib != m_lstBuffs.end())
	{
		BuffEffect& buff = *ib;
		if(buff.m_bEnable)
		{
			switch(buff.m_pValue->eBuffEffectType)
			{
			case BuffEffect_InTime:
				break;
			case BuffEffect_Cycle:
				{
					// 已经到了一个周期
					if(dwCurTime - buff.m_dwLastUpdateTime >= (DWORD)(buff.m_pValue->nCycleInterval))
					{
						if(buff.m_pValue->nPersistenceTime > 0)
							buff.m_dwRemainTime-=1;

						BufferTakeEffect(buff);
						buff.m_dwLastUpdateTime+=(unsigned)buff.m_pValue->nCycleInterval;
						BuffEffect *pBuff = &buff;
						if(pRemain)
						{
							pRemain->OnBuffTime(this,&pBuff,buff.m_dwEffectTime,buff.m_dwRemainTime*buff.m_pValue->nCycleInterval);
						}
					}
				}
				break;
			case BuffEffect_Persistence:
				{
					if (dwCurTime > buff.m_dwLastUpdateTime)
					{
						if (buff.m_pValue->nPersistenceTime != -1)
						{
							if((dwCurTime - buff.m_dwLastUpdateTime) > buff.m_dwRemainTime)
								buff.m_dwRemainTime = 0;
							else
								buff.m_dwRemainTime -= (dwCurTime - buff.m_dwLastUpdateTime);
						}
						buff.m_dwLastUpdateTime = dwCurTime;
					}
				}
				break;
			}
		}
		else
		{//!buff.m_bEnable
			// BUFF开始生效，这点和其他地方稍微有点区别
			if(buff.m_dwBeginTime <= dwCurTime)
			{
				buff.m_bEnable = TRUE;
				switch(buff.m_pValue->eBuffEffectType)
				{
				case BuffEffect_InTime:
					{
						buff.m_dwRemainTime = 0;
					}
					break;
				case BuffEffect_Persistence:
					{
						if (buff.m_dwRemainTime == 0)
						{
							if (buff.m_pValue->nPersistenceTime == -1)
							{
								buff.m_dwRemainTime = -1;
							}
							else
							{
								buff.m_dwRemainTime = buff.m_pValue->nPersistenceTime;
							}
						}	
					}
					break;
				case BuffEffect_Cycle:
					{
						if (buff.m_dwRemainTime == 0)
						{
							if (buff.m_pValue->nPersistenceTime == -1)
							{
								buff.m_dwRemainTime = -1;
							}
							else
							{
								buff.m_dwRemainTime = buff.m_pValue->nPersistenceTime / buff.m_pValue->nCycleInterval;
							}
						}	
					}
					break;
				}

				if(pRemain)
				{
					//BufferTakeEffectFirstTime(&buff);//采用下面的设计延时不为0的立即生效周起生效技能改变状态机会出问题
					// 如果是脚本的话要计算影响值
					if(buff.m_pValue->eEvType == EVT_Script)
					{
						//这里先固定传参
						cInstNode * pSrcInst = (cInstNode *)g_NodeStreamImp.GetInstance(buff.m_nInstID);
						if(pSrcInst && buff.m_pValue->szRefField[0])
						{
							buff.m_nValue = m_pFormulaManager->GetSkillFormulaManager()->Formula(buff.m_pValue->szRefField,
								pSrcInst,this,buff.m_pValue->nParam3,buff.m_pValue->nParam4);
						}
						else
						{
							buff.m_nValue = 0;
						}
					}
					// 常数和状态机的情况
					else
					{
						buff.m_nValue = (float)buff.m_pValue->nValue;
					}
					switch(buff.m_pValue->eBuffEffectType)
					{
					case BuffEffect_InTime:
					case BuffEffect_Cycle:
						{
							if(buff.m_pValue->eEvType != EVT_State)
							{
								BufferTakeEffect(buff);
							}
							else
							{
								if (m_pState)
								{
									int nValue = GetInteger(buff.m_pValue->nOperField);
									m_pState->ChangeState(this,nValue,buff.m_pValue->nValue-1);
								}
							}
						}
						break;
					case BuffEffect_Persistence:
						{
							if(buff.m_pValue->eEvType != EVT_State)
							{
								float fValue = GetFloat(buff.m_pValue->nOperField,FALSE);
								SetFloat(buff.m_pValue->nOperField,fValue);
							}
							// 现在世界定义中的状态机还无法工作.
							else
							{
								if (m_pState)
								{
									int nValue = GetInteger(buff.m_pValue->nOperField);
									m_pState->ChangeState(this,nValue,buff.m_pValue->nValue-1);
								}
							}
						}
						break;
					}


					buff.m_dwLastUpdateTime = dwCurTime;
					BuffEffect *pBuff = &buff;
					if (buff.m_pValue->eBuffEffectType != BuffEffect_InTime)
					{
						pRemain->OnBuffBegin(this,&pBuff);
					}else
					{
						pRemain->OnBuffEnd(this,&pBuff);	//赤煞鬼头延时减生命耗值的技能不表现
					}
				}
			}
		}

		// 残留时间过了 才可以删除
		if(buff.m_bEnable && (buff.m_pValue->nPersistenceTime >0||buff.m_pValue->eBuffEffectType==BuffEffect_InTime) && (buff.m_dwRemainTime <= 0))
		{
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
			std::list<BuffEffect>::iterator iDel = ib;
#else
			Iter_ListBuffEffect iDel = ib;
#endif
			ib ++;

			if(pRemain)
			{
				assert(m_lstBuffs.size()>0);

				// 对于持续生效的要减去生效值
				buff.m_bEnable = FALSE;
				if(buff.m_pValue->eBuffEffectType == BuffEffect_Persistence)
				{
					if (buff.m_pValue->eEvType != EVT_State)
					{
						float fValue = GetFloat(buff.m_pValue->nOperField,FALSE);
						SetFloat(buff.m_pValue->nOperField,fValue);
						assert(m_lstBuffs.size()>0);
					}
					// 现在世界定义中的状态机还无法工作.
					else
					{
						if (m_pState)
						{
							UINT nValue = GetInteger(buff.m_pValue->nOperField);
							UINT nDefault = m_pState->GetDefaultState();
							m_pState->ChangeState(this,nValue,nDefault,eSct_TimeOut);
							assert(m_lstBuffs.size()>0);
						}
					}

				}
				
				assert(m_bInOnBuffEnd == FALSE);
				m_bInOnBuffEnd = TRUE;
				BuffEffect *pBuff = &buff;
				pRemain->OnBuffEnd(this,&pBuff);
				m_bInOnBuffEnd = FALSE;

				assert(m_lstBuffs.size()>0);
				assert(m_bInOnBuffEnd == FALSE);

				if(buff.m_pBuffEvent)
					buff.m_pBuffEvent->OnBuffErased(&buff);
				m_lstBuffs.erase(iDel); 
			}
			continue;
		}
		ib ++;
	}
	/////////////////////技能BUFF追加/////////////////////

}

void cInstNode_Imp::Update(DWORD dwCurTime)						// 更新实例
{
	if(m_bUpdate)
	{
		cRemain * pRemain = g_NodeStreamImp.GetRemain();

		// 技能BUFF的更新
		SkillBuffUpdate(dwCurTime);

		if(!m_lstEffects.empty())
		{
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
			std::list<InstEffect>::iterator i1 = m_lstEffects.begin(); 
#else
			Iter_ListInstEffect i1 = m_lstEffects.begin(); 
#endif
			while(i1 != m_lstEffects.end())
			{
				InstEffect& ie =  *i1;

				if(pRemain)
				{
					if((dwCurTime - m_dwLastUpdate)>ie.dwEffectRemain)
						ie.dwEffectRemain = 0;					
					else
						ie.dwEffectRemain -= dwCurTime - m_dwLastUpdate;	
					pRemain->OnEffectTime(this,&ie,ie.dwEffectTime,ie.dwEffectRemain);   
				}

				// 残留时间过了 才可以删除
				if( ie.dwEffectRemain == 0)
				{
					cLeafRow_Imp * pRow = (cLeafRow_Imp *)ie.pRow; 
#ifndef DEFINE_BLOCK_ALLOCATED_LIST				
					std::list<InstEffect>::iterator iDel = i1;
#else
					Iter_ListInstEffect iDel = i1;
#endif
					i1 ++;

					int nUnitID = iDel->nUnitID;
					int nTemplateID = iDel->nTemplateID;

					m_lstEffects.erase(iDel); 
					OnEffectEnd(pRow); 
					if(pRemain)
						pRemain->OnEffectEnd(this,nUnitID,nTemplateID); 

					continue;
				}
				i1 ++;
			}
		}

		if(!m_lstColds.empty())
		{
	#ifndef DEFINE_BLOCK_ALLOCATED_LIST
			std::list<EffectCold>::iterator i2 = m_lstColds.begin(); 
	#else
			Iter_ListEffectCold i2 = m_lstColds.begin(); 
	#endif
			while(i2 != m_lstColds.end())
			{
				EffectCold& ec =  *i2;

				if(pRemain)
				{
					if((dwCurTime - m_dwLastUpdate)>ec.dwColdRemain)
						ec.dwColdRemain = 0;
					else
						ec.dwColdRemain -= dwCurTime - m_dwLastUpdate;
					pRemain->OnColdTime(this,&ec,ec.dwColdTime,ec.dwColdRemain);   
				}

				// 冷却时间过了 才可以删除
				if(ec.dwColdRemain == 0)
				{
	#ifndef DEFINE_BLOCK_ALLOCATED_LIST
					std::list<EffectCold>::iterator iDel = i2;
	#else
					Iter_ListEffectCold iDel = i2;
	#endif
					i2 ++;
					m_lstColds.erase(iDel); 
					continue;
				}
				i2 ++;
			}
		}
	}
	m_dwLastUpdate = dwCurTime;
}

BOOL cInstNode_Imp::ColdAllow(int nRelationID,cInstNode * pOper)const
{
	cRemain * pRemain = g_NodeStreamImp.GetRemain(); 
	if(pRemain)
	{
		// 取得冷却时间，序列号，序列级别，ID
		EffectCold _ec;
		pRemain->GetColdTime(pOper,&_ec); 

		// 在冷却时间中寻找
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
		std::list<EffectCold>::const_iterator i = m_lstColds.begin(); 
#else
		Const_Iter_ListEffectCold i = m_lstColds.begin(); 
#endif
		while(i != m_lstColds.end())
		{
			const EffectCold& ec =  *i;
			if((_ec.nSerialID == 0) && (_ec.nTemplateID == ec.nTemplateID))
				return FALSE;
			if((_ec.nSerialID != 0) && (_ec.nSerialID == ec.nSerialID) && (_ec.nSerialLevel<=ec.nSerialLevel))
				return FALSE;
			i++;
		}
	}
	return TRUE;
}

int cInstNode_Imp::HandleAllow(int nRelationID,cInstNode * pHandler)const
{
	const cUnit * pHandleUnit = pHandler->GetUnit();
	// 看是否存在指定的关系
	const Relation * rfound = NULL;
	for(UINT n=0;n<m_pUnit->GetRelations();n++)
	{
		const Relation * r = m_pUnit->GetRelation(n); 
		if( (r->nID == nRelationID) && (r->nHandlerUnitID == pHandleUnit->GetUnitDef()->m_nID) )
		{
			rfound = r;
			break;
		}
	}
	// 指定的关系不存在
	if(rfound == NULL)
		return -1;

	// 看实例类型是否符合
	if(rfound->nHandlerUnitID != pHandler->GetUnit()->GetUnitDef()->m_nID)
		return -2;

	// 看实例类型是否符合
	if(rfound->nOperUnitID != m_pUnit->GetUnitDef()->m_nID)
		return -3;

	// 判断所有的限制条件
	cLeafRow_Imp * pRow = (cLeafRow_Imp*)m_pRow;
	UINT nLimits = pRow->GetLimites();
	LimitField ** ppLimits = (LimitField **)pRow->GetLimitePtr();
	//for(UINT n=0;n<nLimits;n++)
	//{
	//	if( (ppLimits[n]->nReleationID == nRelationID) && (ppLimits[n]->eDest == Limit_Handler))
	//	{
	//		if(!LimiteAllow(pHandler,ppLimits[n]))
	//			return -4 - n;
	//	}
	//}
	
	// 取得最大的分组数量
	UINT nMax = 0;
	UINT n;
	for(n=0;n<nLimits;n++)
	{
		nMax = ppLimits[n]->nGroup>nMax?ppLimits[n]->nGroup:nMax;
	}

	// 取得组使用的最高位
	UINT nCurrBit = 0;
	while(nMax)
	{
		nMax = nMax>>1;
		nCurrBit++;
	}

	bool bFlag = true;
	// 没有分组信息的情况
	if(nCurrBit == 0)
	{
		for(UINT n=0;n<nLimits;n++)
		{
			if( (ppLimits[n]->nReleationID == nRelationID) && (ppLimits[n]->eDest == Limit_Handler))
			{
				if(!LimiteAllow(pHandler,ppLimits[n]))
					return -4 - n;
			}
		}
	}
	// 分组的情况(分组和组索引=0的情况暂时不做考虑，如果存在，请添加相关的处理)
	else
	{
		for(UINT nBit=0;nBit<nCurrBit;nBit++)
		{
			bFlag = true;
			for(UINT nCnt=0;nCnt<nLimits;nCnt++)
			{
				if( (ppLimits[n]->nGroup)&(1<<nBit) ) 
				{
					if((ppLimits[n]->nReleationID == nRelationID) && (ppLimits[n]->eDest == Limit_Handler))
					{
						if(!LimiteAllow(pHandler,ppLimits[n]))
						{
							bFlag = false;
							break;
						}
					}
				}
			}

			if(bFlag)
				return 0;
		}
	}

	if(bFlag)
		return 0;
	else
		return -4;

}

int cInstNode_Imp::LimiteAllow(const Attachment * attach)const
{
	// 只看模板的限制,目前无实例限制
	const cInstNode * pHandle = g_NodeStreamImp.GetInstance(attach->nInstHandID);
	const cInstNode * pOper = g_NodeStreamImp.GetInstance(attach->nInstOperID);
	if(pHandle && pOper)
	{

		// 判断所有的限制条件
		cLeafRow_Imp * pRow = (cLeafRow_Imp*)pOper->GetRow();
		assert(pRow);
		if(!pRow)
		{
			return 0;
		}
		UINT nLimits = pRow->GetLimites();
		if (0 == nLimits)
		{
			return 0;
		}
		const cUnit * pOperUnit = pOper->GetUnit();

		// 看是否存在指定的关系
		const Relation * rfound = NULL;
		for(UINT n=0;n<pOperUnit->GetRelations();n++)
		{
			const Relation * r = pOperUnit->GetRelation(n); 
			if( (r->nID == attach->nRelationID) && IfInheritTheUnit(r->nDestUnitID)/*(r->nDestUnitID == m_pUnit->GetUnitDef()->m_nID)*/ )
			{
				rfound = r;
				break;
			}
		}
		// 指定的关系不存在
		if(rfound == NULL)
			return -1;
		
		// 看实例类型是否符合
		if (!this->IfInheritTheUnit(rfound->nHandlerUnitID))
		{
			return -2;
		}
		if (!pOper->IfInheritTheUnit(rfound->nOperUnitID))
		{
			return -3;
		}

		LimitField ** ppLimits = (LimitField **)pRow->GetLimitePtr();
		for(UINT n=0;n<nLimits;n++)
		{
			if(ppLimits[n]->nReleationID == attach->nRelationID)
			{
				if(!LimiteAllow(pHandle,ppLimits[n]))
				{
					return -4 - n;
				}
			}
		}
	}

	return 0;
}

BOOL cInstNode_Imp::LimiteAllow(const cInstNode * pHandle,LimitField * pLimit)const
{
	// 非整数型暂时忽略
	
	int nLimitValue = pLimit->nValue;
	if (pLimit->eValueType == LimitValue_Precent)	//上限百分比
	{
		const cLeafNodeDesc *pDesc = pHandle->GetDesc();
		const LeafField *pField = pDesc->GetField(pLimit->nDestField);
		if (pField->m_nLimitField > 0)
		{
			int nLimit = pHandle->GetInteger(pField->m_nLimitField-1); 
			nLimitValue = (int)(pLimit->nValue/100.0f*nLimit);
		}
	}

	// 基本属性的判断
	if(pLimit->eDestType == LimitType_Prop)
	{
		int nDestValue = 0;
		nDestValue = pHandle->GetInteger(pLimit->nDestField);
		switch(pLimit->eOp)
		{
		case Limit_GreaterEqual:// 大于等于
			if(nDestValue >= nLimitValue)
				return TRUE;
			break;
		case Limit_Greater:		// 大于
			if(nDestValue > nLimitValue)
				return TRUE;
			break;
		case Limit_Equal:		// 等于
			if(nDestValue == nLimitValue)
				return TRUE;
			break;
		case Limit_Lessqual:	// 小于等于
			if(nDestValue <= nLimitValue)
				return TRUE;
			break;
		case Limit_Less:		// 小于
			if(nDestValue <= nLimitValue)
				return TRUE;
			break;
		case Limit_NotEqual:	// 不等于
			if(nDestValue != nLimitValue)
				return TRUE;
			break;
		case Limit_NoFlag:		// 无标记
			if( (nDestValue & ((1<<(nLimitValue-1)))) == 0 )
				return TRUE;
			break;
		case Limit_HasFlag:		// 有标记
			if( (nDestValue & ((1<<(nLimitValue-1)))) == ((1<<(nLimitValue-1))) )
				return TRUE;
			break;
		}
	}
	// 自定义属性的判断
	else if(pLimit->eDestType == LimitType_Custom)
	{
		switch(pLimit->nDestField)
		{
		case DT_UnitId:
			// 不能简单的和单元ID做比较，如果该实例是派生单元的实例，也认为属于该单元
			// 只存在等于和不等于的情况
			switch(pLimit->eOp)
			{
			case Limit_Equal:		// 等于
				return pHandle->IfInheritTheUnit((UINT)nLimitValue);
				break;
			case Limit_GreaterEqual:// 大于等于
			case Limit_Greater:		// 大于
			case Limit_Lessqual:	// 小于等于
			case Limit_Less:		// 小于
			case Limit_NoFlag:		// 无标记
			case Limit_HasFlag:		// 有标记
			case Limit_NotEqual:	// 不等于
				return !(pHandle->IfInheritTheUnit((UINT)nLimitValue));
				break;
			}
			
			break;
		case DT_SocietyRelate:
			switch(pLimit->eOp)
			{
			case Limit_Equal:		// 等于
				{
					return GetRelate2Other(pHandle,nLimitValue);
				}
				break;
			case Limit_GreaterEqual:// 大于等于
			case Limit_Greater:		// 大于
			case Limit_Lessqual:	// 小于等于
			case Limit_Less:		// 小于
			case Limit_NoFlag:		// 无标记
			case Limit_HasFlag:		// 有标记
			case Limit_NotEqual:	// 不等于
				{
					return !(GetRelate2Other(pHandle,nLimitValue));
				}
				break;
			}
			break;
		}
	}
	return FALSE;
}


BOOL cInstNode_Imp::IfInheritTheUnit(UINT nUnitID) const
{
	// todo:添加相关代码，切记！20080612
	const cUnit * pUnit = GetUnit();
	while(pUnit)
	{
		if(pUnit->GetUnitDef()->m_nID == nUnitID || pUnit->GetUnitDef()->m_nBaseClass == nUnitID)
			return TRUE;

		if(pUnit->GetUnitDef()->m_nBaseClass)
			pUnit = g_NodeStreamImp.GetUnit(pUnit->GetUnitDef()->m_nBaseClass);
		else
			pUnit = NULL;
	}
	return FALSE;
}

BOOL cInstNode_Imp::IfAInheritB(int nAUnitID,int nBUnitID)
{
	const cUnit * pUnit = g_NodeStreamImp.GetUnit(nAUnitID);
	while(pUnit)
	{
		if(pUnit->GetUnitDef()->m_nID == nBUnitID || pUnit->GetUnitDef()->m_nBaseClass == nBUnitID)
			return TRUE;

		if(pUnit->GetUnitDef()->m_nBaseClass)
			pUnit = g_NodeStreamImp.GetUnit(pUnit->GetUnitDef()->m_nBaseClass);
		else
			pUnit = NULL;
	}
	return FALSE;
}

BOOL cInstNode_Imp::GetRelate2Other(const cInstNode *pOther,UINT nRelate) const
{
	// todo:添加相关代码，切记！20080612
	pFncGetInstsRelate pRelateListen = g_NodeStreamImp.GetInstsRelateListener();
	if(pRelateListen)
	{
		return pRelateListen(this,pOther,nRelate);
	}
	return FALSE;
}

void cInstNode_Imp::GetEffectValue(EffectValue * pEffect,int * pnValue) const
{
	// 本项目暂时不考虑影响的阶段性.都认为是作用在裸值上的.
	switch(pEffect->eOp-1)
	{
	case eValueAdd:		// 数值加
	case eValueSub:		// 数值减
		*pnValue += (int)pEffect->nValue;
		break;
	case ePercentAdd:	// 百分比加
	case ePercentSub:	// 百分比减
		*pnValue += (int)pEffect->nValue;
		*pnValue = max(0,*pnValue);
		break;
	case eSetFlag:		// 设置标记
		*pnValue |= 1<<((int)pEffect->nValue-1);
	case eChangeState:	// 改变状态机
	case eCover:		// 覆盖
		*pnValue = (int)pEffect->nValue;
		break;
	case eZero:			// 清零
		*pnValue = TRUE;// 真就表示清零
		break;
	}
	return;
}

void cInstNode_Imp::GetBufferValue(BuffEffect * pEffect,int * pnValue) const
{
	switch(pEffect->m_pValue->eOp-1)
	{
	case eValueAdd:		// 数值加
	case eValueSub:		// 数值减
		*pnValue += (int)pEffect->m_nValue;
		break;
	case ePercentAdd:	// 百分比加
	case ePercentSub:	// 百分比减
		*pnValue += (int)pEffect->m_nValue;
		*pnValue = max(0,*pnValue);
		break;
	case eSetFlag:		// 设置标记
		*pnValue |= 1<<((int)pEffect->m_nValue-1);
		break;
	case eChangeState:	// 改变状态机
		*pnValue = (int)pEffect->m_nValue-1;
		break;
	case eCover:		// 覆盖
		*pnValue = (int)pEffect->m_nValue;
		break;
	case eZero:			// 清零
		*pnValue = TRUE;// 真就表示清零
		break;
	}
	return;
}

void cInstNode_Imp::GetEffectValue(int nUnitID,int nField,int * pnValue)const
{
	UINT nRealUnitID = 0;
	if(m_lstEffects.size() > 0)//这个大概没有
	{
		// 临时效果
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
		std::list<InstEffect>::const_iterator ie = m_lstEffects.begin(); 
#else
		Const_Iter_ListInstEffect ie = m_lstEffects.begin(); 
#endif
		while(ie != m_lstEffects.end())
		{
			// 临时效果 无 递归计算子实例

			// 计算子实例本身
			cLeafRow_Imp * pRow = (cLeafRow_Imp*)ie->pRow;
			UINT nEffects = pRow->GetEffects();
			EffectValue ** ppEffects = (EffectValue **)pRow->GetEffectPtr(); 
			for(UINT n=0;n<nEffects;n++)
			{
				nRealUnitID = GetUnitIDByRelateAndDestUnit(ppEffects[n]->nRelationID,ppEffects[n]->nDestUnit);
				if( cInstNode_Imp::IfAInheritB(nUnitID,nRealUnitID)/*(nRealUnitID == nUnitID)*/&&(ppEffects[n]->nOperField == nField))
					GetEffectValue(ppEffects[n],pnValue+ppEffects[n]->eOp-1);
			}

			ie ++;
		}
	}

	if(m_lstAttachment.size() > 0)
	{
		// 关系效果
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
		std::list<Attachment>::const_iterator ia = m_lstAttachment.begin(); 
#else
		Const_Iter_ListAttach ia = m_lstAttachment.begin(); 
#endif
		while(ia != m_lstAttachment.end())
		{
			const cInstNode * pOper = g_NodeStreamImp.GetInstance(ia->nInstOperID);
			assert(pOper);
			if(pOper)
			{
				// 递归计算子实例
				((cInstNode_Imp*)pOper)->GetEffectValue(nUnitID,nField,pnValue);

				// 计算子实例本身		
				cLeafRow_Imp * pRow = (cLeafRow_Imp*)pOper->GetRow();
				UINT nEffects = pRow->GetEffects();
				EffectValue ** ppEffects = (EffectValue **)pRow->GetEffectPtr(); 
				for(UINT n=0;n<nEffects;n++)
				{
					nRealUnitID = GetUnitIDByRelateAndDestUnit(ppEffects[n]->nRelationID,ppEffects[n]->nDestUnit);
					if( cInstNode_Imp::IfAInheritB(nUnitID,nRealUnitID)/*(nRealUnitID == nUnitID)*/&&(ppEffects[n]->nOperField == nField) && ppEffects[n]->nRelationID == ia->nRelationID)
						GetEffectValue(ppEffects[n],pnValue+ppEffects[n]->eOp-1);
				}
			}
			ia ++;
		}
	}

	// 实例效果
	for(UINT i=0;i<m_vecEffInst.size();i++)
	{
		nRealUnitID = GetUnitIDByRelateAndDestUnit(m_vecEffInst[i].nRelationID,m_vecEffInst[i].nDestUnit);
		if( cInstNode_Imp::IfAInheritB(nUnitID,nRealUnitID)/*(nRealUnitID == nUnitID)*/&&(m_vecEffInst[i].nOperField == nField))
			GetEffectValue((EffectValue *)&m_vecEffInst[i],pnValue+m_vecEffInst[i].eOp-1);
	}

	if(m_lstBuffs.size() > 0)
	{
		// buffer效果
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
		std::list<BuffEffect>::const_iterator ib = m_lstBuffs.begin(); 
#else
		Const_Iter_ListBuffEffect ib = m_lstBuffs.begin(); 
#endif
		while(ib != m_lstBuffs.end())
		{
			const BuffEffect * pBuff = &(*ib);
			// Lighter : 周期性的Buff不应该被计算在内.因为周期性的Buff影响已经在周期作用时起过作用了.这里只计算持续型的Buff
			if (pBuff->m_bEnable && pBuff->m_pValue->eBuffEffectType == BuffEffect_Persistence && pBuff->m_pValue->nOperField == nField)
			{
				GetBufferValue((BuffEffect *)pBuff,pnValue+pBuff->m_pValue->eOp-1);
			}
			ib ++;
		}
	}
}


void cInstNode_Imp::BuildEffectValue(int nUnitID,NodeAdder * pNodeAdder)
{
	for(UINT i=0;i<m_pAttrib->GetRows();i++)
	{
		if(pNodeAdder[i][ePercentAdd] == 0)
			pNodeAdder[i][ePercentAdd] = PERCENT_RAD;
		if(pNodeAdder[i][ePercentSub]==0)
			pNodeAdder[i][ePercentSub] = PERCENT_RAD;
	}
	
	UINT nRealUnitID = 0;
	if(m_lstEffects.size() > 0)
	{
		// 临时效果
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
		std::list<InstEffect>::const_iterator ie = m_lstEffects.begin(); 
#else
		Const_Iter_ListInstEffect ie = m_lstEffects.begin(); 
#endif
		while(ie != m_lstEffects.end())
		{
			// 临时效果 无 递归计算子实例

			// 计算子实例本身
			cLeafRow_Imp * pRow = (cLeafRow_Imp*)ie->pRow;
			UINT nEffects = pRow->GetEffects();
			EffectValue ** ppEffects = (EffectValue **)pRow->GetEffectPtr(); 
			for(UINT n=0;n<nEffects;n++)
			{
				nRealUnitID = GetUnitIDByRelateAndDestUnit(ppEffects[n]->nRelationID,ppEffects[n]->nDestUnit);
				if( cInstNode_Imp::IfAInheritB(nUnitID,nRealUnitID) )
					GetEffectValue(ppEffects[n],&pNodeAdder[ppEffects[n]->nOperField][ppEffects[n]->eOp-1]);
			}

			ie ++;
		}
	}

	if(m_lstAttachment.size() > 0)
	{
		// 关系效果
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
		std::list<Attachment>::const_iterator ia = m_lstAttachment.begin(); 
#else
		Const_Iter_ListAttach ia = m_lstAttachment.begin(); 
#endif
		while(ia != m_lstAttachment.end())
		{
			const cInstNode * pOper = g_NodeStreamImp.GetInstance(ia->nInstOperID);
			assert(pOper);
			if(pOper)
			{
				// 递归计算子实例
				((cInstNode_Imp*)pOper)->BuildEffectValue(nUnitID,pNodeAdder);


				// 计算子实例本身		
				cLeafRow_Imp * pRow = (cLeafRow_Imp*)pOper->GetRow();
				UINT nEffects = pRow->GetEffects();
				EffectValue ** ppEffects = (EffectValue **)pRow->GetEffectPtr(); 
				for(UINT n=0;n<nEffects;n++)
				{
					nRealUnitID = GetUnitIDByRelateAndDestUnit(ppEffects[n]->nRelationID,ppEffects[n]->nDestUnit);
					if( cInstNode_Imp::IfAInheritB(nUnitID,nRealUnitID) && ppEffects[n]->nRelationID == ia->nRelationID)
						GetEffectValue(ppEffects[n],&pNodeAdder[ppEffects[n]->nOperField][ppEffects[n]->eOp-1]);
				}
			}

			ia ++;
		}
	}

	// 实例效果
	for(UINT i=0;i<m_vecEffInst.size();i++)
	{
		nRealUnitID = GetUnitIDByRelateAndDestUnit(m_vecEffInst[i].nRelationID,m_vecEffInst[i].nDestUnit);
		if( cInstNode_Imp::IfAInheritB(nUnitID,nRealUnitID) )
			GetEffectValue((EffectValue *)&m_vecEffInst[i],&pNodeAdder[m_vecEffInst[i].nOperField][m_vecEffInst[i].eOp-1]);
	}

	if(m_lstBuffs.size() > 0)
	{
		// buffer效果
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
		std::list<BuffEffect>::const_iterator ib = m_lstBuffs.begin(); 
#else
		Const_Iter_ListBuffEffect ib = m_lstBuffs.begin(); 
#endif
		while(ib != m_lstBuffs.end())
		{
			const BuffEffect * pBuff = &(*ib);
			// Lighter : 周期性的持续不应该被计算在内.因为周期性的Buff影响已经在周期作用时起过作用了.这里只计算持续型的Buff
			if (pBuff->m_bEnable && pBuff->m_pValue->eBuffEffectType == BuffEffect_Persistence)
				GetBufferValue((BuffEffect *)pBuff,&pNodeAdder[pBuff->m_pValue->nOperField][pBuff->m_pValue->eOp-1]);
			ib ++;
		}
	}
}

const LimitField * cInstNode_Imp::GetLimiteFeild(int nIndex) const
{
	if(m_pRow)
	{
		UINT nLimits = m_pRow->GetLimites();
		LimitField ** pp = (LimitField **)m_pRow->GetLimitePtr(); 
		nIndex += 4;
		nIndex = -nIndex; 
		if((nIndex >= 0) && (nIndex < (int)nLimits))
			return pp[nIndex];
	}
	return NULL;
}

LimitField * cInstNode_Imp::GetLimiteValue(int nRelationID,int nFields) const
{
	UINT nRet = 0;
	UINT nLimits = m_pRow->GetLimites();
	LimitField ** pp = (LimitField **)m_pRow->GetLimitePtr(); 
	for(UINT n=0;n<nLimits;n++)
	{
		//if( (pp[n]->nReleationID == nRelationID) && (pp[n]->eDest == Dest_Attrib) && pp[n]->nDestField == nFields)
		if( (pp[n]->nReleationID == nRelationID) && (pp[n]->eDest==Limit_Handler) && pp[n]->nDestField == nFields)
			return pp[n];
	}
	return NULL;
}

UINT cInstNode_Imp::GetLimiteDestFeilds(int nRelationID,Limit_Dest eDest,LimitField ** ppLimits) const
{
	UINT nRet = 0;
	UINT nLimits = m_pRow->GetLimites();
	LimitField ** pp = (LimitField **)m_pRow->GetLimitePtr(); 
	for(UINT n=0;n<nLimits;n++)
	{
		if( (pp[n]->nReleationID == nRelationID) && (pp[n]->eDest == eDest) )
		{
			if(ppLimits)
				ppLimits[nRet]=pp[n];
			nRet ++;
		}
	}
	return nRet;
}

UINT cInstNode_Imp::GetEffectDestFeilds(int nRelationID,Effect_Dest eDest,EffectValue ** ppEffets) const
{
	UINT nRet = 0;
	UINT nEffects = m_pRow->GetEffects();
	EffectValue ** pp = (EffectValue **)m_pRow->GetEffectPtr(); 
	for(UINT n=0;n<nEffects;n++)
	{
		if( (pp[n]->nRelationID == nRelationID) && (pp[n]->eDest == eDest) )
		{
			if(ppEffets)
				ppEffets[nRet]=pp[n];
			nRet ++;
		}
	}
	return nRet;
}

UINT cInstNode_Imp::GetInstEffectFeilds(EffectValue ** ppEffets) const
{
	// 实例加成
	for(UINT i=0;i<m_vecEffInst.size();i++)
	{
		if(ppEffets)
		{
			const EffectValue &eff = m_vecEffInst[i];
			ppEffets[i]= (EffectValue*)&eff;
		}
	}
	return m_vecEffInst.size();
}

UINT cInstNode_Imp::GetEffectDestFeilds(int nRelationID,EffectValue ** ppEffets,UINT * pnCount) const
{
	UINT nRet = 0;
	if(!pnCount)
		pnCount = &nRet;
	
	// 模板加成
	UINT nEffects = m_pRow->GetEffects();
	EffectValue ** pp = (EffectValue **)m_pRow->GetEffectPtr(); 
	for(UINT n=0;n<nEffects;n++)
	{
		if(pp[n]->nRelationID == nRelationID)
		{
			if(ppEffets)
				ppEffets[*pnCount]=pp[n];
			(*pnCount) ++;
		}
	}

	// 实例加成
	for(UINT i=0;i<m_vecEffInst.size();i++)
	{
		if(m_vecEffInst[i].nRelationID  == nRelationID)
		{
			if(ppEffets)
			{
				const EffectValue &eff = m_vecEffInst[i];
				ppEffets[*pnCount]= (EffectValue*)&eff;
			}
			(*pnCount) ++;
		}
	}
	
	// 在附加的实例中寻找
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
	std::list<Attachment>::const_iterator ia = m_lstAttachment.begin(); 
#else
	Const_Iter_ListAttach ia = m_lstAttachment.begin(); 
#endif
	while(ia != m_lstAttachment.end())
	{
		const cInstNode * pOper = g_NodeStreamImp.GetInstance(ia->nInstOperID);
		assert(pOper);
		if(pOper)
		{
			// 递归计算子实例
			((cInstNode_Imp*)pOper)->GetEffectDestFeilds(nRelationID,ppEffets,pnCount);
		}
	}

	return *pnCount;
}

UINT cInstNode_Imp::GetEffectBuffFeilds(EDestUnit nDestUnit,BuffEffectValue ** ppEffets,UINT * pnCount) const
{
	UINT nRet = 0;
	UINT nBuffs = m_pRow->GetBuffs();
	BuffEffectValue ** pp = (BuffEffectValue **)m_pRow->GetBuffPtr(); 
	UINT nMaxCount = pnCount?*pnCount:0;
	for(UINT n=0;n<nBuffs;n++)
	{
		if(pp[n]->nDestUnit == nDestUnit)
		{
			if(ppEffets)
				ppEffets[nRet]=pp[n];
			nRet ++;
			if(nMaxCount>0 &&(nRet >= (nMaxCount-1)))
				break;
		}
	}
	if(pnCount)
		*pnCount = nRet;
	return nRet;
}

UINT cInstNode_Imp::GetBufferEffects(BuffEffect *pBuffers,UINT *pnCount) const
{
	UINT nCnt = 0;
#ifndef DEFINE_BLOCK_ALLOCATED_LIST
	std::list<BuffEffect>::const_iterator iBeg = m_lstBuffs.begin();
	std::list<BuffEffect>::const_iterator iEnd = m_lstBuffs.end();
#else
	Const_Iter_ListBuffEffect iBeg = m_lstBuffs.begin();
	Const_Iter_ListBuffEffect iEnd = m_lstBuffs.end();
#endif
	for (; iBeg!=iEnd; iBeg++)
	{
		*(pBuffers+nCnt) = *iBeg;
		++nCnt;
		if(nCnt==MAX_BUFFER_EFFECTS)
		{
			assert(0);//这个实例的buff怎么会这么多?
			break;
		}
	}
	if (pnCount)
	{
		*pnCount = nCnt;
	}
	return nCnt;
}

void cInstNode_Imp::OnEffectBegin(cLeafRow_Imp * pRow)
{
	UINT nEffects = pRow->GetEffects();
	EffectValue ** ppEffects = (EffectValue **)pRow->GetEffectPtr(); 
	UINT nUnitID = m_pUnit->GetUnitDef()->m_nID;  
	
	for(UINT n=0;n<nEffects;n++)
	{
		if( (ppEffects[n]->nDestUnit == nUnitID) && (ppEffects[n]->eDest == Dest_Attrib) )
		{
			const LeafField * f = m_pNodeDesc->GetField(ppEffects[n]->nOperField);
			if(f->m_nLimitField != 0)
				ApplyEffectToAttrib(ppEffects[n]);
		}
	}
}
void cInstNode_Imp::OnEffectEnd(cLeafRow_Imp * pRow)
{
	UINT nEffects = pRow->GetEffects();
	EffectValue ** ppEffects = (EffectValue **)pRow->GetEffectPtr(); 
	UINT nUnitID = m_pUnit->GetUnitDef()->m_nID;  
	
	for(UINT n=0;n<nEffects;n++)
	{
		if( (ppEffects[n]->nDestUnit == nUnitID) && (ppEffects[n]->eDest == Dest_Attrib) )
		{
			const LeafField * f = m_pNodeDesc->GetField(ppEffects[n]->nOperField);
			if(f->m_nLimitField != 0)
			{
				int nExpendValue = GetInteger(ppEffects[n]->nOperField);
				int nLimitValue = GetInteger(f->m_nLimitField-1);
				if(nExpendValue > nLimitValue)
					SetInteger(ppEffects[n]->nOperField,nLimitValue);
			}
			if(f->m_nLimitDest != 0)
			{
				int nExpendValue = GetInteger(f->m_nLimitDest-1);
				int nLimitValue = GetInteger(ppEffects[n]->nOperField);
				if(nExpendValue > nLimitValue)
					SetInteger(f->m_nLimitDest-1,nLimitValue);
			}
		}
	}
}

UINT cInstNode_Imp::GetUnitIDByRelateAndDestUnit(int nReleation,int nDestUnit)const
{	
	cLeafNode * pRelatNode = g_NodeStreamImp.GetRelation();
	UINT nR = pRelatNode->FindRow((UINT)0,nReleation);
	const cLeafRow *pRelatRow = pRelatNode->GetRow(nR);
	assert(pRelatRow);
	if(!pRelatRow) return 0;
	int nDest = 0;
	if(nDestUnit == 0)
	{
		nDest = pRelatRow->GetInteger("使用者");
	}
	else
	{
		nDest = pRelatRow->GetInteger("作用目标");
	}
	const cUnit *pUnit = g_NodeStreamImp.GetUnit(nDest);
	if (pUnit)
	{
		return pUnit->GetUnitDef()->m_nID;
	}
	return 0;
}

void cInstNode_Imp::ClearDirtyField(UINT nField)
{
	if (nField)
	{
		m_vecFieldDirty[nField] = false;
	}
	else
	{
		int nSize = m_vecFieldDirty.size();
		for (int i=0;i<nSize;i++)
		{
			m_vecFieldDirty[i] = false;
		}
	}
}

int cInstNode_Imp::CalculateInteger(UINT col,int * pnValue,bool blimitfunction,BOOL bCaculateSub)
{
	int nRet = 0;
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if (!pField) 
		return nRet;

	if(pField && (pField->m_nInstanceOffset>=0) )
	{
		switch(pField->m_eType)
		{
		case Field_String:
			nRet = atoi((char *)m_pBuffer + pField->m_nInstanceOffset);
			break;
		case Field_Enumerate:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Boolean:
		case Field_Percent:
		case Field_IdField:
			nRet =  *(int*)((char *)m_pBuffer + pField->m_nInstanceOffset);
			break;
		case Field_Float:
			nRet = (int)(*(float*)((char *)m_pBuffer + pField->m_nInstanceOffset));
		}
	}
	else if(m_pRow && pField && (pField->m_nTemplateOffset>=0) )
		nRet = m_pRow->GetInteger(col); 

	//// 本字段不依赖于任何其他字段
	//if (!pField->m_bIsDeriveField)
	//{
	//	SetCurrValueInteger(col,nRet);
	//	return nRet;
	//}

	if(!m_vecFieldDirty[col] && m_pFormulaManager)
	{
		int pnAdder[eDataOpMax] = {0};

		if(pnValue)
		{
			CopyMemory(pnAdder,pnValue,sizeof(int)*eDataOpMax);
		}
		else
		{
			// 初始化百分比
			pnAdder[ePercentAdd] = PERCENT_RAD;
			pnAdder[ePercentSub] = PERCENT_RAD;
			GetEffectValue(m_pUnit->GetUnitDef()->m_nID,col,pnAdder);
		}

		// 调用计算公式进行计算
		if(m_pAttrib->IfHasConverseDepend(col))
		{
			const TCHAR *pFieldFormula = m_pFormulaManager->GetFieldFormulaManager()->GetFieldFormula(GetUnitID(),col);
			if(pFieldFormula)
			{
				// 是否需要计算关联字段
				if (bCaculateSub)
				{
					// 计算关联的字段
					std::set<UINT> &setConverse = m_pAttrib->GetConverseDependFields(col);
					std::set<UINT>::iterator iBeg = setConverse.begin();
					while (iBeg != setConverse.end())
					{
						int nDependCol = *iBeg;
						CalculateFloat(nDependCol,pnValue,blimitfunction,bCaculateSub);
						iBeg ++;
					}
				}
				//GetSysEfficiency()->beginRecord("FieldFormula");	
				nRet = (int)m_pFormulaManager->GetFieldFormulaManager()->Formula(pFieldFormula,this,pnAdder);
				//GetSysEfficiency()->endRecord("FieldFormula");	
			}
		}
		// (基元)没有专用公式，只需要简单计算即可
		else
		{
			nRet = SimpleComputeI(nRet,pnAdder);
		}
		//限制自己的字段
		int nLimitValue = (pField->m_nLimitField)?GetInteger(pField->m_nLimitField-1):0;
		if( (pField->m_nLimitField) && (nRet > nLimitValue) )
		{
			nRet = nLimitValue;
		}
		//自己限制的字段
		if(blimitfunction)
		{
			nLimitValue = (pField->m_nLimitDest)?GetInteger(pField->m_nLimitDest-1):0;
			if( (pField->m_nLimitDest) && (nRet < nLimitValue) )
			{
				// 设定当前值
				SetCurrValueInteger((pField->m_nLimitDest-1),nRet);
			}
		}

		m_vecFieldDirty[col] = true;
	}
	else
	{
		if(pField->m_nLimitField)
		{
			int nLimitValue = GetInteger(pField->m_nLimitField-1);			
			if(nRet > nLimitValue)
				nRet = nLimitValue;
		}
	}
	// 上下限设定
	if (pField->m_nMax != 0 && nRet > pField->m_nMax)
	{
		nRet = pField->m_nMax;
	}
	if ((pField->m_nMin == 0 && pField->m_nMax != 0) && nRet < pField->m_nMin)
	{
		nRet = pField->m_nMin;
	}
	// 设定当前值
	SetCurrValueInteger(col,nRet);

	return nRet;
}

float cInstNode_Imp::CalculateFloat(UINT col,int * pnValue,bool blimitfunction,BOOL bCaculateSub)
{
	float fValue = 0.0f;
	const LeafField * pField = m_pNodeDesc->GetField(col);
	assert(pField);
	if (!pField) return fValue;

	if(pField && (pField->m_nInstanceOffset>=0) )
	{
		switch(pField->m_eType)
		{
		case Field_String:
			break;
		case Field_Enumerate:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Boolean:
		case Field_Percent:
		case Field_IdField:
			fValue = (float)(*(int*)((char *)m_pBuffer + pField->m_nInstanceOffset));
			break;
		case Field_Float:
			fValue = (*(float*)((char *)m_pBuffer + pField->m_nInstanceOffset));
		}
	}
	else if(pField && (pField->m_nTemplateOffset>=0) )
		fValue = m_pRow->GetFloat(col); 


	//BOOL bExpendField = (pField && (pField->m_nInstanceOffset>=0) && (pField->m_nLimitField != 0));	
	if(!m_vecFieldDirty[col] && m_pFormulaManager)
	{
		int pnAdder[eDataOpMax] = {0};
		if(pnValue)
		{
			CopyMemory(pnAdder,pnValue,sizeof(int)*eDataOpMax);
		}
		else
		{
			pnAdder[ePercentAdd] = PERCENT_RAD;
			pnAdder[ePercentSub] = PERCENT_RAD;
			GetEffectValue(m_pUnit->GetUnitDef()->m_nID,col,pnAdder);
		}

		// 调用计算公式进行计算
		if(m_pAttrib->IfHasConverseDepend(col))
		{
			const TCHAR *pFieldFormula = m_pFormulaManager->GetFieldFormulaManager()->GetFieldFormula(GetUnitID(),col);
			//assert(pFieldFormula);
			if(pFieldFormula)
			{
				// 是否需要计算关联字段
				if (bCaculateSub)
				{
					// 计算关联的字段
					std::set<UINT> &setConverse = m_pAttrib->GetConverseDependFields(col);
					std::set<UINT>::iterator iBeg = setConverse.begin();
					while (iBeg != setConverse.end())
					{
						int nDependCol = *iBeg;
						CalculateFloat(nDependCol,pnValue,blimitfunction,bCaculateSub);
						iBeg ++;
					}
				}
				//GetSysEfficiency()->beginRecord("FieldFormula");
				fValue = m_pFormulaManager->GetFieldFormulaManager()->Formula(pFieldFormula,this,pnAdder);
				//GetSysEfficiency()->endRecord("FieldFormula");
			}
		}
		// (基元)没有专用公式，只需要简单计算即可
		else
		{
			fValue = SimpleComputeF(fValue,pnAdder);
		}

		//限制自己的字段
		float fLimitValue = (pField->m_nLimitField)?GetFloat(pField->m_nLimitField-1):0;
		if( (pField->m_nLimitField) && (fValue > fLimitValue) )
		{
			fValue = fLimitValue;
		}
		//自己限制的字段
		if(blimitfunction)
		{
			fLimitValue = (pField->m_nLimitDest)?GetFloat(pField->m_nLimitDest-1):0;
			if( (pField->m_nLimitDest) && (fValue < fLimitValue) )
			{
				// 设定当前值
				SetCurrValueFloat((pField->m_nLimitDest-1),fValue);
			}
		}
		// 上下限设定
		if (pField->m_nMax != 0 && fValue > (float)pField->m_nMax)
		{
			fValue = (float)pField->m_nMax;
		}
		if ((pField->m_nMin == 0 && pField->m_nMax != 0) && fValue < (float)pField->m_nMin)
		{
			fValue = (float)pField->m_nMin;
		}
		// 设定当前值
		SetCurrValueFloat(col,fValue);
		m_vecFieldDirty[col] = true;
	}
	else
	{
		SetCurrValueFloat(col,fValue);
	}	

	return fValue;
}
void cInstNode_Imp::CalculateAll(bool blimitfunction)
{
	ClearDirtyField();
	UINT nCol = 0;
	UINT * lstOrder = m_pAttrib->GetOrderFields();
	UINT nFields = m_pAttrib->GetRows();

	//GetSysEfficiency()->beginRecord("BuildEffectValue");	
	NodeAdder nodeAdder[128] = {0};
	assert(nFields<=sizeof(nodeAdder)/sizeof(NodeAdder));
	BuildEffectValue(m_pUnit->GetUnitDef()->m_nID,nodeAdder);
	//GetSysEfficiency()->endRecord("BuildEffectValue");	

	//GetSysEfficiency()->beginRecord("CalculateFields");	
	for (UINT i = 0;i<nFields;i++)
	{
		nCol = lstOrder[i];
		const LeafField * pField = m_pNodeDesc->GetField(nCol);
		if(pField && pField->m_nInstanceOffset >= 0)
		{
			switch(pField->m_eType)
			{
			case Field_Enumerate:
			case Field_Integer:
			case Field_Dynamic:
			case Field_Boolean:
			case Field_Percent:
			case Field_IdField:
				CalculateInteger(nCol,nodeAdder[nCol],blimitfunction,FALSE);
				break;
			case Field_Float:
				CalculateFloat(nCol,nodeAdder[nCol],blimitfunction,FALSE);
				break;
			}		
		}
	}
	//GetSysEfficiency()->endRecord("CalculateFields");	

	ClearDirtyField();
}

void cInstNode_Imp::SetCurrValueInteger(UINT col,int nValue)
{
	assert(!(13 == m_pUnit->GetUnitDef()->m_nID && col == 12 && nValue<=1));//可能出现的最大生命值为0的ASSERT，zhaoyangma-20100826
	const LeafField * pField = m_pNodeDesc->GetField(col);
	// 设定当前值
	if (pField->m_nInstanceOffset >= 0)
	{
		switch(pField->m_eType)
		{
		case Field_Enumerate:
		case Field_Boolean:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Percent:
		case Field_IdField:
			*(int*)((char *)m_pCurrValueBuffer + pField->m_nInstanceOffset) = nValue;
			break;
		case Field_Float:
			*(float*)((char *)m_pCurrValueBuffer + pField->m_nInstanceOffset) = (float)nValue;
			break;
		}
		m_vecFieldDirty[col] = true;
	}

}
void cInstNode_Imp::SetCurrValueFloat(UINT col,float nValue)
{
	assert(!(13 == m_pUnit->GetUnitDef()->m_nID && col == 12 && nValue<=1));//可能出现的最大生命值为0的ASSERT，zhaoyangma-20100826
	const LeafField * pField = m_pNodeDesc->GetField(col);
	// 设定当前值
	if (pField->m_nInstanceOffset >= 0)
	{
		switch(pField->m_eType)
		{
		case Field_Enumerate:
		case Field_Boolean:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Percent:
		case Field_IdField:
			*(int*)((char *)m_pCurrValueBuffer + pField->m_nInstanceOffset) = (int)nValue/*(int)(nValue+.5f)*/;
			break;
		case Field_Float:
			*(float*)((char *)m_pCurrValueBuffer + pField->m_nInstanceOffset) = (float)nValue;
			break;
		}
		m_vecFieldDirty[col] = true;
	}	
}

int cInstNode_Imp::SimpleComputeI(int nOrigValue,int *pFormula) const
{
	if (pFormula[eZero])
	{
		return 0;
	}
	else if (pFormula[eCover])
	{
		return pFormula[eCover];
	}
	else if (pFormula[eChangeState])
	{
		return pFormula[eChangeState];
	}
	else if (pFormula[eSetFlag])
	{
		return (nOrigValue | pFormula[eSetFlag]);
	}
	if (pFormula[ePercentAdd] != pFormula[ePercentSub])
	{
		return (int)((nOrigValue+pFormula[eValueAdd]-pFormula[eValueSub])*((float)pFormula[ePercentAdd]/PERCENT_RAD));
	}

	return (nOrigValue+pFormula[eValueAdd]-pFormula[eValueSub]);
}

float cInstNode_Imp::SimpleComputeF(float fOrigValue,int *pFormula) const
{
	if (pFormula[eZero])
	{
		return 0;
	}
	else if (pFormula[eCover])
	{
		return (float)pFormula[eCover];
	}
	else if (pFormula[eChangeState])
	{
		return (float)pFormula[eChangeState];
	}
	else if (pFormula[eSetFlag])
	{
		return (float)((int)fOrigValue | pFormula[eSetFlag]);
	}
	if (pFormula[ePercentAdd] != pFormula[ePercentSub])
	{
		return (fOrigValue+pFormula[eValueAdd]-pFormula[eValueSub])*((float)pFormula[ePercentAdd]/PERCENT_RAD);
	}

	return (fOrigValue+pFormula[eValueAdd]-pFormula[eValueSub]);
}

void cInstNode_Imp::BufferTakeEffect(BuffEffect &buff)
{
	// 是谁在 调效果公式的时候max传了个0进去,这样会出事吧?
	const LeafField * f = m_pNodeDesc->GetField(buff.m_pValue->nOperField);
	float fValue = GetFloat(buff.m_pValue->nOperField);
	float fLimitValue = (f->m_nLimitField)?GetFloat(f->m_nLimitField-1):0.f;
	float fAdd = m_pFormulaManager->GetEffectFormulaManager()->Formula(buff.m_pValue->eOp-1,fValue,fLimitValue,buff.m_nValue);
	float fLast = fValue+fAdd*buff.m_nFactor;

	buff.m_nLastValue = fValue;
	
	if(fLimitValue&&fLast>fLimitValue)
		fLast=fLimitValue;

	fLast = max((float)f->m_nMin,fLast);
	if(f->m_nMax > 0)
		fLast = min((float)f->m_nMax,fLast);

	// 设置当前字段值~
	SetFloat(buff.m_pValue->nOperField,fLast);


	if(f->m_eType == Field_Float)
	{
		assert(GetFloat(buff.m_pValue->nOperField,FALSE) == fLast);
	}
	else
	{
		assert(GetInteger(buff.m_pValue->nOperField,FALSE) == (int)fLast);
	}
}
