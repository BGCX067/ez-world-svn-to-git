#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <strstream>
#include <assert.h>
#include <vector>

#include "LogicEngine.h"
#include "TreeNode.h"
#include "LeafNode.h"
#include "theworld.h"

#define OVERLOAD
#include "overload.h"

// #define _CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>
// #ifdef _DEBUG
// #define   new   new(_NORMAL_BLOCK,   __FILE__,   __LINE__)
// #endif
#define ALLOC_GROW	16

extern cWorldImp g_NodeStreamImp;


////////////////////////////////////////////////////////////////////////////
//	节点描述指针				节点名				节点类型

NodeTable g_NodeTable[];	

cTreeNodeDesc_imp::cTreeNodeDesc_imp(Node_Type type,TCHAR * lpszDescName,
				  BOOL bFiled,BOOL bExclusive,cNodeDesc ** ppNodeDesc)
		:m_eType(type)
		,m_lpszDescName(lpszDescName)
		,m_bFiled(bFiled)
		,m_bExclusive(bExclusive)
		,m_nChildTypes(0)
		,m_ppNodeDesc(ppNodeDesc)
{
	while(ppNodeDesc && ppNodeDesc[m_nChildTypes])
		m_nChildTypes ++;
}

cTreeNodeDesc_imp::~cTreeNodeDesc_imp()
{
	m_eType = (Node_Type)-1;
	m_lpszDescName = NULL;
	m_bFiled = FALSE;
	m_bExclusive = FALSE;
	m_nChildTypes = 0;
	m_ppNodeDesc = NULL;
}

const cNodeDesc * cTreeNodeDesc_imp::GetChildTypeAllowed(UINT n)const 			// 允许包含的节点类型
{
	return n<m_nChildTypes?m_ppNodeDesc[n]:NULL;
}

BOOL cTreeNodeDesc_imp::IfChildTypeAllowed(Node_Type type)const 				// 是否允许包含指定的节点类型
{
	for(UINT n=0; n<m_nChildTypes;n++)
	{
		if( type == m_ppNodeDesc[n]->GetType())
			return TRUE;
	}
	return FALSE;
}

cTreeNode_Imp::cTreeNode_Imp(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent)
	:m_pNodeDesc((cTreeNodeDesc*)pDesc)
	,m_nChildren(0)
	,m_ppChildren(NULL)
	,m_nAlloc(0)
	,m_pParent(pParent)
{
	lstrcpy(m_szName,lpszName);
	lstrcpy(m_szAlias,lpszAlias);

	// 自动添加独占类型的子节点
	for(UINT n=0;n<m_pNodeDesc->GetChildTypeAllowedCount();n++)
	{
		const cNodeDesc * pDesc = m_pNodeDesc->GetChildTypeAllowed(n);
		if(pDesc->IfExclusive())
		{
			Node_Type type = pDesc->GetType();
			if(g_NodeTable[type].pNodeDesc)
				NewChildNode(type,NULL,NULL);
		}
	}
}
cTreeNode_Imp::~cTreeNode_Imp()
{
	for(UINT n=0;n<m_nChildren;n++)
		delete m_ppChildren[n];
	delete []m_ppChildren;
	m_pNodeDesc = NULL;
	m_szName[0] = 0;
	m_nChildren = 0;
	m_ppChildren = NULL;
	m_nAlloc = 0;
}

const TCHAR * cTreeNode_Imp::GetPath(TCHAR * lpszBuf,size_t size) const
{
	// todo: 增加根据 size 的 buf 超界判断
	if(m_pParent)
		m_pParent->GetPath(lpszBuf,size);
	lstrcat(lpszBuf,m_szAlias); 
	lstrcat(lpszBuf,"/"); 
	return lpszBuf;
}

const cBaseNode * cTreeNode_Imp::GetNodeByPath(const TCHAR * lpszPath) const
{
	// 第一层子目录名必须跟自己的名字相同
	if(_strnicmp(lpszPath,m_szAlias,strlen(m_szAlias)))
		return NULL;

	// 如果路径已经结束了,就返回自己
	const TCHAR * lpszLeftPath = lpszPath + strlen(m_szAlias);
	if(!lpszLeftPath[0] || ((strlen(lpszLeftPath) == 1) && ((lpszLeftPath[0] == '/') || (lpszLeftPath[0] == '\\'))))
		return this;

	// 如果还有剩余路径，在子节点中寻找
	for(UINT n=0;n<m_nChildren;n++)
	{
		const cBaseNode * pNode = m_ppChildren[n]->GetNodeByPath(lpszLeftPath+1);
		if(pNode)
			return pNode; 
	}

	return NULL;
}

BOOL cTreeNode_Imp::AddChildNode(cBaseNode * pNode)
{
	// 类型是否允许
	
	const cNodeDesc * pDesc = pNode->GetDesc();
	Node_Type type = pDesc->GetType();
	
	BOOL bAllowed = FALSE;
	for(UINT n=0; n<m_pNodeDesc->GetChildTypeAllowedCount();n++)
	{
		if(m_pNodeDesc->GetChildTypeAllowed(n)->GetType() == type)
		{
			bAllowed = TRUE;
			break;
		}
	}
	if(!bAllowed)
		return FALSE;

	// 是否独占类型已经存在
	
	if(pDesc->IfExclusive())
	{
		for(UINT n=0;n<m_nChildren;n++)
		{
			if(m_ppChildren[n]->GetDesc()->GetType() == type)
				return FALSE;
		}
	}
	
	// 是否需要重新分配空间

	if(m_nChildren == m_nAlloc)
	{
		cBaseNode ** ppTemp = new cBaseNode *[m_nAlloc+ALLOC_GROW];
		if(m_ppChildren)
		{
			memcpy(ppTemp,m_ppChildren,sizeof(cBaseNode *)*m_nChildren);
			delete []m_ppChildren;
		}
		m_ppChildren = ppTemp;
		m_nAlloc += ALLOC_GROW;
	}
	
	// 添加子节点
	m_ppChildren[m_nChildren] = pNode;
	m_nChildren ++;
	return TRUE;
}

cBaseNode * cTreeNode_Imp::NewChildNode(Node_Type type,TCHAR * lpszName,TCHAR * lpszAlias)		// 新建子节点
{
	TCHAR szNewName[MAX_NAME],szNewAlias[MAX_NAME];
	if(!lpszName)
	{
		if(g_NodeTable[type].pNodeDesc->IfExclusive())
		{
			lpszName = g_NodeTable[type].lpszNodeName;
			lpszAlias = g_NodeTable[type].lpszNodeAlias;
		}
		else
		{
			sprintf(szNewName,"%s%_%02d",g_NodeTable[type].lpszNodeName,m_nChildren+1);  
			lpszName = szNewName;
			sprintf(szNewAlias,"%s%_%02d",g_NodeTable[type].lpszNodeAlias,m_nChildren+1);  
			lpszAlias = szNewAlias;
		}
	}

	cBaseNode * pNode = NULL;
	if(g_NodeTable[type].pNodeDesc->IsLeaf())
	{
		if(type != Node_UnitAttrib && type != Node_ContactAttrib)
		{
			if(type == Node_UnitFsm)
			{
				pNode = new cFsmLeafNode(g_NodeTable[type].pNodeDesc, lpszName,lpszAlias,this);
			}
			else
			{
				pNode = new cLeafNode_Imp(g_NodeTable[type].pNodeDesc, lpszName,lpszAlias,this);
			}
		}
		else
		{
			if(type == Node_UnitAttrib)
			{
				pNode = new cAttribLeafNode(g_NodeTable[type].pNodeDesc, lpszName,lpszAlias,this);
			}
			else
			{
				pNode = new cContactAttribLeafNode(g_NodeTable[type].pNodeDesc, lpszName,lpszAlias,this);
			}
		}
	}
	else
	{
		if(type != Node_Unit && type != Node_Contact)
			pNode = new cTreeNode_Imp(g_NodeTable[type].pNodeDesc, lpszName,lpszAlias,this); 
		else
		{
			if(type == Node_Unit)
			{
				pNode = new cUnitTreeNode(g_NodeTable[type].pNodeDesc, lpszName,lpszAlias,this);
			}
			else if(type == Node_Contact)
			{
				pNode = new cContactTreeNode(g_NodeTable[type].pNodeDesc, lpszName,lpszAlias,this);
			}
			else if(type == Node_UnitTree)
			{
				pNode = new cUnitMgrTreeNode(g_NodeTable[type].pNodeDesc, lpszName,lpszAlias,this);
			}
		}
	}
	BOOL bRet = AddChildNode(pNode);
	if(!bRet)
	{
		delete pNode;
		return NULL;
	}
	return pNode;
}
BOOL cTreeNode_Imp::DelChildNode(cBaseNode * pNode)			// 删除子节点
{
	// 寻找子节点
	for(UINT n=0;n<m_nChildren;n++)
	{
		if(pNode == m_ppChildren[n] )
		{
			// 把后面的向前挪动
			UINT nMove = m_nChildren - n - 1;
			if(nMove)
				MoveMemory(m_ppChildren+n,m_ppChildren+n+1,sizeof(cBaseNode*)*nMove); 
			// 最后一项清除。
			m_ppChildren[m_nChildren-1] = NULL;
			m_nChildren --;
			return TRUE;
		}
	}
	return FALSE;
}

void cUnitTreeNode::RefreshUnitTreeNode()
{
	// 数据模板	限制模板 影响模板 BUFF模板 状态机
	BOOL bClassElement[] = {m_pUnitDef->m_bHasTemplate,m_pUnitDef->m_bHasLimit,m_pUnitDef->m_bHasRelation,m_pUnitDef->m_bHasBuff,m_pUnitDef->m_bHasFsm};
	BOOL bNodeElement[] = {FALSE,FALSE,FALSE,FALSE,FALSE};
	Node_Type eType[] = {Node_UnitTemplate,Node_UnitLimite,Node_UnitEffect,Node_BuffEffect,Node_UnitFsm};
	UINT nCount = sizeof(bClassElement)/sizeof(BOOL);

	for(UINT n=0;n<nCount;n++)
	{
		cBaseNode * pDelNode = NULL;
		for(UINT i=0;i<m_nChildren;i++)
		{
			if(m_ppChildren[i]->GetDesc()->GetType()== eType[n])
			{
				pDelNode = m_ppChildren[i];
				bNodeElement[n] = TRUE;
				break;
			}
		}

		// 添加
		if(bClassElement[n] && !bNodeElement[n])
			NewChildNode(eType[n],NULL,NULL);
		// 删除
		else if(!bClassElement[n] && bNodeElement[n])
			DelChildNode(pDelNode); 
	}
}


BOOL cUnitTreeNode::ChildExist(const cNodeDesc * pDesc)const 
{
	// 单元节点有
	if( m_pUnitDef)
	{
		if(	(!m_pUnitDef->m_bHasTemplate && (pDesc->GetType() == Node_UnitTemplate))
			||(!m_pUnitDef->m_bHasLimit && (pDesc->GetType() == Node_UnitLimite))
			||(!m_pUnitDef->m_bHasRelation && (pDesc->GetType() == Node_UnitEffect))
			//||(!m_pUnitDef->m_bHasRelation && ((pDesc->GetType() == Node_UnitLimite) || (pDesc->GetType() == Node_UnitEffect)))
			||(!m_pUnitDef->m_bHasBuff && (pDesc->GetType() == Node_BuffEffect))
			||(!m_pUnitDef->m_bHasFsm && (pDesc->GetType() == Node_UnitFsm)) )
		{
			return FALSE;
		}
	}
	return TRUE;
}

cUnitTreeNode::cUnitTreeNode(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent)
	:cTreeNode_Imp(pDesc,pParent)
	,m_pUnitDef(NULL)
	,m_ppRelations(NULL) 
	,m_nRelations(0)
	,m_ppFieldEvents(NULL)
	,m_nFields(0)
{
	lstrcpy(m_szName,lpszName);
	lstrcpy(m_szAlias,lpszAlias);

	// 单元节点,寻找自己的单元描述
	if(m_pNodeDesc->GetType()==Node_Unit)
	{
		cLeafNode * pNode = g_NodeStreamImp.GetUnitManager(); 
		UINT nRow = pNode->FindRow(1,m_szName);
		if(nRow != -1)
		{
			const cLeafRow * pRow = pNode->GetRow(nRow); 
			m_pUnitDef = (UnitDef *)pRow->GetBuffer(); 
		}
		else
			return;

		std::vector<Relation *> vecRelations;
		pNode = g_NodeStreamImp.GetRelation();
		for(UINT n=0;n<pNode->GetRows();n++)
		{
			Relation * r = (Relation *)pNode->GetRow(n)->GetBuffer();
			if(r->nOperUnitID == m_pUnitDef->m_nID)
				vecRelations.push_back(r); 
		}
		m_nRelations = vecRelations.size();
		if(m_nRelations)
		{
			m_ppRelations = new Relation *[m_nRelations];
			CopyMemory(m_ppRelations,&vecRelations.front(),sizeof(Relation *)*m_nRelations); 
		}
	}

	// 自动添加独占类型的子节点
	for(UINT n=0;n<m_pNodeDesc->GetChildTypeAllowedCount();n++)
	{
		const cNodeDesc * pDesc = m_pNodeDesc->GetChildTypeAllowed(n);
		if(pDesc->IfExclusive())
		{
			if(!ChildExist(pDesc))
				continue;
			
			Node_Type type = pDesc->GetType();
			if(g_NodeTable[type].pNodeDesc)
				NewChildNode(type,NULL,NULL);
		}
	}
	///////////yuanke.lou add for cAttribLeafNode's m_pBaseClassDesc cann't correct load//////////
	//UINT nChilds = this->GetChildren();
	//for(UINT i=0;i<nChilds;i++)
	//{
	//	cLeafNode *pLeafNode = (cLeafNode *)this->GetChildNode(i);
	//	if(pLeafNode->GetDesc()->GetType() == Node_UnitAttrib)
	//	{
	//		cAttribLeafNode *pAttrib = (cAttribLeafNode*)pLeafNode;
	//		pAttrib->reInit();
	//	}
	//}
	///////////yuanke.lou add for cAttribLeafNode's m_pBaseClassDesc cann't correct load//////////
	// 
}

void cUnitTreeNode::SetFieldEvent(cFieldEvent * pEven,UINT nCol)
{
	if(!m_ppFieldEvents)
	{
		const cLeafNode * pAttrib = GetLeaf(Node_UnitAttrib); 
		m_nFields = pAttrib->GetRows(); 
		m_ppFieldEvents = new cFieldEvent*[m_nFields];
		ZeroMemory(m_ppFieldEvents,sizeof(cFieldEvent*)*m_nFields);
	}
	if(m_nFields > nCol)
		m_ppFieldEvents[nCol] = pEven;
}
const cFieldEvent * cUnitTreeNode::GetFieldEvent(UINT nCol) const
{
	if(m_ppFieldEvents && (m_nFields > nCol))
		return m_ppFieldEvents[nCol];
	return NULL;
}


cUnitTreeNode::~cUnitTreeNode()
{
	delete []m_ppRelations;
	m_ppRelations = NULL; 
	m_nRelations = 0;

	m_nFields = 0; 
	delete []m_ppFieldEvents;
	m_ppFieldEvents = NULL;
}

const cLeafNode * cUnitTreeNode::GetLeaf(Node_Type type) const
{
	for(UINT n=0;n<m_nChildren;n++) 
	{
		if(m_ppChildren[n]->GetDesc()->GetType() == type)
			return (cLeafNode*)m_ppChildren[n];
	}
	return NULL;
}
const cLeafNodeDesc * cUnitTreeNode::GetAttribDesc() const
{
	const cLeafNode * pAttribNode = GetLeaf(Node_UnitAttrib);
	cTemplateLeafNodeDesc * pTemplateDesc = (cTemplateLeafNodeDesc*)((cAttribLeafNode*)pAttribNode)->GetTargetDesc();
	return pTemplateDesc; 
}

cContactTreeNode::cContactTreeNode(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent)
:cTreeNode_Imp(pDesc,pParent)
	,m_ppFieldEvents(NULL)
	,m_nFields(0)
{
	lstrcpy(m_szName,lpszName);
	lstrcpy(m_szAlias,lpszAlias);

	// 联系节点,寻找自己的联系描述
	if(m_pNodeDesc->GetType()==Node_Contact)
	{
		cLeafNode * pNode = g_NodeStreamImp.GetContactManager();  
		UINT nRow = pNode->FindRow(1,m_szName);
		if(nRow != -1)
		{
			const cLeafRow * pRow = pNode->GetRow(nRow); 
			m_pContactDef = (ContactDef *)pRow->GetBuffer(); 
		}
		else
			return;
	}

	// 自动添加独占类型的子节点
	for(UINT n=0;n<m_pNodeDesc->GetChildTypeAllowedCount();n++)
	{
		const cNodeDesc * pDesc = m_pNodeDesc->GetChildTypeAllowed(n);
		if(pDesc->IfExclusive())
		{
			if(!ChildExist(pDesc))
				continue;

			Node_Type type = pDesc->GetType();
			if(g_NodeTable[type].pNodeDesc)
				NewChildNode(type,NULL,NULL);
		}
	}

	// 
}

BOOL cContactTreeNode::ChildExist(const cNodeDesc * pDesc)const 
{
	return TRUE;
}

const cLeafNode * cContactTreeNode::GetLeaf(Node_Type type) const
{
	for(UINT n=0;n<m_nChildren;n++) 
	{
		if(m_ppChildren[n]->GetDesc()->GetType() == type)
			return (cLeafNode*)m_ppChildren[n];
	}
	return NULL;
}

void cContactTreeNode::RefreshContactTreeNode()
{
	// 联系定义	联系数据
	BOOL bContactElement[] = {TRUE,TRUE};
	BOOL bNodeElement[] = {FALSE,FALSE};
	Node_Type eType[] = {Node_ContactAttrib,Node_ContactData};
	UINT nCount = sizeof(bContactElement)/sizeof(BOOL);

	for(UINT n=0;n<nCount;n++)
	{
		cBaseNode * pDelNode = NULL;
		for(UINT i=0;i<m_nChildren;i++)
		{
			if(m_ppChildren[i]->GetDesc()->GetType()== eType[n])
			{
				pDelNode = m_ppChildren[i];
				bNodeElement[n] = TRUE;
				break;
			}
		}

		// 添加
		if(bContactElement[n] && !bNodeElement[n])
			NewChildNode(eType[n],NULL,NULL);
		// 删除
		else if(!bContactElement[n] && bNodeElement[n])
			DelChildNode(pDelNode); 
	}
}

void cContactTreeNode::SetFieldEvent(cFieldEvent * pEven,UINT nCol)
{
	if(!m_ppFieldEvents)
	{
		const cLeafNode * pAttrib = GetLeaf(Node_ContactAttrib); 
		m_nFields = pAttrib->GetRows(); 
		m_ppFieldEvents = new cFieldEvent*[m_nFields];
		ZeroMemory(m_ppFieldEvents,sizeof(cFieldEvent*)*m_nFields);
	}
	if(m_nFields > nCol)
		m_ppFieldEvents[nCol] = pEven;
}
const cFieldEvent * cContactTreeNode::GetFieldEvent(UINT nCol) const
{
	if(m_ppFieldEvents && (m_nFields > nCol))
		return m_ppFieldEvents[nCol];
	return NULL;
}

const cLeafNodeDesc * cContactTreeNode::GetAttribDesc() const
{
	const cLeafNode * pAttribNode = GetLeaf(Node_ContactAttrib);
	cContactDataLeafNodeDesc * pContactDesc = (cContactDataLeafNodeDesc*)((cContactAttribLeafNode*)pAttribNode)->GetTargetDesc();
	return pContactDesc; 
}


int cUnitMgrTreeNode::compare( const void *c1, const void *c2)
{
	cBaseNode ** pNode1 = (cBaseNode **)c1;
	cBaseNode ** pNode2 = (cBaseNode **)c2;
	assert((*pNode1)->GetDesc()->GetType() == Node_Unit);
	assert((*pNode2)->GetDesc()->GetType() == Node_Unit);
	cUnitTreeNode * pUnit1 = *(cUnitTreeNode **)pNode1;
	cUnitTreeNode * pUnit2 = *(cUnitTreeNode **)pNode2;

	const UnitDef * pUnitDef1 = pUnit1->GetUnitDef();
	const UnitDef * pUnitDef2 = pUnit2->GetUnitDef();

	if(pUnitDef1->m_nBaseClass == pUnitDef2->m_nBaseClass)
		return pUnitDef1->m_nID - pUnitDef2->m_nID;
	return pUnitDef1->m_nBaseClass - pUnitDef2->m_nBaseClass;
}
void cUnitMgrTreeNode::SortChildren()
{
	qsort(m_ppChildren,m_nChildren,sizeof(cBaseNode	*),compare);
}