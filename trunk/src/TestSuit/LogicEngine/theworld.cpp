#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <strstream>
#include <assert.h>
#include <shlwapi.h>
#include "xlist.h"
#include "LogicEngine.h"
#include "TreeNode.h"
#include "LeafNode.h"
#include "theworld.h"
#include "pugxml.h"
#include "BinFile.h"

#define OVERLOAD
#include "overload.h"

// #define _CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>
// #ifdef _DEBUG
// #define   new   new(_NORMAL_BLOCK,   __FILE__,   __LINE__)
// #endif

#ifdef _DEBUG
#define FORMULAMANAGER_DLL	_T("FormulaCenterD.dll")
#else
#define FORMULAMANAGER_DLL	_T("FormulaCenter.dll")
#endif
#define GetFormulaManager_FN _T("GetFormulaManager")


#define PATH_UNIT_ATTRIB	_T("Root/UnitTree/%s/UnitAttrib")
#define PATH_UNIT_TEMPLATE	_T("Root/UnitTree/%s/UnitTemplate")
#define PATH_BUFF_EFFECT	_T("Root/UnitTree/%s/BuffEffect")
#define PATH_UNIT_TREE		_T("Root/UnitTree")
#define PATH_UNIT2			_T("Root/UnitTree/%s")
#define PATH_UNIT_MANAGER	_T("Root/UnitManager")
#define PATH_UNIT			_T("UnitTree/%s")
#define PATH_CONTACT		_T("ContactTree/%s")
#define PATH_CONTACT_DATA	_T("Root/ContactTree/%s/ContactData")
#define PATH_CONTACT_ATTRIB	_T("Root/ContactTree/%s/ContactAttrib")


#define CONTACT_MANAGER "Root/ContactManager"
#define CONTACT_DATA	"Root/ContactTree"

#define CONTACT_NAME_COL	1
#define CONTACT_ALIAS_COL	2
#define UNIT_NAME_COL	1
#define UNIT_ALIAS_COL	2

#define CONTACT_EFFECT_VALUE_SCRIPT	_T("EffectValueScript")	//影响值计算脚本
#define CONTACT_EFFECT_OP_SCRIPT	_T("EffectOpScript")	//影响操作
#define CONTACT_SKILL_TYPE			_T("SkillType")			//技能类型

extern void SplitString(std::string str,std::vector<std::string>& vecString,std::string spliter);
NodeTable g_NodeTable[];	

cWorldImp::cWorldImp()
	:m_pWorldRoot(NULL)
	,m_ppUnits(NULL)
	,m_nUnits(0)
	,m_pRemain(NULL)
	,m_hFormulaMG(NULL)
	,m_pFormulaManager(NULL)
	,m_pMemPool(NULL)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	ReadConfig("./path.ini");

	// 如果配置文件中没有指定脚本路径,就不装载了.
	if(!m_sScript.empty())
	{
		LoadLib();
		m_pFormulaManager->SetLuaScriptPath((TCHAR *)m_sScript.c_str());
	}
}
cWorldImp::~cWorldImp()
{
}

BOOL cWorldImp::LoadLib()
{
	// 装载动态连接库
	m_hFormulaMG = LoadLibrary(FORMULAMANAGER_DLL);
	if(!m_hFormulaMG)
	{
		//无法找到公式库
		::MessageBox(NULL, _T("无法找到公式库"), _T("郁闷"), MB_OK);
		return FALSE;
	}
	FnGetFormulaManager * pFnGetFormulaMG = (FnGetFormulaManager *)GetProcAddress( m_hFormulaMG, GetFormulaManager_FN);
	if(!pFnGetFormulaMG)
		return FALSE;
	m_pFormulaManager = pFnGetFormulaMG();

	return TRUE;
}

cBaseNode * cWorldImp::LoadFromBinFile(FILE * fp,cBaseNode * pParent)				// 从2进制文件装载
{
	LogicHeader header;
	if(fread(&header,sizeof(LogicHeader),1,fp) != 1)
		return NULL;
	if( (header.dwFlag != NODE_FLAG) ||(header.nVersion != NODE_VER) )
		return NULL;

	NodeDescEx desc;
	if(fread(&desc,sizeof(NodeDescEx),1,fp) != 1 )
		return NULL;

	// 寻找节点描述
	NodeTable * pNodeTable = NULL;
	for(int i=0;i<Node_Type_Max;i++)
	{
		if(g_NodeTable[i].pNodeDesc)
		{
			if(g_NodeTable[i].pNodeDesc->GetType() == desc.m_eType)
			{
				pNodeTable = &g_NodeTable[i];
				break;
			}
		}
	}
	if(!pNodeTable)
		return NULL;
	cNodeDesc * pDesc = pNodeTable->pNodeDesc;

	// 寻找或者创建子节点
	cBaseNode * pNode = NULL;
	if(pParent)
	{
		cTreeNode * pTreeNode = (cTreeNode *)pParent;
		if(pDesc->IfExclusive())
		{
			for(UINT n=0;n<pTreeNode->GetChildren();n++)
			{
				cBaseNode * pExist = pTreeNode->GetChildNode(n);
				if(pExist->GetDesc()->GetType() == pDesc->GetType())
				{
					pNode = pExist;
					break;
				}
			}
		}
		else
		{
			//const TCHAR * lpszAlias = NULL;
			//if(desc.m_eType == Node_Unit)
			//{
			//	const cLeafNode * pUnitMgrNode = (const cLeafNode *)pParent->GetParent()->GetNodeByPath(PATH_UNIT_MANAGER);
			//	if(pUnitMgrNode)
			//	{
			//		UINT nClasses = pUnitMgrNode->GetRows();
			//		for(UINT n=0;n<nClasses;n++)
			//		{
			//			const TCHAR * lpszClassName = pUnitMgrNode->GetString(n,_T(UNIT_NAME_COL)); 
			//			if(!_tcscmp(lpszClassName,desc.m_szName))
			//			{
			//				lpszAlias = pUnitMgrNode->GetString(n,_T(UNIT_ALIAS_COL)); 
			//				break;
			//			}
			//		}
			//	}	
			//}
			//else if(desc.m_eType == Node_Contact)
			//{
			//	const cLeafNode * pContactMgrNode = (const cLeafNode *)pParent->GetParent()->GetNodeByPath(CONTACT_MANAGER);
			//	if(pContactMgrNode)
			//	{
			//		UINT nContacts = pContactMgrNode->GetRows();
			//		for(UINT n=0;n<nContacts;n++)
			//		{
			//			const TCHAR * lpszContactName = pContactMgrNode->GetString(n,_T(CONTACT_NAME_COL)); 
			//			if(!_tcscmp(lpszContactName,desc.m_szName))
			//			{
			//				lpszAlias = pContactMgrNode->GetString(n,_T(CONTACT_ALIAS_COL)); 
			//				break;
			//			}
			//		}
			//	}	
			//}
			//else
			//{
			//	assert(0 && "节点类型不对");
			//}
			//assert(lpszAlias);
			pNode = pTreeNode->NewChildNode(pDesc->GetType(),desc.m_szName,desc.m_szAlias);  
		}
	}
	else
	{
		// 创建世界根节点
		assert(!pDesc->IsLeaf());
		pNode = new cTreeNode_Imp(pDesc,pNodeTable->lpszNodeName,pNodeTable->lpszNodeAlias,NULL);
		m_pWorldRoot = pNode;
	}
	if(!pNode)
		return NULL;

	// 树节点
	if(!pDesc->IsLeaf())
	{
		UINT nChildren = 0;
		if(fread(&nChildren,sizeof(UINT),1,fp) != 1 )
			return NULL;
		if(!nChildren)
			return pNode;

		for(UINT i=0;i<nChildren;i++)
		{
			// 装载子节点
			if(!LoadFromBinFile(fp,pNode))
				{/*return NULL;*/}
		}
	}
	// 叶节点
	else
	{
		// 叶节点
		cLeafNode * pLeafNode = (cLeafNode *)pNode;
		cLeafNodeDesc * pLeafDesc = (cLeafNodeDesc *)pLeafNode->GetDesc(); 
		Node_Type nt = pLeafDesc->GetType();

		LeafNodeDesc leaf;
		if(fread(&leaf,sizeof(LeafNodeDesc),1,fp) != 1 )
			return NULL;

		if(leaf.nRows > 0)
		{
			((cLeafNode_Imp*)pLeafNode)->ValidRow(leaf.nRows,FALSE); 

			for(UINT i=0;i<leaf.nRows;i++)
			{
				const cLeafRow * pRow = pLeafNode->GetRow(i);
				const void * pBuffer = pRow->GetBuffer();
				fread((void *)pBuffer,leaf.nRowLength,1,fp);

				////////////////////20080625公式依赖问题//////////////////////
				if(nt == Node_UnitAttrib)
				{
					cAttribLeafNode *pAttrib = (cAttribLeafNode*)pLeafNode;
					
					if(!pRow->GetInteger(LeafNode_IsDeriveField-1))
					{
						pAttrib->RemoveDeriveField(i);
					}
					else
					{
						const TCHAR * lpszRef = pRow->GetString(LeafNode_RefField-1);
						if(lpszRef)
						{
							pAttrib->SetDependField(i,std::string(lpszRef));
						}									
					}
				}
				////////////////////20080625公式依赖问题//////////////////////

				// 特例
				if( ((pDesc->GetType()==Node_UnitAttrib)&&(pLeafNode->GetInteger(i,LeafNode_Type-1)==Field_Enumerate))
					|| ((pDesc->GetType()==Node_ContactAttrib)&&(pLeafNode->GetInteger(i,ContactData_Type-1)==Field_Enumerate)))
				{
					UINT nEnum = 0;
					if(fread(&nEnum,sizeof(UINT),1,fp) != 1 )
						return NULL;

					if(nEnum)
					{
						TCHAR * lpszEnum = new TCHAR[nEnum * MAX_NAME];
						ZeroMemory(lpszEnum,sizeof(TCHAR)*nEnum * MAX_NAME);
						fread(lpszEnum,MAX_NAME,nEnum,fp);
						pLeafNode->SetEnumString(i,lpszEnum,nEnum); 
						delete []lpszEnum;
					}
				}
				else if( ((pDesc->GetType()==Node_UnitAttrib)&&(pLeafNode->GetInteger(i,LeafNode_Type-1)==Field_IdField))
					|| ((pDesc->GetType()==Node_ContactAttrib)&&(pLeafNode->GetInteger(i,ContactData_Type-1)==Field_IdField)))
				{
					FieldIdDesc fd;
					if(fread(&fd,sizeof(FieldIdDesc),1,fp) != 1 )
						return NULL;

					FieldId f;							
					f.nDispField = fd.nDispField;
					f.nValField = fd.nValField;
					f.pLeafPath = fd.szLeafPath; 
					if(f.nDispField && fd.szLeafPath[0])
						pLeafNode->SetFieldID(i,&f); 
				}
			}

			if(pDesc->GetType()==Node_UnitAttrib)
			{
				// 建立字段间依赖的排序
				cAttribLeafNode *pAttrib = (cAttribLeafNode*)pLeafNode;
				pAttrib->BuildOrderLst();
				// 要刷新自己对数据模板的描述.
				cTemplateLeafNodeDesc * pTemplateDesc = (cTemplateLeafNodeDesc*)((cAttribLeafNode*)pLeafNode)->GetTargetDesc();
				pTemplateDesc->Build(); 
			}
			else if(pDesc->GetType()==Node_ContactAttrib)
			{
				// 要刷新自己对数据模板的描述.
				cContactDataLeafNodeDesc * pContactDesc = (cContactDataLeafNodeDesc*)((cContactAttribLeafNode*)pLeafNode)->GetTargetDesc();
				pContactDesc->Build();
			}
			pLeafNode->BuildIndex(); 
		}
	}
	return pNode;
}

cBaseNode * cWorldImp::LoadFromXmlNode(HANDLE hXmlNode,cBaseNode * pParent)		// 从Xml节点装载
{
	pug::xml_node * pXmlNode = (pug::xml_node * )hXmlNode;
	std::string strType = pXmlNode->attribute("Type").getvalue();
	std::string strName = pXmlNode->attribute("Name").getvalue();
	std::string strAlias = pXmlNode->attribute("Alias").getvalue();

	// 寻找节点描述
	cNodeDesc * pDesc = NULL;
	for(int i=0;i<Node_Type_Max;i++)
	{
		if(g_NodeTable[i].pNodeDesc)
		{
			if(!lstrcmpi(g_NodeTable[i].pNodeDesc->GetDescName(),strType.c_str()))
			{
				pDesc = g_NodeTable[i].pNodeDesc;
				break;
			}
		}
	}
	if(!pDesc)
		return NULL;

	// 寻找或者创建子节点
	cBaseNode * pNode = NULL;
	if(pParent)
	{
		cTreeNode * pTreeNode = (cTreeNode *)pParent;
		if(pDesc->IfExclusive())
		{
			for(UINT n=0;n<pTreeNode->GetChildren();n++)
			{
				cBaseNode * pExist = pTreeNode->GetChildNode(n);
				if(pExist->GetDesc()->GetType() == pDesc->GetType())
				{
					pNode = pExist;
					break;
				}
			}
		}
		else
		{
			pNode = pTreeNode->NewChildNode(pDesc->GetType(),(TCHAR *)strName.c_str(),(TCHAR *)strAlias.c_str());  
		}
	}
	else
	{
		// 创建世界根节点
		assert(!pDesc->IsLeaf());
		pNode = new cTreeNode_Imp(pDesc,(TCHAR *)strName.c_str(),(TCHAR *)strAlias.c_str(),NULL);
		m_pWorldRoot = pNode;
	}
	if(!pNode)
		return NULL;

	// 树节点
	if(!pDesc->IsLeaf())
	{
		UINT nChildren = (long)pXmlNode->attribute(_T("Children"));
		if(!nChildren)
			return pNode;

		std::string strTreeType = pXmlNode->attribute("Type").getvalue();
		// 修正原因为单元定义的先后不同，ID小的单元继承ID大的单元，造成的继承失败问题
		// ID	名称	父单元
		// 3	玩家	13
		// ...	...		...
		// 13	角色	1
		// 如上问题
		// 如果是单元数据节点，继承间依赖关系的确保
		if(!lstrcmpi(strTreeType.c_str(),"UnitTree"))
		{
			pug::xml_node child_node = pXmlNode->first_element_by_name(_T("Node"));
			if(child_node.empty())
				return NULL;
			cLeafNode *pUnitM = GetUnitManager();
			TCHAR lpszNodePath[MAX_PATH] = {0};
			if(!pUnitM)
				return NULL;
			for(UINT i=0;i<nChildren;i++)
			{
				std::string strUnitName = child_node.attribute("Name").getvalue();

				ZeroMemory(lpszNodePath,sizeof(lpszNodePath));
				sprintf(lpszNodePath,PATH_UNIT_ATTRIB,strUnitName.c_str());
				const cBaseNode *pMyNode = GetRoot()->GetNodeByPath(lpszNodePath);
				// 如果自身已被装载过，则跳过(以为可能存在前面的单元继承后面的单元，所以后面的接点有可能已经被装载过)
				if(!pMyNode)
				{
					UINT nRow = pUnitM->FindRow(1,strUnitName.c_str());
					const cLeafRow *pLeafRow = pUnitM->GetRow(nRow);
					UINT nParentID = pLeafRow->GetInteger("父单元");
					// 父单元ID为0以为着自身是超类，不继承任何单元，直接装载即可，不存在向上依赖
					if(nParentID==0)
					{
						// 装载子节点
						bool bFiled = child_node.attribute(_T("Filed")); 
						// 子节点失败不退出
						// TODO:报告子节点错误信息
						if(bFiled)
						{
							strName = child_node.attribute("Name").getvalue();
							//LoadFromFile(strName.c_str(),pNode);
							if(!LoadFromFile(strName.c_str(),pNode))
								{/*return NULL;*/}
						}
						else
						{
							//LoadFromXmlNode(&child_node,pNode);
							if(!LoadFromXmlNode(&child_node,pNode))
								{/*return NULL;*/}
						}
					}
					else
					{
						UINT nParentRow = pUnitM->FindRow((UINT)0,nParentID);
						if(nParentRow != -1)
						{
							const TCHAR * lpszParentName = pUnitM->GetRow(nParentRow)->GetString(UNIT_NAME_COL);
							ZeroMemory(lpszNodePath,sizeof(lpszNodePath));
							sprintf(lpszNodePath,PATH_UNIT_ATTRIB,lpszParentName);
							const cBaseNode *pParentNode = GetRoot()->GetNodeByPath(lpszNodePath);
							// 如果父接点已经装载，直接装载该接点，否则先装载父亲接点
							if(pParentNode)
							{
								// 装载子节点
								bool bFiled = child_node.attribute(_T("Filed")); 
								// 子节点失败不退出
								// TODO:报告子节点错误信息
								if(bFiled)
								{
									strName = child_node.attribute("Name").getvalue();
									//LoadFromFile(strName.c_str(),pNode);
									if(!LoadFromFile(strName.c_str(),pNode))
										{/*return NULL;*/}
								}
								else
								{
									//LoadFromXmlNode(&child_node,pNode);
									if(!LoadFromXmlNode(&child_node,pNode))
										{/*return NULL;*/}
								}
							}
							// 装载父接点之后装载自身接点，当心有遗漏的接点没有装载
							else
							{
								pug::xml_node child_node_find = pXmlNode->first_element_by_name(_T("Node"));
								if(child_node_find.empty())
									return NULL;
								for(UINT j=0;j<nChildren;j++)
								{
									// 优先装载父接点
									std::string strTempName = child_node_find.attribute("Name").getvalue();
									if(!lstrcmpi(strTempName.c_str(),lpszParentName))
									{
										// 装载父节点
										bool bFiled = child_node_find.attribute(_T("Filed")); 
										// 子节点失败不退出
										// TODO:报告子节点错误信息
										if(bFiled)
										{
											//strName = (std::string)child_node_find.attribute("Name");
											//LoadFromFile(strName.c_str(),pNode);
											if(!LoadFromFile(strTempName.c_str(),pNode))
												{/*return NULL;*/}
										}
										else
										{
											//LoadFromXmlNode(&child_node,pNode);
											if(!LoadFromXmlNode(&child_node_find,pNode))
												{/*return NULL;*/}
										}

										// 装载子节点
										bFiled = child_node.attribute(_T("Filed")); 
										// 子节点失败不退出
										// TODO:报告子节点错误信息
										if(bFiled)
										{
											strName = child_node.attribute("Name").getvalue();
											//LoadFromFile(strName.c_str(),pNode);
											if(!LoadFromFile(strName.c_str(),pNode))
												{/*return NULL;*/}
										}
										else
										{
											//LoadFromXmlNode(&child_node,pNode);
											if(!LoadFromXmlNode(&child_node,pNode))
												{/*return NULL;*/}
										}
										break;
									}
									// 下一个节点
									if(!child_node_find.moveto_next_sibling())
										break;
								}
							}
						}
						// 未在单元管理中找到父接点的话，直接装载自身接点(认为父接点ID异常，暂不考虑)
						else
						{
							// 装载子节点
							bool bFiled = child_node.attribute(_T("Filed")); 
							// 子节点失败不退出
							// TODO:报告子节点错误信息
							if(bFiled)
							{
								strName = child_node.attribute("Name").getvalue();
								//LoadFromFile(strName.c_str(),pNode);
								if(!LoadFromFile(strName.c_str(),pNode))
									{/*return NULL;*/}
							}
							else
							{
								//LoadFromXmlNode(&child_node,pNode);
								if(!LoadFromXmlNode(&child_node,pNode))
									{/*return NULL;*/}
							}
						}
					}
				}

				// 下一个节点
				if(!child_node.moveto_next_sibling())
					break;

			}
		}
		else
		{
			pug::xml_node child_node = pXmlNode->first_element_by_name(_T("Node"));
			if(child_node.empty())
				return NULL;

			for(UINT i=0;i<nChildren;i++)
			{
				// 装载子节点
				bool bFiled = child_node.attribute(_T("Filed")); 
				// 子节点失败不退出
				// TODO:报告子节点错误信息
				if(bFiled)
				{
					strName = child_node.attribute("Name").getvalue();
					//LoadFromFile(strName.c_str(),pNode);
					if(!LoadFromFile(strName.c_str(),pNode))
						{/*return NULL;*/}
				}
				else
				{
					//LoadFromXmlNode(&child_node,pNode);
					if(!LoadFromXmlNode(&child_node,pNode))
						{/*return NULL;*/}
				}

				// 下一个节点
				if(!child_node.moveto_next_sibling())
					break;
			}
		}
	}
	// 叶节点
	else
	{
		// 叶节点
		cLeafNode * pLeafNode = (cLeafNode *)pNode;
		cLeafNodeDesc * pLeafDesc = (cLeafNodeDesc *)pLeafNode->GetDesc(); 
		Node_Type nt = pLeafDesc->GetType();
		UINT nRows = (long)pXmlNode->attribute(_T("Rows"));
		pug::xml_node row = pXmlNode->first_element_by_name(_T("Row"));
		if(!row.empty())
		{
			((cLeafNode_Imp*)pLeafNode)->ValidRow(nRows,FALSE); 

			for(UINT i=0;i<nRows;i++)
			{
				pug::xml_node field = row.first_element_by_name(_T("Field"));
				if(!field.empty())
				{
					for(UINT j=0;j<pLeafDesc->GetFieldCount();j++)
					{
						if(pLeafDesc->GetField(j)->m_nTemplateOffset>=0)
						{
							std::string value = field.child_value();
							////////////////////20080625公式依赖问题//////////////////////
							//if(nt == Node_UnitAttrib)
							//{
							//	cAttribLeafNode *pAttrib = (cAttribLeafNode*)pLeafNode;
							//	if(j==LeafNode_IsDeriveField-1)
							//	{
							//		if(!value.empty() && !value.compare("1"))
							//		{
							//			pAttrib->SetDependField(i,std::string(""));
							//		}
							//	}
							//	else if(j==LeafNode_RefField-1)
							//	{
							//		if(!value.empty() && pAttrib->IsDeriveField(i))
							//		{
							//			pAttrib->SetDependField(i,value);
							//		}									
							//	}
							//}
							////////////////////20080625公式依赖问题//////////////////////
							pLeafNode->SetString(i,j,(TCHAR*)value.c_str()); 
							if(!field.moveto_next_sibling())
								break;
						}
					}

					// 特例
					if( ((pDesc->GetType()==Node_UnitAttrib)&&(pLeafNode->GetInteger(i,LeafNode_Type-1)==Field_Enumerate))
						|| ((pDesc->GetType()==Node_ContactAttrib)&&(pLeafNode->GetInteger(i,ContactData_Type-1)==Field_Enumerate)))
					{
						pug::xml_node enum_node = row.first_element_by_name(_T("Enum"));
						if(!enum_node.empty())
						{
							UINT nEnum = (long)enum_node.attribute(_T("Count"));
							pug::xml_node enum_Item = enum_node.first_element_by_name(_T("Item"));
							if(!enum_Item.empty())
							{
								TCHAR * lpszEnum = new TCHAR[nEnum * MAX_NAME];
								ZeroMemory(lpszEnum,sizeof(TCHAR)*nEnum * MAX_NAME);
								for(UINT n=0;n<nEnum;n++)
								{
									std::string value = enum_Item.child_value();
									lstrcpyn(lpszEnum + n * MAX_NAME,value.c_str(),MAX_NAME-1);
									if(!enum_Item.moveto_next_sibling())
										break;
								}
								pLeafNode->SetEnumString(i,lpszEnum,nEnum); 
								delete []lpszEnum;
							}
						}
					}
					else if( ((pDesc->GetType()==Node_UnitAttrib)&&(pLeafNode->GetInteger(i,LeafNode_Type-1)==Field_IdField))
						|| ((pDesc->GetType()==Node_ContactAttrib)&&(pLeafNode->GetInteger(i,ContactData_Type-1)==Field_IdField)))
					{
						pug::xml_node idfield_node = row.first_element_by_name(_T("ID_Field"));
						if(!idfield_node.empty())
						{
							FieldId f;							
							f.nDispField = (long)idfield_node.attribute(_T("Field"));
							f.nValField = (long)idfield_node.attribute(_T("Value"));
							std::string sPath = idfield_node.attribute(_T("Path")).getvalue();
							f.pLeafPath = (TCHAR*)sPath.c_str(); 
							if(f.nDispField && !sPath.empty())
								pLeafNode->SetFieldID(i,&f); 
						}
					}
				}
				if(!row.moveto_next_sibling())
					break;
			}

			if(pDesc->GetType()==Node_UnitAttrib)
			{
				// 要刷新自己对数据模板的描述.
				cTemplateLeafNodeDesc * pTemplateDesc = (cTemplateLeafNodeDesc*)((cAttribLeafNode*)pLeafNode)->GetTargetDesc();
				pTemplateDesc->Build(); 
			}
			else if(pDesc->GetType()==Node_ContactAttrib)
			{
				// 要刷新自己对数据模板的描述.
				cContactDataLeafNodeDesc * pContactDesc = (cContactDataLeafNodeDesc*)((cContactAttribLeafNode*)pLeafNode)->GetTargetDesc();
				pContactDesc->Build();
			}
			pLeafNode->BuildIndex(); 
		}
	}
	return pNode;
}

void cWorldImp::SaveToBinFile(FILE * fp,cBaseNode * pNode)					// 保存到2进制文件
{
	LogicHeader h = {NODE_FLAG,NODE_VER};
	fwrite(&h,sizeof(LogicHeader),1,fp);
	
	const cNodeDesc * pDesc = pNode->GetDesc();
	NodeDescEx desc = {pDesc->GetType(),_T(""),_T(""),pDesc->IsFiled(),pDesc->IfExclusive(),pDesc->IsLeaf()};
	_tcscpy(desc.m_szName,pNode->GetName());
	_tcscpy(desc.m_szAlias,pNode->GetAlias());
	fwrite(&desc,sizeof(NodeDescEx),1,fp);
	
	if(!pDesc->IsLeaf())
	{
		cTreeNode * pTreeNode = (cTreeNode *)pNode;
		UINT nChildren = pTreeNode->GetChildren();
		fwrite(&nChildren,sizeof(UINT),1,fp);
		for(UINT i=0;i<nChildren;i++)
		{
			cBaseNode * pChildNode = pTreeNode->GetChildNode(i);
			if(pChildNode->GetDesc()->IsFiled())
				SaveToFile(pChildNode->GetName(),pChildNode,TRUE);
			else
				SaveToBinFile(fp,pChildNode);
		}
	}
	else
	{
		// 叶节点
		cLeafNode * pLeafNode = (cLeafNode *)pNode;
		cLeafNodeDesc * pDesc = (cLeafNodeDesc *)pNode->GetDesc(); 
		Node_Type nt = pDesc->GetType();

		LeafNodeDesc leaf = {pLeafNode->GetRows(),pDesc->GetRowLength()};
		fwrite(&leaf,sizeof(LeafNodeDesc),1,fp);

		for(UINT i=0;i<leaf.nRows;i++)
		{
			cLeafRow * pRow = (cLeafRow *)pLeafNode->GetRow(i);
			/////////////////////20080625 公式依赖问题//////////////////////////
			if(nt == Node_UnitAttrib)
			{
				cAttribLeafNode *pAttrib = (cAttribLeafNode*)pLeafNode;
				if(pAttrib->IsDeriveField(i))
				{
					pRow->SetInteger(LeafNode_IsDeriveField-1,1);
					std::string strTmp = pAttrib->GetDependField(i);
					pRow->SetString(LeafNode_RefField-1,strTmp.c_str());
				}
				else
				{
					pRow->SetInteger(LeafNode_IsDeriveField-1,0);
					pRow->SetString(LeafNode_RefField-1,"");
				}
			}
			/////////////////////20080625 公式依赖问题//////////////////////////
			const void * pBuffer = pRow->GetBuffer();
			fwrite(pBuffer,leaf.nRowLength,1,fp);


			// 特例
			if( ((pDesc->GetType()==Node_UnitAttrib)&&(pLeafNode->GetInteger(i,LeafNode_Type-1)==Field_Enumerate)) 
				|| ((pDesc->GetType()==Node_ContactAttrib)&&(pLeafNode->GetInteger(i,ContactData_Type-1)==Field_Enumerate)))
			{
				UINT nEnum = 0;
				const TCHAR * lpszEnum = pLeafNode->GetEnumString(i,nEnum);
				fwrite(&nEnum,sizeof(UINT),1,fp);
				if(nEnum)
					fwrite(lpszEnum,MAX_NAME,nEnum,fp);
			}
			else if( ((pDesc->GetType()==Node_UnitAttrib)&&(pLeafNode->GetInteger(i,LeafNode_Type-1)==Field_IdField))
				|| ((pDesc->GetType()==Node_ContactAttrib)&&(pLeafNode->GetInteger(i,ContactData_Type-1)==Field_IdField)))
			{
				FieldId f = {0};							
				pLeafNode->GetFieldID(i,&f);

				FieldIdDesc fd = {0};
				if(f.pLeafPath)
					_tcscpy(fd.szLeafPath,f.pLeafPath);
				fd.nDispField = f.nDispField;
				fd.nValField = f.nValField;
				
				fwrite(&fd,sizeof(FieldIdDesc),1,fp);
			}
		}	
	}
}

void cWorldImp::SaveToXmlNode(HANDLE hXmlNode,cBaseNode * pNode)		// 保存到Xml节点
{
	pug::xml_node * pXmlNode = (pug::xml_node *)hXmlNode; 
	pug::xml_node node = pXmlNode->append_child(pug::node_element,_T("Node"));

	// 通用
	node.append_attribute(_T("Type"),pNode->GetDesc()->GetDescName());
	node.append_attribute(_T("Name"),pNode->GetName());
	node.append_attribute(_T("Filed"),pNode->GetDesc()->IsFiled()?true:false);

	// 树节点
	if(!pNode->GetDesc()->IsLeaf())
	{
		cTreeNode * pTreeNode = (cTreeNode *)pNode;
		UINT nChildren = pTreeNode->GetChildren();
		node.append_attribute(_T("Children"),(long)nChildren);
		for(UINT i=0;i<nChildren;i++)
		{
			cBaseNode * pChildNode = pTreeNode->GetChildNode(i);
			if(pChildNode->GetDesc()->IsFiled())
				SaveToFile(pChildNode->GetName(),pChildNode);
			else
				SaveToXmlNode((HANDLE)&node,pChildNode);
		}
	}
	else
	{
		// 叶节点
		cLeafNode * pLeafNode = (cLeafNode *)pNode;
		cLeafNodeDesc * pDesc = (cLeafNodeDesc *)pNode->GetDesc(); 
		Node_Type nt = pDesc->GetType();
		UINT nRows = pLeafNode->GetRows();
		node.append_attribute(_T("Rows"),(long)nRows);
		for(UINT i=0;i<nRows;i++)
		{
			pug::xml_node row = node.append_child(pug::node_element,_T("Row"));
			for(UINT j=0;j<pDesc->GetFieldCount();j++)
			{
				const LeafField * f = pDesc->GetField(j);
				if(f->m_nTemplateOffset>=0)
				{
					pug::xml_node field = row.append_child(pug::node_element,_T("Field"));
					const TCHAR *lpszValue = pLeafNode->GetString(i,j);
					if(lpszValue)
					{
						field.set_child_value(pug::node_pcdata,lpszValue);  
					}
					else
					{
						field.set_child_value(pug::node_pcdata,_T(""));  
					}
					/////////////////////20080625 公式依赖问题//////////////////////////
					if(nt == Node_UnitAttrib)
					{
						cAttribLeafNode *pAttrib = (cAttribLeafNode*)pLeafNode;
						if(j==LeafNode_IsDeriveField-1)
						{
							if(pAttrib->IsDeriveField(i))
							{
								field.set_child_value(pug::node_pcdata,_T("1"));
							}
							else
							{
								field.set_child_value(pug::node_pcdata,_T("0"));
							}
						}
						else if(j==LeafNode_RefField-1)
						{
							if(pAttrib->IsDeriveField(i))
							{
								std::string strTmp = pAttrib->GetDependField(i);
								field.set_child_value(pug::node_pcdata,strTmp.c_str());
							}
							else
							{
								field.set_child_value(pug::node_pcdata,_T(""));
							}
						}
					}
					/////////////////////20080625 公式依赖问题//////////////////////////
				}
				//const LeafField * f = pDesc->GetField(j);
				//if(f->m_nTemplateOffset>=0)
				//{
				//	pug::xml_node field = row.append_child(pug::node_element,_T("Field"));
				//	const TCHAR *lpszValue = pLeafNode->GetString(i,j);
				//	if(lpszValue)
				//	{
				//		field.set_child_value(pug::node_pcdata,lpszValue);  
				//	}
				//	else
				//	{
				//		field.set_child_value(pug::node_pcdata,_T(""));  
				//	}
				//}
			}

			// 特例
			if( ((pDesc->GetType()==Node_UnitAttrib)&&(pLeafNode->GetInteger(i,LeafNode_Type-1)==Field_Enumerate)) 
				|| ((pDesc->GetType()==Node_ContactAttrib)&&(pLeafNode->GetInteger(i,ContactData_Type-1)==Field_Enumerate)))
			{
				UINT nEnum = 0;
				const TCHAR * lpszEnum = pLeafNode->GetEnumString(i,nEnum);
				
				pug::xml_node enum_node = row.append_child(pug::node_element,_T("Enum"));
				enum_node.append_attribute(_T("Count"),(long)nEnum);
				for(UINT n=0;n<nEnum;n++)
				{
					pug::xml_node enum_Item = enum_node.append_child(pug::node_element,_T("Item"));
					enum_Item.set_child_value(pug::node_pcdata,lpszEnum + n*MAX_NAME);  
				}
			}
			else if( ((pDesc->GetType()==Node_UnitAttrib)&&(pLeafNode->GetInteger(i,LeafNode_Type-1)==Field_IdField))
				|| ((pDesc->GetType()==Node_ContactAttrib)&&(pLeafNode->GetInteger(i,ContactData_Type-1)==Field_IdField)))
			{
				FieldId f = {0};							
				pLeafNode->GetFieldID(i,&f);
				
				pug::xml_node idfield_node = row.append_child(pug::node_element,_T("ID_Field"));
				idfield_node.append_attribute(_T("Field"),(long)f.nDispField);
				idfield_node.append_attribute(_T("Value"),(long)f.nValField);
				idfield_node.append_attribute(_T("Path"),f.pLeafPath);
			}
		}	
	}
}
cBaseNode * cWorldImp::LoadFromFile(const TCHAR * lpszFile,cBaseNode * pParent)	// 从文件装载
{
	m_lstInstPool.Create(eBlockSize,false);

	
	cBaseNode * pNode = NULL;
	const TCHAR * lpszExt = PathFindExtension(lpszFile);
	if(!_tcsicmp(lpszExt,_T(".xml")))
	{
		// 打开文件
		std::locale loc = std::locale::global(std::locale(""));
		std::ifstream is(lpszFile, std::ios::in | std::ios::binary);
		std::locale::global(std::locale(loc )); 
		if (is)
		{
			// 把文件读入内存
			is.seekg(0, std::ios::end);
			int size = (int)is.tellg();
			TCHAR *bits = new char[size + 1];
			is.seekg(0, std::ios::beg);
			is.read(bits, size);
			bits[size] = 0;

			// 解析 xml 文件
			pug::xml_parser xml;
			if(xml.parse_file_in_mem(bits))
			{
				pug::xml_node root = xml.document();
				pug::xml_node node = root.first_element_by_name(_T("Node"));
				if(node.empty())
					return NULL;
				pNode = LoadFromXmlNode(&node,pParent);
				BuildUnits(); 
				
			}
			delete []bits;
		}
	}
	else
	{
		FILE * fp = fopen(lpszFile,_T("rb"));
		if(fp)
		{
			LogicHeader h;
			fread(&h,sizeof(LogicHeader),1,fp);
			if( (h.dwFlag == LOGIC_FLAG) && (h.nVersion == LOGIC_VER) )
			{
				pNode = LoadFromBinFile(fp,pParent);
				if(pNode)
					BuildUnits(); 
			}
			fclose(fp);
		}		
	}
	CreateGlobalMemPool();
	return pNode;
}
BOOL cWorldImp::SaveToFile(const TCHAR * lpszFile,cBaseNode * pNode,BOOL bBin)	// 保存到文件
{
	const TCHAR * lpszExt = PathFindExtension(lpszFile);
	if(!_tcsicmp(lpszExt,_T(".xml")))
	{
		std::locale loc = std::locale::global(std::locale(""));
		std::ofstream os(lpszFile, std::ios::trunc);
		std::locale::global(std::locale(loc )); 
		if (os)	
		{
			pug::xml_parser xml;
			xml.create();
			pug::xml_node root = xml.document();
			pug::xml_node pi = root.append_child(pug::node_pi,_T("xml"));
			pi.attribute(_T("version")) = _T("1.0");

			SaveToXmlNode(&root,pNode);

			os << root;

			return TRUE;
		}	
	}
	else
	{
		FILE * fp = fopen(lpszFile,_T("wb"));
		if(fp)
		{
			LogicHeader h = {LOGIC_FLAG,LOGIC_VER};
			fwrite(&h,sizeof(LogicHeader),1,fp);
			SaveToBinFile(fp,pNode);
			fclose(fp);
			return TRUE;
		}
	}
	return FALSE;
}

cBaseNode * cWorldImp::CreateWorld()
{
	if(m_pWorldRoot)
		delete m_pWorldRoot;
	m_pWorldRoot = new cTreeNode_Imp(g_NodeTable[Node_Root].pNodeDesc,g_NodeTable[Node_Root].lpszNodeName,g_NodeTable[Node_Root].lpszNodeAlias,NULL);
	m_lastmemdump=GetTickCount();
	return m_pWorldRoot;
}
void cWorldImp::DestroyWorld(cBaseNode ** ppRoot)
{
	if(m_mapInstNodes.size()>0)
	{
		_error("Logic engine: %d not release!\n",m_mapInstNodes.size());
		IterBlockMap i = m_mapInstNodes.begin(); 
		while(i != m_mapInstNodes.end())
		{
			cInstNode_Imp * pInstNode = (cInstNode_Imp *)i->second; 
			_error("\tUnitID = %d,Template = %d\n",pInstNode->GetUnitID(),pInstNode->GetTemplateID());
			i++;
		}
	}

	DestroyAllInstance();
	
	delete m_pWorldRoot;
	m_pWorldRoot = NULL;
	if(ppRoot)
		*ppRoot = NULL;

	if(m_ppUnits && m_nUnits)
		delete []m_ppUnits;
	m_ppUnits = NULL;
	m_nUnits = 0;
	m_mapUnitIds.clear();
	m_mapUnitNames.clear(); 
	m_mapInstMaxId.clear();		// 每种类型的实例

	if(m_hFormulaMG)
	{
		FreeLibrary(m_hFormulaMG); 
		m_hFormulaMG = NULL;
		m_pFormulaManager = NULL;
	}

	m_lstInstPool.Destroy();
	DestroyGlobalMemPool();
}

void cWorldImp::Update(DWORD dwCurTime)
{
	m_dwTime = dwCurTime;
	
	if(m_dwTime-m_lastmemdump>60*1000)
	{
		_CrtMemState s1;
		_CrtMemCheckpoint(&s1);
		OutputDebugString("==========Logicengine memory dump!\n");
		_CrtMemDumpStatistics(&s1);
		m_lastmemdump=dwCurTime;
		m_pFormulaManager->DumpMemory();

	}


	cInstNode_Imp * pInstNode = m_lstInstPool.First();
	while (pInstNode)
	{
		pInstNode->Update(dwCurTime); 
		pInstNode = m_lstInstPool.Next();
	}
}

// todo:"世界定义/单元管理" 和 "世界定义/单元数据" 是两个特殊的节点,应该作为成员变量被保留
void cWorldImp::RefreshUnitTree()
{
	// 1.用单元管理去刷新单元数据树
	cLeafNode * pUnitMgrNode = (cLeafNode *)m_pWorldRoot->GetNodeByPath(PATH_UNIT_MANAGER); 
	cTreeNode * pUnitMgrTree = (cTreeNode *)m_pWorldRoot->GetNodeByPath(PATH_UNIT_TREE); 
	UINT nClasses = pUnitMgrNode->GetRows();
	UINT nChildren = pUnitMgrTree->GetChildren(); 
	
	bool * pbShouldAdd = NULL;
	bool * pbShouldDel = NULL;
	cBaseNode ** ppShouldDel = NULL;
	if(nClasses)
	{
		pbShouldAdd = new bool[nClasses];		// 单元管理器有，单元树中没有，需要添加
		ZeroMemory(pbShouldAdd,sizeof(bool)*nClasses);
	}
	if(nChildren)
	{
		pbShouldDel = new bool[nChildren];		// 单元树中有，单元管理器没有，需要删除
		ppShouldDel = new cBaseNode *[nChildren];
		FillMemory(pbShouldDel,sizeof(bool)*nChildren,1);
		ZeroMemory(ppShouldDel,sizeof(cBaseNode *)*nChildren);
	}
	for(UINT n=0;n<nClasses;n++)
	{
		const TCHAR * lpszClassName = pUnitMgrNode->GetString(n,_T(UNIT_NAME_COL)); 
		
		bool bFound = false;
		for(UINT m=0;m<nChildren;m++)
		{
			cUnitTreeNode *pTreeNode = (cUnitTreeNode *)pUnitMgrTree->GetChildNode(m);
			const TCHAR * lpszChildName = pTreeNode->GetName();
			if(!lstrcmp(lpszClassName,lpszChildName))
			{
				// 新增单元后，需要重新定向内存
				UnitDef * pDef = (UnitDef *)pUnitMgrNode->GetRow(n)->GetBuffer();
				pTreeNode->SetUnitDef(pDef);
				pbShouldDel[m] = false;	
				bFound = true;
				break;
			}
		}
		if(!bFound)
			pbShouldAdd[n] = true;
	}

	// 删除需要删除的单元数据
	for(UINT m=0;m<nChildren;m++)
	{
		if(pbShouldDel[m])
			ppShouldDel[m] = pUnitMgrTree->GetChildNode(m); 
	}
	for(UINT m=0;m<nChildren;m++)
	{
		if(pbShouldDel[m])
			pUnitMgrTree->DelChildNode(ppShouldDel[m]); 
	}
	
	// 添加需要添加的单元数据
	for(UINT n=0;n<nClasses;n++)
	{
		if(pbShouldAdd[n])
		{
			pUnitMgrTree->NewChildNode(Node_Unit,
				(TCHAR *)pUnitMgrNode->GetString(n,UNIT_NAME_COL),
				(TCHAR *)pUnitMgrNode->GetString(n,UNIT_ALIAS_COL)
				);
		}
		else
		{
			// 不需要添加，但是有可能是需要刷新的。
			const TCHAR * lpszClassName = pUnitMgrNode->GetString(n,_T(UNIT_ALIAS_COL)); 
			TCHAR szPath[256];
			sprintf(szPath,PATH_UNIT,lpszClassName);
			cUnitTreeNode * pUnitNode = (cUnitTreeNode*)pUnitMgrTree->GetNodeByPath(szPath); 
			pUnitNode->RefreshUnitTreeNode(); 
		}
	}

	delete []pbShouldAdd;
	delete []pbShouldDel;


	((cUnitMgrTreeNode*)pUnitMgrTree)->SortChildren();
}

// todo:"世界定义/联系管理" 和 "世界定义/单元联系" 是两个特殊的节点,应该作为成员变量被保留
void cWorldImp::RefreshContactTree()
{
	// 1.用联系管理去刷新单元联系树
	cLeafNode * pContactMgrNode = (cLeafNode *)m_pWorldRoot->GetNodeByPath(CONTACT_MANAGER); 
	cTreeNode * pContactMgrTree = (cTreeNode *)m_pWorldRoot->GetNodeByPath(CONTACT_DATA); 
	UINT nClasses = pContactMgrNode->GetRows();
	UINT nChildren = pContactMgrTree->GetChildren(); 

	bool * pbShouldAdd = NULL;
	bool * pbShouldDel = NULL;
	cBaseNode ** ppShouldDel = NULL;
	if(nClasses)
	{
		pbShouldAdd = new bool[nClasses];		// 单元管理器有，单元树中没有，需要添加
		ZeroMemory(pbShouldAdd,sizeof(bool)*nClasses);
	}
	if(nChildren)
	{
		pbShouldDel = new bool[nChildren];		// 单元树中有，单元管理器没有，需要删除
		ppShouldDel = new cBaseNode *[nChildren];
		FillMemory(pbShouldDel,sizeof(bool)*nChildren,1);
		ZeroMemory(ppShouldDel,sizeof(cBaseNode *)*nChildren);
	}
	for(UINT n=0;n<nClasses;n++)
	{
		const TCHAR * lpszContactName = pContactMgrNode->GetString(n,CONTACT_NAME_COL); 

		bool bFound = false;
		for(UINT m=0;m<nChildren;m++)
		{
			const TCHAR * lpszChildName = pContactMgrTree->GetChildNode(m)->GetName();
			if(!lstrcmp(lpszContactName,lpszChildName))
			{
				pbShouldDel[m] = false;	
				bFound = true;
				break;
			}
		}
		if(!bFound)
			pbShouldAdd[n] = true;
	}

	// 删除需要删除的单元数据
	for(UINT m=0;m<nChildren;m++)
	{
		if(pbShouldDel[m])
			ppShouldDel[m] = pContactMgrTree->GetChildNode(m); 
	}
	for(UINT m=0;m<nChildren;m++)
	{
		if(pbShouldDel[m])
			pContactMgrTree->DelChildNode(ppShouldDel[m]); 
	}

	// 添加需要添加的单元数据
	for(UINT n=0;n<nClasses;n++)
	{
		if(pbShouldAdd[n])
		{
			pContactMgrTree->NewChildNode(Node_Contact,
				(TCHAR *)pContactMgrNode->GetString(n,CONTACT_NAME_COL),
				(TCHAR *)pContactMgrNode->GetString(n,CONTACT_ALIAS_COL));
		}
		else
		{
			// 不需要添加，但是有可能是需要刷新的。
			const TCHAR * lpszClassName = pContactMgrNode->GetString(n,CONTACT_ALIAS_COL); 
			TCHAR szPath[256];
			sprintf(szPath,PATH_CONTACT,lpszClassName);
			cContactTreeNode * pContactNode = (cContactTreeNode*)pContactMgrTree->GetNodeByPath(szPath); 
			pContactNode->RefreshContactTreeNode(); 
		}
	}

	delete []pbShouldAdd;
	delete []pbShouldDel;
}

void cWorldImp::RefreshTemplate(cBaseNode * pAttrib)
{
	if(pAttrib->GetDesc()->GetType()==Node_UnitAttrib)
	{
		// 寻找自己兄弟节点中的数据模板.
		cTreeNode * pParent = (cTreeNode*)pAttrib->GetParent();
		for(UINT n=0;n<pParent->GetChildren();n++)
		{
			cBaseNode * pNode = pParent->GetChildNode(n);
			if(pNode->GetDesc()->GetType() == Node_UnitTemplate)
			{
				((cLeafNode_Imp *)pNode)->OnAttribDefChanged();
				return;
			}
		}
		
		// 在单元管理中寻找自己的派生类,刷新派生类.
		if(pParent->GetDesc()->GetType() == Node_Unit)
		{
			cUnitTreeNode * pUnit = (cUnitTreeNode *)pParent;
			if(pUnit->GetUnitDef()->m_eType == Unit_Base)
			{
				UINT nUnitID = pUnit->GetUnitDef()->m_nID;
				for(UINT n=0;n<m_nUnits;n++)
				{
					if(m_ppUnits[n]->GetUnitDef()->m_nBaseClass == nUnitID)
					{
						cLeafNode * pUnitChild = GetUnitAttrib(m_ppUnits[n]->GetUnitDef()->m_nID);
						RefreshTemplate(pUnitChild);
					}
				}
			}
		}

		// 要刷新自己对数据模板的描述.
		cTemplateLeafNodeDesc * pTemplateDesc = (cTemplateLeafNodeDesc*)((cAttribLeafNode*)pAttrib)->GetTargetDesc();
		pTemplateDesc->Build(); 
	}
}

void cWorldImp::RefreshContact(cBaseNode * pAttrib)
{
	if(pAttrib->GetDesc()->GetType()==Node_ContactAttrib)
	{
		// 寻找自己兄弟节点中的数据模板.
		cTreeNode * pParent = (cTreeNode*)pAttrib->GetParent();
		for(UINT n=0;n<pParent->GetChildren();n++)
		{
			cBaseNode * pNode = pParent->GetChildNode(n);
			if(pNode->GetDesc()->GetType() == Node_ContactData)
			{
				((cLeafNode_Imp *)pNode)->OnAttribDefChanged();
				return;
			}
		}

		// 要刷新自己对数据模板的描述.
		cContactDataLeafNodeDesc * pContactDesc = (cContactDataLeafNodeDesc*)((cContactAttribLeafNode*)pAttrib)->GetTargetDesc();
		pContactDesc->Build(); 
	}
}

cLeafNode * cWorldImp::GetUnitTemplate(int nID)
{
	cLeafNode * pUnitMgr = GetUnitManager();
	if(pUnitMgr)
	{
		UINT nRow = pUnitMgr->FindRowByID(nID);
		if(nRow != (UINT)-1)
		{
			const TCHAR * lpszName = pUnitMgr->GetString(nRow,_T(UNIT_ALIAS_COL)); 
			return GetUnitTemplate(lpszName);
		}
	}
	return NULL;
}

cLeafNode * cWorldImp::GetUnitTemplate(const TCHAR * lpszUnit)
{
	TCHAR lpszPath[MAX_PATH];
	wsprintf(lpszPath,PATH_UNIT_TEMPLATE,lpszUnit);
	return (cLeafNode *)m_pWorldRoot->GetNodeByPath(lpszPath);
}

cLeafNode * cWorldImp::GetUnitBuffer(int nID)
{
	cLeafNode * pUnitMgr = GetUnitManager();
	if(pUnitMgr)
	{
		UINT nRow = pUnitMgr->FindRowByID(nID);
		if(nRow != (UINT)-1)
		{
			const TCHAR * lpszName = pUnitMgr->GetString(nRow,_T(UNIT_ALIAS_COL)); 
			return GetUnitBuffer(lpszName);
		}
	}
	return NULL;
}
cLeafNode * cWorldImp::GetUnitBuffer(const TCHAR * lpszUnit)
{
	TCHAR lpszPath[MAX_PATH];
	wsprintf(lpszPath,PATH_BUFF_EFFECT,lpszUnit);
	return (cLeafNode *)m_pWorldRoot->GetNodeByPath(lpszPath);
}
cLeafNode * cWorldImp::GetContactTemplate(int nID)
{
	cLeafNode * pContactMgr = GetContactManager();
	if(pContactMgr)
	{
		UINT nRow = pContactMgr->FindRowByID(nID);
		if(nRow != (UINT)-1)
		{
			const TCHAR * lpszName = pContactMgr->GetString(nRow,CONTACT_ALIAS_COL); 
			return GetContactTemplate(lpszName);
		}
	}
	return NULL;
}
cLeafNode * cWorldImp::GetContactTemplate(const TCHAR * lpszUnit)
{
	TCHAR lpszPath[MAX_PATH];
	wsprintf(lpszPath,PATH_CONTACT_DATA,lpszUnit);
	return (cLeafNode *)m_pWorldRoot->GetNodeByPath(lpszPath);
}
cLeafNode * cWorldImp::GetUnitAttrib(int nID)
{
	cLeafNode * pUnitMgr = GetUnitManager();		// 根据 ID 取得名字
	if(pUnitMgr)
	{
		UINT nRow = pUnitMgr->FindRowByID(nID);
		if(nRow != (UINT)-1)
		{
			const TCHAR * lpszName = pUnitMgr->GetString(nRow,_T(UNIT_ALIAS_COL)); 
			return GetUnitAttrib(lpszName);
		}
	}
	return NULL;
}
cLeafNode * cWorldImp::GetUnitAttrib(const TCHAR * lpszUnit)
{
	TCHAR lpszPath[MAX_PATH];
	wsprintf(lpszPath,PATH_UNIT_ATTRIB,lpszUnit);
	return (cLeafNode *)m_pWorldRoot->GetNodeByPath(lpszPath);
}

cLeafNode * cWorldImp::GetContactAttrib(int nID)
{
	cLeafNode * pContactMgr = GetContactManager();		// 根据 ID 取得名字
	if(pContactMgr)
	{
		UINT nRow = pContactMgr->FindRowByID(nID);
		if(nRow != (UINT)-1)
		{
			const TCHAR * lpszName = pContactMgr->GetString(nRow,CONTACT_ALIAS_COL); 
			return GetContactAttrib(lpszName);
		}
	}
	return NULL;
}
cLeafNode * cWorldImp::GetContactAttrib(const TCHAR * lpszUnit)
{
	TCHAR lpszPath[MAX_PATH];
	wsprintf(lpszPath,PATH_CONTACT_ATTRIB,lpszUnit);
	return (cLeafNode *)m_pWorldRoot->GetNodeByPath(lpszPath);
}
cLeafNode * cWorldImp::GetUnitManager()
{
	return (cLeafNode *)FindFirstNode(m_pWorldRoot,Node_UnitManager);
}
cLeafNode * cWorldImp::GetContactManager()
{
	return (cLeafNode *)FindFirstNode(m_pWorldRoot,Node_ContactManager);
}
cLeafNode * cWorldImp::GetRelation()
{
	return (cLeafNode *)FindFirstNode(m_pWorldRoot,Node_Relation);
}

int cWorldImp::EnumLeafNode(cLeafNode ** ppNode)
{
	int nCount = 0;
	EnumLeafNode(ppNode,nCount,m_pWorldRoot);
	return nCount;
}

void cWorldImp::EnumLeafNode(cLeafNode ** ppNode,int& nCount,cBaseNode * pNode)
{
	if(!pNode->GetDesc()->IsLeaf())
	{
		cTreeNode * pTreeNode = (cTreeNode *)pNode;
		for(UINT i=0;i<pTreeNode->GetChildren();i++)
		{
			cBaseNode * pChild = pTreeNode->GetChildNode(i);
			if(pChild->GetDesc()->IsLeaf())
			{
				cLeafNodeDesc * pDesc = (cLeafNodeDesc *)pChild->GetDesc();
				if(pDesc->GetFieldCount()&& !lstrcmpi(pDesc->GetField(0)->m_lpszName,_T("ID")))
				{
					if(ppNode)
						ppNode[nCount] = (cLeafNode *)pChild;
					nCount ++;
				}
			}
			else
				EnumLeafNode(ppNode,nCount,pChild); 
		}
	}
}

void cWorldImp::BuildUnits()
{
	cTreeNode * pUnitMgrTree = (cTreeNode *)m_pWorldRoot->GetNodeByPath(PATH_UNIT_TREE); 
	((cUnitMgrTreeNode*)pUnitMgrTree)->SortChildren();
	
	cLeafNode * pUnitMgr = GetUnitManager();	// 根据 ID 取得名字
	if(pUnitMgr)
	{
		TCHAR lpszPath[MAX_PATH];
		
		if(m_ppUnits && m_nUnits)
		{
			delete []m_ppUnits;
			m_mapUnitIds.clear();
			m_mapUnitNames.clear();
		}
		
		m_nUnits = pUnitMgr->GetRows();
		if(m_nUnits)
		{
			m_ppUnits = new cUnit*[m_nUnits];
			for(UINT n=0;n<m_nUnits;n++)
			{
				UnitDef * pUnitDef = (UnitDef*)pUnitMgr->GetRow(n)->GetBuffer();
				wsprintf(lpszPath,PATH_UNIT2,pUnitDef->m_szAlias);
				m_ppUnits[n] = (cUnitTreeNode*)m_pWorldRoot->GetNodeByPath(lpszPath);

				m_mapUnitIds[pUnitDef->m_nID] = m_ppUnits[n];
				m_mapUnitNames[t_string(pUnitDef->m_szName)] = m_ppUnits[n];

				if(pUnitDef->m_eType == Unit_Unit)
				{
					if (m_ppUnits[n])
					{
						cLeafNode_Imp * pTemplate = (cLeafNode_Imp *)m_ppUnits[n]->GetLeaf(Node_UnitTemplate);
						if(pTemplate)
							pTemplate->BuildRelation();
					}
				}

			}
		}
	}
}

cBaseNode * cWorldImp::FindFirstNode(cBaseNode * pNode,Node_Type eType)
{
	if(!pNode->GetDesc()->IsLeaf())
	{
		cTreeNode * pTreeNode = (cTreeNode *)pNode;
		for(UINT n=0;n<pTreeNode->GetChildren();n++)
		{
			cBaseNode * pChild = pTreeNode->GetChildNode(n);
			if(pChild->GetDesc()->GetType() == eType)
				return pChild;
			
			if(!pChild->GetDesc()->IsLeaf())
			{
				pChild = FindFirstNode(pChild,eType);
				if(pChild)
					return pChild;
			}
		}
	}
	if(pNode->GetDesc()->GetType() == eType)
		return pNode;
	return NULL;
}
const cUnit * cWorldImp::GetUnit(UINT n) const
{
	if(n < m_nUnits)
		return m_ppUnits[n];
	return NULL;
}
const cUnit * cWorldImp::GetUnit(int nID) const						// 用单元ID取得单元
{
	std::map<int,cUnit*>::const_iterator i = m_mapUnitIds.find(nID); 
	if(i != m_mapUnitIds.end())
		return i->second; 
	return NULL;
}
const cUnit * cWorldImp::GetUnit(const TCHAR * lpszUnit) const			// 用单元ID取得单元
{
	std::map<t_string,cUnit*>::const_iterator i = m_mapUnitNames.find(lpszUnit); 
	if(i != m_mapUnitNames.end())
		return i->second; 
	return NULL;
}
BOOL cWorldImp::IfDeriveFrom(const cUnit * pSrc,const cUnit * pDst)const		// 判断一个单元是否继承自另外一个单元(可能是间接继承)
{
	if(pSrc && pDst)
	{
		UINT nDstID = pDst->GetUnitDef()->m_nID;
		const cUnit * pCurUnit = pSrc;
		UINT nUnitID = pCurUnit->GetUnitDef()->m_nBaseClass;
		while (nUnitID > 0)
		{
			
			if( nDstID == nUnitID)
				return TRUE;
			pCurUnit = GetUnit((int)nUnitID);
			nUnitID = pCurUnit->GetUnitDef()->m_nBaseClass;
		}
	}
	return FALSE;
}
cInstNode * cWorldImp::CreateInstance(const cUnit * pUnit,int nTemplateID,int nInstanceID)		// 实例ID
{
	// 未指定实例ID,需要自行生成
	int nInstID = 0;
	int nClassID = pUnit->GetUnitDef()->m_nID;

	//GetSysEfficiency()->beginRecord("寻找实例");
	Const_IterBlockMap i = m_mapInstNodes.find(nInstanceID); 
	//GetSysEfficiency()->endRecord("寻找实例");
	if(i != m_mapInstNodes.end())
		return i->second; 

	//_printf("create instance current number of instnace =%d\n",m_mapInstNodes.size());
	cInstNode_Imp * pInstNode = NULL;
	//GetSysEfficiency()->beginRecord("分配实例");
	unsigned int nId = XLIST_NEW(m_lstInstPool,&pInstNode);
	//GetSysEfficiency()->endRecord("分配实例");
	if (nId > 0)
	{
		if (nId > 0xffffff)
		{
			return NULL;
		}
		//GetSysEfficiency()->beginRecord("初始化实例");
		assert(nId < 0xffffff && _T("Inst id overflow"));
		nInstanceID = (nClassID<<24)&0xff000000|nId&0xffffff;
		if(!pInstNode->Init(pUnit,nTemplateID,nInstanceID,m_pFormulaManager))
		{
			m_lstInstPool.Del(pInstNode);
			return NULL;
		}
		//GetSysEfficiency()->endRecord("初始化实例");
	}
	else
		return NULL;

	// 加入管理器
	//GetSysEfficiency()->beginRecord("加入管理器");
	m_mapInstNodes[nInstanceID] = pInstNode;
	//GetSysEfficiency()->endRecord("加入管理器");
	//_printf("Create instance %d unit =%s template =%d total instance %d\n",nInstanceID,pUnit->GetUnitDef()->m_szName,nTemplateID,m_mapInstNodes.size());

#ifdef _DEBUG
	//printf("创建实例后当前世界实例数[%d]\n",m_mapInstNodes.size());
#endif
	return pInstNode;
}

cInstNode * cWorldImp::GetInstance(int nID)const
{
#ifndef DEFINE_BLOCK_ALLOCATED_MAP
	std::map<int,cInstNode*>::const_iterator i = m_mapInstNodes.find(nID); 
#else
	Const_IterBlockMap i = m_mapInstNodes.find(nID); 
#endif
	if(i != m_mapInstNodes.end())
		return i->second; 
	return NULL;
}
void cWorldImp::DestroyInstance(int nID)
{
#ifndef DEFINE_BLOCK_ALLOCATED_MAP
	std::map<int,cInstNode*>::iterator i = m_mapInstNodes.find(nID); 
#else
	IterBlockMap i = m_mapInstNodes.find(nID);
#endif
	if(i != m_mapInstNodes.end())
	{
		cInstNode_Imp * pInstNode = (cInstNode_Imp *)i->second;
#if 0
		delete pInstNode;
#else
		pInstNode->Destroy();
		m_lstInstPool.Del(pInstNode);
#endif
		m_mapInstNodes.erase(i);
		//_printf("Destory instance %d left size =%d\n",nID,m_mapInstNodes.size());
	}else
	{
		_error("destory instance id=%d not found!\n",nID);
		assert(0);
	}
}
void cWorldImp::DestroyAllInstance()
{
#ifndef DEFINE_BLOCK_ALLOCATED_MAP
	std::map<int,cInstNode*>::iterator i = m_mapInstNodes.begin(); 
#else
	IterBlockMap i = m_mapInstNodes.begin(); 
#endif
	while(i != m_mapInstNodes.end())
	{
#ifndef DEFINE_BLOCK_ALLOCATED_MAP
		std::map<int,cInstNode*>::iterator _iTemp = i;
#else
		IterBlockMap _iTemp = i;
#endif
		//_printf("destroy instance %d\n",i->second->GetID());
		i++;
		cInstNode_Imp * pInstNode = (cInstNode_Imp *)_iTemp->second; 
#if 0
		delete pInstNode;
#else
		pInstNode->Destroy();
		m_lstInstPool.Del(pInstNode);
#endif
		m_mapInstNodes.erase(_iTemp); 
	}
	m_mapInstNodes.clear(); 
}

int cWorldImp::GetNextInstHandle(int nID)const				// 取得实例句柄。hHandle == NULL为第一个
{
#ifndef DEFINE_BLOCK_ALLOCATED_MAP
	std::map<int,cInstNode*>::const_iterator i;
#else
	Const_IterBlockMap i;
#endif
	if(nID)
	{
		i = m_mapInstNodes.find(nID);
		i++;
		if( i != m_mapInstNodes.end() ) 
			return i->first; 
		return 0;
	}
	else
	{
		i = m_mapInstNodes.begin();
		return m_mapInstNodes.size()?i->first:0; 
	}
	return 0;
}

const TCHAR * lpszBegin = {_T(
"/////////////////////////////////////////////////////////////////////////////\n\
// 此代码由逻辑编辑器生成,请不要手工编辑这部分代码\n\
//	生成日期:%04d-%02d-%02d %02d:%02d:%02d\n\
/////////////////////////////////////////////////////////////////////////////\n\
#pragma once\n\
#pragma pack(1)\n\
namespace Logic\n\
{\n")};

const TCHAR * lpszEnd = {_T(
"}\n\
#pragma pack()\n\
// 代码结束\n")};

const TCHAR * lpszPrefix[] = {
	_T("m_sz"),	//Field_String,
	_T("m_e"),	//Field_Enumerate,
	_T("m_n"),	//Field_Integer,
	_T("m_b"),	//Field_Boolean,
	_T("m_n"),	//Field_Percent,
	_T("m_f"),	//Field_Float,
	_T("m_n"),	//Field_IdField,
};
const TCHAR * lpszDataType[] = {
	_T("TCHAR"),//Field_String,
	_T(""),		//Field_Enumerate,
	_T("int"),	//Field_Integer,
	_T("BOOL"),	//Field_Boolean,
	_T("int"),	//Field_Percent,
	_T("float"),//Field_Float,
	_T("int"),	//Field_IdField,
};

int __cdecl UnitCompare( const void *u1, const void *u2 )
{
	const UnitDef * d1 =  (*(const cUnit**)u1)->GetUnitDef();
	const UnitDef * d2 =  (*(const cUnit**)u2)->GetUnitDef();
	if(d1->m_nBaseClass == d2->m_nBaseClass)
		return d1->m_nID - d2->m_nID;
	return d1->m_nBaseClass - d2->m_nBaseClass;
}


BOOL cWorldImp::ExportSrcHeader(const TCHAR * lpszPath)			// 用实例ID取得实例指针
{
	FILE * fp = _tfopen(lpszPath,"w");
	if(!fp)
		return FALSE;

	SYSTEMTIME time;
	GetLocalTime(&time);
	_ftprintf(fp,lpszBegin,time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
	
	
	// 保证基类单元排在前面.
	cUnit ** ppExportUnits = new cUnit *[m_nUnits];
	CopyMemory(ppExportUnits,m_ppUnits,sizeof(cUnit *)*m_nUnits);
	qsort(ppExportUnits,m_nUnits,sizeof(cUnit *),UnitCompare);	
	
	// 单元ID枚举
	_ftprintf(fp,_T("\t// 单元ID枚举\n"));	
	_ftprintf(fp,_T("\tenum Unit_IDs {\n"));
	for(UINT n=0;n<m_nUnits;n++)
	{
		const UnitDef * pUnitDef = ppExportUnits[n]->GetUnitDef();
		_ftprintf(fp,_T("\t\tUnit_%s = %d,\t//%s\n"),
			pUnitDef->m_szAlias,pUnitDef->m_nID,pUnitDef->m_szName);
	}
	_ftprintf(fp,_T("\t};\n"));
	
	
	for(UINT n=0;n<m_nUnits;n++)
	{
		const UnitDef * pUnitDef = ppExportUnits[n]->GetUnitDef();
		const UnitDef * pBaseUnitDef = pUnitDef->m_nBaseClass?GetUnit(pUnitDef->m_nBaseClass)->GetUnitDef():NULL;
		const cTemplateLeafNodeDesc * pDesc = (cTemplateLeafNodeDesc*)ppExportUnits[n]->GetAttribDesc();
		
		// 单元名注释
		_ftprintf(fp,_T("\t//////////////////////////////////////////////////////////////\n"));
		_ftprintf(fp,_T("\t// 单元 %s ID = %d\n"),pUnitDef->m_szName,pUnitDef->m_nID);	
		_ftprintf(fp,_T("\t//////////////////////////////////////////////////////////////\n"));

		// 属性
		UINT nBaseFields = pDesc->GetBaseFields();
		UINT nFields = pDesc->GetFieldCount();

		// 属性域枚举
		_ftprintf(fp,_T("\t// 属性域枚举\n"));	
		_ftprintf(fp,_T("\tenum %s_Fields {\n"),pUnitDef->m_szAlias);
		for(UINT i=nBaseFields;i<nFields;i++)
		{
			const LeafField * field = pDesc->GetField(i); 
			_ftprintf(fp,_T("\t\t%s_%s = %d,\t//%s\n"),pUnitDef->m_szAlias,field->m_lpszAlias,i,field->m_lpszName);
		}
		_ftprintf(fp,_T("\t\t%s_Max = %d\n"),pUnitDef->m_szAlias,nFields);
		_ftprintf(fp,_T("\t};\n"));

		// 字段枚举值
		for(UINT i=nBaseFields;i<nFields;i++)
		{
			const LeafField * field = pDesc->GetField(i); 
			if(field->m_eType == Field_Enumerate)
			{
				_ftprintf(fp,_T("\t// 属性域 %s 枚举值定义\n"),field->m_lpszName);
				_ftprintf(fp,_T("\tenum %s_%s_Enum {\n"),pUnitDef->m_szAlias,field->m_lpszAlias);
				
				UINT nNum = 0;
				const TCHAR * lpszString = pDesc->GetEnumString(i,nNum);
				UINT e;
				for(e=0;e<nNum;e++) 
				{
					_ftprintf(fp,_T("\t\t%s_%s_%d,\t//%s\n"),
						pUnitDef->m_szAlias,
						field->m_lpszAlias,
						e,lpszString + e*MAX_NAME);
				}
				_ftprintf(fp,_T("\t\t%s_%sMax = %d\n"),pUnitDef->m_szAlias,field->m_lpszAlias,e,nNum);
				_ftprintf(fp,_T("\t};\n"));
			}
		}

		// 模板结构
		//if(pUnitDef->m_bHasTemplate) 无论如何 模板结构都是必须的
		{
			_ftprintf(fp,_T("\t// 数据模板结构\n"));	
			if(pUnitDef->m_nBaseClass == 0)
				_ftprintf(fp,_T("\tstruct %s_t {\n"),pUnitDef->m_szAlias);
			else
				_ftprintf(fp,_T("\tstruct %s_t : public %s_t {\n"),pUnitDef->m_szAlias,pBaseUnitDef->m_szAlias); 

			for(UINT i=nBaseFields;i<nFields;i++)
			{
				const LeafField * field = pDesc->GetField(i); 
				if(field->m_nTemplateOffset >= 0)
				{
					TCHAR szDataType[256];
					if(field->m_eType != Field_Enumerate)
						lstrcpy(szDataType,lpszDataType[field->m_eType]);
					else
						_stprintf(szDataType,_T("%s_%s_Enum"),pUnitDef->m_szAlias,field->m_lpszAlias);
					
					if(field->m_eType != Field_String)
					{
						_ftprintf(fp,_T("\t\t%s\t%s%s;\t//%s\n"),
							szDataType,lpszPrefix[field->m_eType],field->m_lpszAlias,field->m_lpszName);
					}
					else
					{
						_ftprintf(fp,_T("\t\t%s\t%s%s[%d];\t//%s\n"),
							szDataType,lpszPrefix[field->m_eType],field->m_lpszAlias,field->m_nLength,field->m_lpszName);
					}
				}
			}
			_ftprintf(fp,_T("\t};\n"));
		}

		// 实例结构
		if(/*pUnitDef->m_eType == Unit_Unit*/true)
		{
			_ftprintf(fp,_T("\t// 数据实例结构\n"));	
			
			// 基类不产生实例字段。单元都只能继承自基类。所以单元实例不需要继承。
			//_ftprintf(fp,_T("\tstruct %s_i {\n"),pUnitDef->m_szAlias);
			// 基类也产生实例字段,单元实例也需要继承,是否可保证不同的派生类中,基类字段实例化都是一致的?
			
			if(pUnitDef->m_nBaseClass == 0)
				_ftprintf(fp,_T("\tstruct %s_i {\n"),pUnitDef->m_szAlias);
			else
				_ftprintf(fp,_T("\tstruct %s_i : public %s_i {\n"),pUnitDef->m_szAlias,pBaseUnitDef->m_szAlias); 


			for(UINT i=nBaseFields;i<nFields;i++)
			{
				const LeafField * field = pDesc->GetField(i); 
				if(field->m_nInstanceOffset >= 0)
				{
					TCHAR szDataType[256];
					if(field->m_eType != Field_Enumerate)
						lstrcpy(szDataType,lpszDataType[field->m_eType]);
					else
						_stprintf(szDataType,_T("%s_%s_Enum"),pUnitDef->m_szAlias,field->m_lpszAlias);
					if(field->m_eType != Field_String)
					{
						_ftprintf(fp,_T("\t\t%s\t%s%s;\t//%s\n"),
							szDataType,lpszPrefix[field->m_eType],field->m_lpszAlias,field->m_lpszName);
					}
					else
					{
						_ftprintf(fp,_T("\t\t%s\t%s%s[%d];\t//%s\n"),
							szDataType,lpszPrefix[field->m_eType],field->m_lpszAlias,field->m_nLength,field->m_lpszName);
					}
				}
			}
			_ftprintf(fp,_T("\t};\n"));


			// 模板偏移量数组
			if(nFields)
			{
				_ftprintf(fp,_T("#ifdef %s_instance \n"),pUnitDef->m_szAlias);
				_ftprintf(fp,_T("\tint n%s_iOffset[%d] = {\n\t\t"),pUnitDef->m_szAlias,nFields);
				for(UINT i=0;i<nFields;i++)
				{
					const LeafField * field = pDesc->GetField(i); 
					if(i<nFields-1)
						_ftprintf(fp,_T("%d,"),field->m_nInstanceOffset);
					else
						_ftprintf(fp,_T("%d"),field->m_nInstanceOffset);
				}
				_ftprintf(fp,_T("\t};\n"),pUnitDef->m_szAlias);
				_ftprintf(fp,_T("#endif //%s_instance \n"),pUnitDef->m_szAlias);
			}
		}
	}	

	// 联系的结构定义
	cLeafNode * pContactMgr = GetContactManager();
	if(pContactMgr)
	{
		UINT n = 0;
		// 联系ID枚举
		_ftprintf(fp,_T("\t// 联系ID枚举\n"));	
		_ftprintf(fp,_T("\tenum Contact_IDs {\n"));
		UINT nRows = pContactMgr->GetRows();
		for(n=0;n<nRows;n++)
		{
			const cLeafRow *pRow = pContactMgr->GetRow(n);
			if (pRow)
			{
				_ftprintf(fp,_T("\t\tContact_%s = %d,\t//%s\n"),
					pRow->GetString(2),pRow->GetInteger((UINT)0),pRow->GetString(1));
			}
		}
		_ftprintf(fp,_T("\t};\n"));


		for (n=0;n<nRows;n++)
		{
			const cLeafRow *pRow = pContactMgr->GetRow(n);
			if (pRow)
			{
				cLeafNode *pLeafNode = GetContactAttrib(pRow->GetInteger((UINT)0));
				if (pLeafNode)
				{
					UINT nCnt = pLeafNode->GetRows();
					// 联系名注释
					_ftprintf(fp,_T("\t//////////////////////////////////////////////////////////////\n"));
					_ftprintf(fp,_T("\t// 联系 %s ID = %d\n"),pRow->GetString(1),pRow->GetInteger((UINT)0));
					_ftprintf(fp,_T("\t//////////////////////////////////////////////////////////////\n"));

					// 属性域枚举
					_ftprintf(fp,_T("\t// 属性域枚举\n"));	
					_ftprintf(fp,_T("\tenum %s_Fields {\n"),pRow->GetString(2));
					for(UINT i=0;i<nCnt;i++)
					{
						_ftprintf(fp,_T("\t\t%s_%s = %d,\t//%s\n"),pRow->GetString(2),pLeafNode->GetString(i,1),i,pLeafNode->GetString(i,(UINT)0));
					}
					_ftprintf(fp,_T("\t\t%s_Max = %d\n"),pRow->GetString(2),nCnt);
					_ftprintf(fp,_T("\t};\n"));


					for(UINT i=0;i<nCnt;i++)
					{
						FieldType eType = (FieldType)pLeafNode->GetInteger(i,2);
						if (eType == Field_Enumerate)
						{
							int nMin = pLeafNode->GetInteger(i,3);
							int nMax = pLeafNode->GetInteger(i,4);
							// 属性域枚举
							_ftprintf(fp,_T("\t// 属性域 %s 枚举值定义\n"),pLeafNode->GetInteger(i,1));	
							_ftprintf(fp,_T("\tenum %s_%s_Enum {\n"),pRow->GetString(2),pLeafNode->GetString(i,1));
							for(int j=nMin;j<nMax;j++)
							{
								_ftprintf(fp,_T("\t\t%s_%s_%d,\t//\n"),pRow->GetString(2),pLeafNode->GetString(i,1),j);
							}
							_ftprintf(fp,_T("\t\t%s_%sMax = %d\n"),pRow->GetString(2),pLeafNode->GetString(i,1),nMax);
							_ftprintf(fp,_T("\t};\n"));
						}
					}


					// 属性结构
					_ftprintf(fp,_T("\t// 联系数据结构\n"));	
					_ftprintf(fp,_T("\tstruct %s_t {\n"),pRow->GetString(2));

					for(UINT i=0;i<nCnt;i++)
					{
						FieldType eType = (FieldType)pLeafNode->GetInteger(i,2);
						TCHAR szDataType[256];
						if(eType != Field_Enumerate)
							lstrcpy(szDataType,lpszDataType[eType]);
						else
							_stprintf(szDataType,_T("%s_%s_Enum"),pRow->GetString(2),pLeafNode->GetString(i,1));

						if(eType != Field_String)
						{
							_ftprintf(fp,_T("\t\t%s\t%s%s;\t//%s\n"),
								szDataType,lpszPrefix[eType],pLeafNode->GetString(i,1),pLeafNode->GetString(i,(UINT)0));
						}
						else
						{
							_ftprintf(fp,_T("\t\t%s\t%s%s[%d];\t//%s\n"),
								szDataType,lpszPrefix[eType],pLeafNode->GetString(i,1),pLeafNode->GetInteger(i,5),pLeafNode->GetString(i,(UINT)0));
						}
					}
					_ftprintf(fp,_T("\t};\n"));

				}
			}
		}
	}

	_ftprintf(fp,lpszEnd);
	fclose(fp);

	delete []ppExportUnits;

	return TRUE;
}

const void * cWorldImp::GetTemplateBuf(int nUnitID,UINT nCol,int nValue)const	// 取得指定单元ID,指定行的缓存
{
	const cUnit * pUnit = GetUnit(nUnitID);
	if(pUnit)
	{
		const cLeafNode * pLeafNode = pUnit->GetLeaf(Node_UnitTemplate);
		if(pLeafNode)
		{
			UINT nRow = pLeafNode->FindRow(nCol,nValue); 
			if(nRow != -1)
				return pLeafNode->GetRow(nRow)->GetBuffer();  
		}
	}
	return NULL;
}

cBaseNode * cWorldImp::ImportFromExcel(cBaseNode * pParent,const TCHAR * lpszPath,const TCHAR * lpszNode,const TCHAR * lpszAlias,const TCHAR * lpszType)
{
	if(lpszNode == NULL)
	{
		lpszNode = g_NodeTable[0].lpszNodeName;
		lpszAlias = g_NodeTable[0].lpszNodeAlias;
		lpszType = g_NodeTable[0].pNodeDesc->GetDescName();
	}
	
	// 寻找节点描述
	cNodeDesc * pDesc = NULL;
	for(int i=0;i<Node_Type_Max;i++)
	{
		if(g_NodeTable[i].pNodeDesc)
		{
			if(!lstrcmpi(g_NodeTable[i].pNodeDesc->GetDescName(),lpszType))
			{
				pDesc = g_NodeTable[i].pNodeDesc;
				break;
			}
		}
	}
	if(!pDesc)
		return NULL;

	// 寻找或者创建子节点
	cBaseNode * pNode = NULL;
	if(pParent)
	{
		cTreeNode * pTreeNode = (cTreeNode *)pParent;
		if(pDesc->IfExclusive())
		{
			for(UINT n=0;n<pTreeNode->GetChildren();n++)
			{
				cBaseNode * pExist = pTreeNode->GetChildNode(n);
				if(pExist->GetDesc()->GetType() == pDesc->GetType())
				{
					pNode = pExist;
					break;
				}
			}
		}
		else
		{
			pNode = pTreeNode->NewChildNode(pDesc->GetType(),(TCHAR*)lpszNode,(TCHAR*)lpszAlias);  
		}
	}
	else
	{
		// 创建世界根节点
		assert(!pDesc->IsLeaf());
		pNode = new cTreeNode_Imp(pDesc,(TCHAR*)lpszNode,(TCHAR*)lpszAlias,NULL);
		m_pWorldRoot = pNode;
	}
	if(!pNode)
		return NULL;

	TCHAR szName[MAX_PATH];
	_tcscpy(szName,lpszPath);
	_tcscat(szName,_T("\\"));
	_tcscat(szName,lpszNode);

	// 树节点
	if(!pDesc->IsLeaf())
	{
		// 检查目录是否存在.
		if(!PathFileExists(szName) || !PathIsDirectory(szName))
			return NULL;

		// 节点类型文件
		TCHAR szTypeFile[MAX_PATH];
		_tcscpy(szTypeFile,szName);
		_tcscat(szTypeFile,_T("\\NodeType.txt"));
		std::locale loc = std::locale::global(std::locale(""));
		std::ifstream is(szTypeFile, std::ios::in | std::ios::binary);
		std::locale::global(std::locale(loc )); 
		if (!is)
			return NULL;
		
		TCHAR szBuf[MAX_PATH];
		is.getline(szBuf,MAX_PATH);
		szBuf[_tcslen(szBuf)-1]= 0;
		int nChildren = _ttoi(szBuf);
		for(int i=0;i<nChildren;i++)
		{
			is.getline(szBuf,MAX_PATH);
			szBuf[_tcslen(szBuf)-1]= 0;
			// 名字和类型用:分割开
			if( szBuf[0])
			{
				TCHAR szNodeName[MAX_PATH]={0},szNodeAlias[MAX_PATH]={0},szNoedType[MAX_PATH]={0};
				_stscanf(szBuf,_T("%s:%s:%s"),szNodeName,szNodeAlias,szNoedType);
				ImportFromExcel(pNode,szName,szNodeName,szNodeAlias,szNoedType);
			}
		}
	}
	// 叶节点
	else
	{
		// 检查文件是否存在.
		_tcscat(szName,_T(".xml"));
		if(!PathFileExists(szName) || PathIsDirectory(szName))
			return NULL;

		// 叶节点
		cLeafNode * pLeafNode = (cLeafNode *)pNode;
		HRESULT hr = pLeafNode->ImportFromExcel(szName);
	}
	return pNode;	
}

BOOL cWorldImp::ExportToExcel(cBaseNode * pNode,const TCHAR * lpszPath)
{
	// 是目录
	TCHAR szName[MAX_PATH];
	_tcscpy(szName,lpszPath);
	_tcscat(szName,_T("\\"));
	_tcscat(szName,pNode->GetName());
	if(!pNode->GetDesc()->IsLeaf())
	{
		// 如果目录不存在,需要创建新目录
		if(!PathFileExists(szName) || !PathIsDirectory(szName))
		{
			if(!CreateDirectory(szName,NULL))
			{
				TCHAR szBuf[MAX_PATH];
				_stprintf(szBuf,_T("无法创建目录 %s"),lpszPath);
				MessageBox(NULL,szBuf,_T("衰吧"),MB_OK);
				return FALSE;
			}
		}

		cTreeNode * pTreeNode = (cTreeNode *)pNode;
		
		TCHAR szTypeFile[MAX_PATH];
		_tcscpy(szTypeFile,szName);
		_tcscat(szTypeFile,_T("\\NodeType.txt"));
		std::locale loc = std::locale::global(std::locale(""));
		std::ofstream os(szTypeFile, std::ios::trunc);
		std::locale::global(std::locale(loc )); 
		os << pTreeNode->GetChildren() << std::endl;
		for (UINT i=0;i<pTreeNode->GetChildren();i++)
		{
			cBaseNode * pChild = pTreeNode->GetChildNode(i);
			if(!ExportToExcel(pChild,szName))
				return FALSE;

			const TCHAR * lpszType = pChild->GetDesc()->GetDescName();
			os << pChild->GetName() << _T(":") << pChild->GetAlias() << _T(":") << lpszType << std::endl;
		}
	}
	// 是节点
	else
	{
		_tcscat(szName,_T(".xml"));
		cLeafNode * pLeafNode = (cLeafNode * )pNode;
		pLeafNode->ExportToExcel(szName);
	}
	return TRUE;	
}

BOOL cWorldImp::SaveInstInfoToFile(const TCHAR * lpszFile)
{
	std::locale loc = std::locale::global(std::locale(""));
	std::ofstream os(lpszFile, std::ios::trunc);
	std::locale::global(std::locale(loc )); 
	if (os)	
	{
		pug::xml_parser xml;
		xml.create();
		pug::xml_node root = xml.document();
		pug::xml_node pi = root.append_child(pug::node_pi,_T("xml"));
		pi.attribute(_T("version")) = _T("1.0");

		pug::xml_node top = root.append_child(pug::node_element,_T("Root"));
		int nCnt = 0;
		int nInstance = this->GetNextInstHandle(0);
		while(nInstance)
		{
			cInstNode* pNode = const_cast<cInstNode*>(this->GetInstance(nInstance));
			SaveInstToXmlNode(&top,pNode);
			nInstance = this->GetNextInstHandle(nInstance);
			nCnt++;
		}
		top.append_attribute(_T("count"),(long)nCnt);
		os << root;

		return TRUE;
	}

	return FALSE;
}

void cWorldImp::SaveInstToXmlNode(HANDLE hXmlNode,cInstNode * pNode)
{
	pug::xml_node * pXmlNode = (pug::xml_node *)hXmlNode; 
	pug::xml_node node = pXmlNode->append_child(pug::node_element,_T("Node"));

	// 通用
	node.append_attribute(_T("InstanceID"),(long)pNode->GetID());
	node.append_attribute(_T("UnitID"),(long)pNode->GetUnit()->GetUnitDef()->m_nID);
	node.append_attribute(_T("TempleteID"),(long)pNode->GetInteger((UINT)0));

	cLeafNodeDesc * pDesc = (cLeafNodeDesc *)pNode->GetDesc(); 
	const cLeafRow* pLeafRow = pNode->GetRow();
	pug::xml_node value = node.append_child(pug::node_element,_T("Values"));
	value.append_attribute(_T("count"),(long)pDesc->GetFieldCount());
	for(UINT i=0;i < pDesc->GetFieldCount();i++)
	{
		pug::xml_node field = value.append_child(pug::node_element,_T("Field"));
		field.set_child_value(pug::node_pcdata,pNode->GetString(i)); 
	}

	pug::xml_node attaches = node.append_child(pug::node_element,_T("Attaches"));
	attaches.append_attribute(_T("count"),(long)pNode->GetAttchments());
	for(UINT j=0;j<pNode->GetAttchments();j++)
	{
		char buff[10] = {0};
		const Attachment *pAttach = pNode->GetAttchment(j);
		pug::xml_node attach = attaches.append_child(pug::node_element,_T("Attach"));
		attach.append_attribute(_T("InstHandID"),(long)pAttach->nInstHandID);
		attach.append_attribute(_T("InstOperID"),(long)pAttach->nInstOperID);
		attach.append_attribute(_T("RelationID"),(long)pAttach->nRelationID);
	}
}

BOOL cWorldImp::LoadInstInfoFromFile(const TCHAR * lpszFile)
{
	// 打开文件
	std::locale loc = std::locale::global(std::locale(""));
	std::ifstream is(lpszFile, std::ios::in | std::ios::binary);
	std::locale::global(std::locale(loc )); 
	if (is)
	{
		// 把文件读入内存
		is.seekg(0, std::ios::end);
		int size = (int)is.tellg();
		TCHAR *bits = new char[size + 1];
		is.seekg(0, std::ios::beg);
		is.read(bits, size);
		bits[size] = 0;

		// 解析 xml 文件
		pug::xml_parser xml;
		if(xml.parse_file_in_mem(bits))
		{
			pug::xml_node root = xml.document();
			pug::xml_node first = root.first_element_by_name(_T("Root"));
			if(first.empty())
			{
				delete []bits;
				return FALSE;
			}
			UINT nCnt = (long)first.attribute(_T("count"));
			this->DestroyAllInstance();


			UINT nInstanceID = 0;
			pug::xml_node node = first.first_element_by_name(_T("Node"));
			while (nCnt)
			{
				if(node.empty())
				{
					delete []bits;
					return FALSE;
				}

				nInstanceID = (long)node.attribute(_T("InstanceID"));
				if (this->GetInstance(nInstanceID) != NULL)
				{
					if(!node.moveto_next_sibling())
					{
						node = first.first_element_by_name(_T("Node"));
						continue;
					}
					continue;
				}

				pug::xml_node attach_node = node.first_element_by_name(_T("Attaches"));
				if(attach_node.empty())
				{
					delete []bits;
					return FALSE;
				}

				UINT nAttachCnt = (long)attach_node.attribute(_T("count"));
				bool bCanSafeLoad = true;
				for (UINT i=0;i<nAttachCnt;i++)
				{
					pug::xml_node attach = attach_node.first_element_by_name(_T("Attach"));
					if(attach_node.empty())
					{
						delete []bits;
						return FALSE;
					}

					UINT nInstHandID = (long)attach.attribute(_T("InstHandID"));
					UINT nInstOperID = (long)attach.attribute(_T("InstOperID"));
					//if ( (nInstHandID!=nInstanceID && pWorld->GetInstance(nInstHandID)==NULL) || pWorld->GetInstance(nInstOperID)==NULL)
					if (this->GetInstance(nInstOperID)==NULL)
					{
						//如果和他关联的实例还有未加载的，先滞后装载该实例
						bCanSafeLoad = false;
						break;
					}
				}

				if (bCanSafeLoad)
				{
					LoadInstFromXmlNode(&node);
					nCnt--;
				}
				if(!node.moveto_next_sibling())
				{
					node = first.first_element_by_name(_T("Node"));
					continue;
				}
			}

 
		}

		delete []bits;
	}
	return TRUE;
}

void cWorldImp::LoadInstFromXmlNode(HANDLE hXmlNode)
{
	pug::xml_node * pXmlNode = (pug::xml_node * )hXmlNode;
	std::string strUnitID = pXmlNode->attribute("UnitID").getvalue();
	std::string strTempleteID = pXmlNode->attribute("TempleteID").getvalue();
	std::string strInstanceID = pXmlNode->attribute("InstanceID").getvalue();

	const cUnit* pUnit = this->GetUnit(atoi(strUnitID.c_str()));
	cInstNode* pNode = this->CreateInstance(pUnit,atoi(strTempleteID.c_str()),atoi(strInstanceID.c_str()));

	pug::xml_node field_node = pXmlNode->first_element_by_name(_T("Values"));
	if(field_node.empty())
		return;

	UINT nFieldCnt = (long)field_node.attribute(_T("count"));
	if(!nFieldCnt)
		return;

	pug::xml_node field = field_node.first_element_by_name(_T("Field"));
	if(!field.empty())
	{

		for(UINT i=0;i<nFieldCnt;i++)
		{

			std::string value = field.child_value();
			pNode->SetString(i,(TCHAR*)value.c_str()); 
			if(!field.moveto_next_sibling())
				break;
		}

	}

	pug::xml_node attach_node = pXmlNode->first_element_by_name(_T("Attaches"));
	if(attach_node.empty())
		return;

	UINT nAttachCnt = (long)attach_node.attribute(_T("count"));
	if(!nAttachCnt)
		return;

	pug::xml_node attach = attach_node.first_element_by_name(_T("Attach"));
	if(!attach.empty())
	{

		for(UINT i=0;i<nAttachCnt;i++)
		{
			UINT nInstHandID = (long)attach.attribute(_T("InstHandID"));
			UINT nInstOperID = (long)attach.attribute(_T("InstOperID"));
			UINT nRelationID = (long)attach.attribute(_T("RelationID"));

			Attachment attachment = {nRelationID,nInstOperID,nInstHandID};
			if(pNode->Attach(&attachment) != 0)
			{
				MessageBox(NULL,_T("不允许在本实例上建此关系！"),_T("衰吧"),MB_OK);
				return;
			}
			if(!attach.moveto_next_sibling())
				break;
		}

	}

}


const TCHAR * lpszFormulaBeg = {_T(
"/////////////////////////////////////////////////////////////////////////////\n\
// 此代码由逻辑编辑器生成,请不要手工编辑这部分代码\n\
//	生成日期:%04d-%02d-%02d %02d:%02d:%02d\n\
/////////////////////////////////////////////////////////////////////////////\n\
#pragma once\n\
extern cFormulaManager_Imp g_FormulaManagerImp;\n\
\n\n")};

const TCHAR * lpszEffectFormulaBeg = {_T(
"class cEffectFormula : public cBaseFormula\n\
{\n\
public:\n\
	cEffectFormula(){}\n\
	~cEffectFormula(){}\n")
};

const TCHAR * lpszEffectFormulaDeclare = {_T(
	"\t//描述：%s 影响方式的计算公式原型\n\
	static float EF_%s(float v,float v_max,double a);\n")
};

const TCHAR * lpszEffectFormulaDefine = {_T(
	"//描述：%s 影响方式的计算公式原型\n\
float cEffectFormula::EF_%s(float v,float v_max,double a)\n\
{\n\
	//todo:添加处理逻辑\n\
	return g_FormulaManagerImp.GetLuaExplain()->GetEffectValue(\"lua_%s\",v,v_max,a);\n\
}\n")
};

const TCHAR * lpszFieldFormulaBeg = {_T(
"class cFieldFormula : public cBaseFormula\n\
{\n\
public:\n\
	cFieldFormula(){}\n\
	~cFieldFormula(){}\n")
};

const TCHAR * lpszFieldFormulaDeclare = {_T(
	"\t//描述：%s 字段最终值计算公式原型\n\
	static float FF_%s%s(void * pOrigFields,void * pCurFields,int n,int * v);\n")
};

const TCHAR * lpszFieldFormulaDefine = {_T(
	"//描述：%s 字段最终值计算公式原型\n\
float cFieldFormula::FF_%s%s(void * pOrigFields,void * pCurFields,int n,int * v)\n\
{\n\
	//todo:添加处理逻辑\n\
	return g_FormulaManagerImp.GetLuaExplain()->GetFieldValue(%d,\"lua_%s%s\",pOrigFields,pCurFields,n,v);\n\
}\n")
};

const TCHAR * lpszBehaviorFormulaBeg = {_T(
"class cBehaviorFormula : public cBaseFormula\n\
{\n\
public:\n\
	cBehaviorFormula(){}\n\
	~cBehaviorFormula(){}\n")
};

const TCHAR * lpszBehaviorFormulaDeclare = {_T(
	"\t//描述：%s 行为计算原型\n\
	static float BF_%s(void * pSrcOrigFields,void * pSrcCurFields,void * pDstOrigFields,void * pDstCurFields);\n")
};

const TCHAR * lpszBehaviorFormulaDefine = {_T(
	"//描述：%s 行为计算原型\n\
float cBehaviorFormula::BF_%s(void * pSrcOrigFields,void * pSrcCurFields,void * pDstOrigFields,void * pDstCurFields)\n\
{\n\
	//todo:添加处理逻辑\n\
	return g_FormulaManagerImp.GetLuaExplain()->GetBehaviorResult(%d,%d,\"lua_%s\",pSrcOrigFields,pSrcCurFields,pDstOrigFields,pDstCurFields);\n\
}\n")
};


const TCHAR * lpszSkillFormulaBeg = {_T(
"class cSkillFormula : public cBaseFormula\n\
{\n\
public:\n\
	cSkillFormula(){}\n\
	~cSkillFormula(){}\n")
};

const TCHAR * lpszSkillFormulaDeclare = {_T(
	"\t//描述：%s 技能伤害计算原型\n\
	static float BS_%s(int nSrcUnit,int nDestUnit,void * pSrc,void * pDst,double dwMin,double dwMax);\n")
};

const TCHAR * lpszSkillFormulaDefine = {_T(
	"//描述：%s 技能伤害计算原型\n\
float cSkillFormula::BS_%s(int nSrcUnit,int nDestUnit,void * pSrc,void * pDst,double dwMin,double dwMax)\n\
{\n\
	//todo:添加处理逻辑 目前这里的两个单元ID暂时是硬编码，留作改进\n\
	return g_FormulaManagerImp.GetLuaExplain()->GetSkillResult(nSrcUnit,nDestUnit,\"lua_%s\",pSrc,pDst,dwMin,dwMax);\n\
}\n")
};

const TCHAR * lpszFormulaInitDeclare = {_T(
	"\tvirtual void Init();\n")
};

const TCHAR * lpszEffectFormulaInitDefine = {_T(
"void cEffectFormula::Init()\n\
{\n\
	struct EffectFormula *pFormula = &g_EffectFormula[0];\n\
	while(pFormula->m_pFn)\n\
	{\n\
		m_pm->AddFormula((void *)pFormula);\n\
		pFormula++;\n\
	}\n\
}\n")
};

const TCHAR * lpszFieldFormulaInitDefine = {_T(
"void cFieldFormula::Init()\n\
{\n\
	struct FieldFormula *pFormula = &g_FieldFormula[0];\n\
	while(pFormula->m_pFn)\n\
	{\n\
		m_pm->AddFormula((void *)pFormula);\n\
		pFormula++;\n\
	}\n\
}\n")
};

const TCHAR * lpszBehaviorFormulaInitDefine = {_T(
"void cBehaviorFormula::Init()\n\
{\n\
	struct BehaviorFormula *pFormula = &g_BehaviorFormula[0];\n\
	while(pFormula->m_pFn)\n\
	{\n\
		m_pm->AddFormula((void *)pFormula);\n\
		pFormula++;\n\
	}\n\
}\n")
};

const TCHAR * lpszSkillFormulaInitDefine = {_T(
"void cSkillFormula::Init()\n\
{\n\
	struct SkillFormula *pFormula = &g_SkillFormula[0];\n\
	while(pFormula->m_pFn)\n\
	{\n\
		m_pm->AddFormula((void *)pFormula);\n\
		pFormula++;\n\
	}\n\
}\n")
};

const TCHAR * lpszEffectArrayBeg = {_T(
	"struct EffectFormula g_EffectFormula[] = {\n")
};

const TCHAR * lpszEffectArrayItem = {_T(
	"\t{%d,\"%s\",\"%s\",(FnEffectFormula *)&cEffectFormula::EF_%s},\n")
};

const TCHAR * lpszFieldArrayBeg = {_T(
	"struct FieldFormula g_FieldFormula[] = {\n")
};

const TCHAR * lpszFieldArrayItem = {_T(
	"\t{%d,%d,(FnFieldFormula *)&cFieldFormula::FF_%s%s},\n")
};

const TCHAR * lpszBehaviorArrayBeg = {_T(
	"struct BehaviorFormula g_BehaviorFormula[] = {\n")
};

const TCHAR * lpszBehaviorArrayItem = {_T(
	"\t{%d,\"%s\",\"%s\",(FnBehaviorFormula *)&cBehaviorFormula::BF_%s},\n")
};

const TCHAR * lpszSkillArrayBeg = {_T(
	"struct SkillFormula g_SkillFormula[] = {\n")
};

const TCHAR * lpszSkillArrayItem = {_T(
	"\t{%d,(FnSkillFormula *)&cSkillFormula::BS_%s},\n")
};

const TCHAR * lpszArrayEnd = {_T(
 "\tNULL\n\
};\n")};

const TCHAR * lpszFormulaEnd = {_T(
"};\n")};



BOOL cWorldImp::BuildFormulaSrc(const TCHAR * lpszPath)
{
	::MessageBox(NULL,"最终解释权归程序","此功能已作废",MB_OK);
	return TRUE;
}

BOOL cWorldImp::BuildLuaFrame(const TCHAR * lpszPath)
{
	FILE * fp = _tfopen(lpszPath,"w");
	if(!fp)
		return FALSE;
		
	_ftprintf(fp,_T("--[[--------LUA脚本框架--------]]--\n"));
	_ftprintf(fp,_T("--[[--注意请不要修改函数名称---]]--\n\n\n\n"));
	
	
	//取得操作接点的信息
	_ftprintf(fp,_T("--[[影响方式计算公式]]--\n\n"));
	cLeafNode *pOpNode = (cLeafNode *)GetContactTemplate(CONTACT_EFFECT_OP_SCRIPT);
	for(UINT i=0;i<pOpNode->GetRows();i++)
	{
		_ftprintf(fp,_T("--[%s] 影响方式计算公式\n"),pOpNode->GetString(i,1));
		_ftprintf(fp,_T("function lua_%s(v,v_max,a)\n"),pOpNode->GetString(i,2));
		_ftprintf(fp,_T("	local ret = 0\n"));
		_ftprintf(fp,_T("	--添加脚本计算内容\n\n"));
		_ftprintf(fp,_T("end\n\n"));
	}
	
	_ftprintf(fp,_T("--[[字段最终值计算公式]]--\n\n"));
	cLeafNode * pUnitMgr = GetUnitManager();
	if(pUnitMgr)
	{

		for(UINT u=0;u<pUnitMgr->GetRows();u++)
		{
			int nType = pUnitMgr->GetInteger(u,_T("类别"));
			if((UnitType)nType == Unit_Base)
				continue;
			const TCHAR * lpszName = pUnitMgr->GetString(u,_T(UNIT_NAME_COL));
			cLeafNode *pLeafNode = GetUnitAttrib(lpszName);
			for(UINT l=0;l<pLeafNode->GetRows();l++)
			{
				//int nIsDerive = pLeafNode->GetInteger(l,_T("是否衍生字段"));
				if(pLeafNode->IsDeriveField(l))
				{
					_ftprintf(fp,_T("--[%s]单元 [%s]字段计算公式\n"),lpszName,pLeafNode->GetString(l,_T("字段名")));
					_ftprintf(fp,_T("function lua_%s%s(all)\n"),pUnitMgr->GetString(u,_T("英文别名")),pLeafNode->GetString(l,_T("英文别名")));
					_ftprintf(fp,_T("	orig = all[\"裸值\"]\n"));
					_ftprintf(fp,_T("	curr = all[\"当前值\"]\n"));
					_ftprintf(fp,_T("	v = all[\"其他值\"]\n"));
					_ftprintf(fp,_T("	local ret = 0\n"));
					_ftprintf(fp,_T("	--添加脚本计算内容\n\n"));
					_ftprintf(fp,_T("end\n\n"));					
				}
			}
		}
	}


	_ftprintf(fp,_T("--[[行为计算公式]]--\n\n"));
	//取得操作接点的信息
	cLeafNode *pBehaviorNode = (cLeafNode *)FindFirstNode(m_pWorldRoot,Node_Beharior);
	//生成行为公式的定义
	for(UINT i=0;i<pBehaviorNode->GetRows();i++)
	{
		_ftprintf(fp,_T("--[%s]计算公式\n"),pBehaviorNode->GetString(i,_T("行为名")));
		_ftprintf(fp,_T("function lua_%s(all)\n"),pBehaviorNode->GetString(i,_T("英文别名")));
		_ftprintf(fp,_T("	srcOrig = all[\"源单元裸值\"]\n"));
		_ftprintf(fp,_T("	srcCurr = all[\"源单元当前值\"]\n"));
		_ftprintf(fp,_T("	dstOrig = all[\"目标单元裸值\"]\n"));
		_ftprintf(fp,_T("	dstCurr = all[\"目标单元当前值\"]\n"));
		_ftprintf(fp,_T("	local ret = 0\n"));
		_ftprintf(fp,_T("	--添加脚本计算内容\n\n"));
		_ftprintf(fp,_T("end\n\n"));	
	}


	_ftprintf(fp,_T("--[[技能伤害计算公式]]--\n\n"));
	cLeafNode *pScriptNode = (cLeafNode *)GetContactTemplate(CONTACT_EFFECT_VALUE_SCRIPT);
	for(UINT i=0;i<pScriptNode->GetRows();i++)
	{
		_ftprintf(fp,_T("--[%s]计算公式\n"),pScriptNode->GetString(i,1));
		_ftprintf(fp,_T("function lua_%s(all)\n"),pScriptNode->GetString(i,2));
		_ftprintf(fp,_T("	srcCurr = all[\"源单元当前值\"]\n"));
		_ftprintf(fp,_T("	dstCurr = all[\"目标单元当前值\"]\n"));
		_ftprintf(fp,_T("	min = all[\"参数1\"]\n"));
		_ftprintf(fp,_T("	max = all[\"参数2\"]\n"));
		_ftprintf(fp,_T("	local ret = 0\n"));
		_ftprintf(fp,_T("	--添加脚本计算内容\n\n"));
		_ftprintf(fp,_T("end\n\n"));
	}

	fclose(fp);
	return TRUE;
}

BOOL cWorldImp::ReadConfig(char * lpszConfig)
{
	if(!m_mapUnitPools.empty())
		m_mapUnitPools.clear();

	char lpszPath[1024] = {0};
	char *pPath = lpszPath;
	DWORD dwRet = 0;
	int pos = 0;

	GetPrivateProfileString("path","script","",lpszPath,1024,lpszConfig);
	m_sScript = lpszPath;

	dwRet = GetPrivateProfileSection("pool",lpszPath,1024,lpszConfig);
	while(dwRet > 0)
	{
		std::vector<std::string> vecTmp;
		std::string strTmp(pPath);
		SplitString(strTmp,vecTmp,"=");
		
		if(vecTmp.size() == 2)
			m_mapUnitPools[ vecTmp[0] ] = _ttoi(vecTmp[1].c_str());

		int len = strlen(pPath)+1;
		pPath += len;
		dwRet -= len;

	}
	return TRUE;
}

void cWorldImp::CreateGlobalMemPool()
{
	std::map<int,int> mapBlock;

	UINT nUnits = GetUnits();
	for (UINT n=0;n<nUnits;n++)
	{
		const cUnit * pUnit = GetUnit(n);
		if (pUnit && pUnit->GetUnitDef()->m_eType != Unit_Base)
		{
			const cLeafNodeDesc *pAttribDesc = pUnit->GetAttribDesc();
			const TCHAR *pszName = (const TCHAR *)(pUnit->GetUnitDef()->m_szAlias);
			int nLength = pAttribDesc->GetInstLength();
			if(nLength > 0)
			{
				if(m_mapUnitPools.find(pszName)==m_mapUnitPools.end()&&pAttribDesc->GetInstLength()!=0)
				{
					_printf("memory pool chunk size not specified %s,this equals to 0!\n",pszName);
				}

				std::map<int,int>::iterator i = mapBlock.find(nLength);
				if(i == mapBlock.end())
				{
					std::pair<int,int> p(pAttribDesc->GetInstLength(),m_mapUnitPools[pszName]);
					mapBlock.insert(p);
				}
				else
				{
					int& nCnt = i->second;
					nCnt += m_mapUnitPools[pszName];
				}
			}
		}
	}

	int * pnBlockCnt = new int[mapBlock.size()];
	int * pnBlockSize = new int[mapBlock.size()];
	int nSize = 0;
	std::map<int,int>::iterator iBlock = mapBlock.begin();
	while(iBlock != mapBlock.end())
	{
		pnBlockSize[nSize] = iBlock->first;
		pnBlockCnt[nSize] = iBlock->second;
		iBlock ++;
		nSize++;
	}
	// 创建内存池
	m_pMemPool = new CMemoryPool(pnBlockSize,pnBlockCnt,nSize);
	if (!m_pMemPool)
	{
		assert(false && "MemoryPool Create Failed!");
	}
	delete []pnBlockCnt;
	delete []pnBlockSize;
}

void cWorldImp::DestroyGlobalMemPool()
{
	if (m_pMemPool)
	{
		delete m_pMemPool;
		m_pMemPool = NULL;
	}
}

void * cWorldImp::GetMemory(std::size_t size,int &nId)
{
	if (m_pMemPool)
	{
		return m_pMemPool->GetMemory(size,nId);
	}
	return NULL;
}

void cWorldImp::FreeMemory(std::size_t size,int nId)
{
	if (m_pMemPool)
	{
		m_pMemPool->FreeMemory(size,nId);
	}
}


BOOL cWorldImp::ExportStringTable(const TCHAR * lpszFile)
{
	std::locale loc = std::locale::global(std::locale(""));
	std::ofstream os(lpszFile, std::ios::trunc);
	std::locale::global(std::locale(loc )); 
	if (os)	
	{
		pug::xml_parser xml;
		xml.create();
		pug::xml_node root = xml.document();
		pug::xml_node pi = root.append_child(pug::node_pi,_T("xml"));
		pi.attribute(_T("version")) = _T("1.0");
		pi.attribute(_T("encoding")) = _T("gb2312");

		pug::xml_node nodes = root.append_child(pug::node_element,_T("Nodes"));
		SaveToXmlStringTable(&nodes,m_pWorldRoot);

		os << root;

		return TRUE;
	}	
	return FALSE;
}
BOOL cWorldImp::ImportStringTable(const TCHAR * lpszFile)
{
	// 打开文件
	std::locale loc = std::locale::global(std::locale(""));
	std::ifstream is(lpszFile, std::ios::in | std::ios::binary);
	std::locale::global(std::locale(loc )); 
	if (is)
	{
		// 把文件读入内存
		is.seekg(0, std::ios::end);
		int size = (int)is.tellg();
		TCHAR *bits = new char[size + 1];
		is.seekg(0, std::ios::beg);
		is.read(bits, size);
		bits[size] = 0;

		// 解析 xml 文件
		pug::xml_parser xml;
		if(xml.parse_file_in_mem(bits))
		{
			pug::xml_node root = xml.document();
			pug::xml_node nodes = root.first_element_by_name(_T("Nodes"));

			pug::xml_node node = nodes.first_element_by_name("Node");
			if(!node.empty())
			{
				do
				{
					LoadToXmlStringTable(&node);
				}
				while(node.moveto_next_sibling());
			}
			return TRUE;
		}
	}
	return FALSE;
}
void cWorldImp::SaveToXmlStringTable(HANDLE hXmlNode,cBaseNode * pNode)
{
	pug::xml_node * pXmlNode = (pug::xml_node *)hXmlNode; 
	pug::xml_node node = pXmlNode->append_child(pug::node_element,_T("Node"));

	node.append_attribute(_T("name"),pNode->GetName());

	TCHAR szPath[MAX_PATH] = {0};
	pNode->GetPath(szPath,MAX_PATH);
	node.append_attribute(_T("path"),szPath);

	// 树节点
	if(!pNode->GetDesc()->IsLeaf())
	{
		cTreeNode * pTreeNode = (cTreeNode *)pNode;
		UINT nChildren = pTreeNode->GetChildren();
		for(UINT i=0;i<nChildren;i++)
		{
			cBaseNode * pChildNode = pTreeNode->GetChildNode(i);
			SaveToXmlStringTable(hXmlNode,pChildNode);
		}
	}
	else
	{
		// 叶节点
		cLeafNode * pLeafNode = (cLeafNode *)pNode;
		cLeafNodeDesc * pDesc = (cLeafNodeDesc *)pNode->GetDesc(); 

		Node_Type eType = pDesc->GetType();

		UINT nStringFields = 0;
		for(UINT x=0;x<pDesc->GetFieldCount();x++)
		{
			const LeafField * f = pDesc->GetField(x);
			if(f->m_nTemplateOffset>=0 && f->m_eType == Field_String)
				nStringFields ++;
		}
		if(nStringFields>0)
		{
			Node_Type nt = pDesc->GetType();
			UINT nRows = pLeafNode->GetRows();
			node.append_attribute(_T("Rows"),(long)nRows);
			for(UINT i=0;i<nRows;i++)
			{
				pug::xml_node row = node.append_child(pug::node_element,_T("Row"));
				for(UINT j=0;j<pDesc->GetFieldCount();j++)
				{
					const LeafField * f = pDesc->GetField(j);
					if(f->m_nTemplateOffset>=0 && 
						((f->m_eType == Field_String) ||
						((eType == Node_UnitTemplate) && (f->m_eType == Field_Integer) && (_tcscmp(f->m_lpszName,_T("ID"))==0)) ||
						((eType == Node_ContactData) && (f->m_eType == Field_Integer) && (j==0)))
						)
					{
						const TCHAR *lpszValue = pLeafNode->GetString(i,j);
						if(lpszValue && lpszValue[0] && ((HasChinese(lpszValue) && !Exclude(szPath,j)) || (f->m_eType == Field_Integer) || _tcscmp(lpszValue,_T("ID"))==0))
						{
							pug::xml_node field = row.append_child(pug::node_element,_T("Field"));
							field.append_attribute(_T("f"),(long)j);
							field.append_attribute(_T("v"),lpszValue);  
						}
					}
				}

				// 特例
				if( ((pDesc->GetType()==Node_UnitAttrib)&&(pLeafNode->GetInteger(i,LeafNode_Type-1)==Field_Enumerate)) 
					|| ((pDesc->GetType()==Node_ContactAttrib)&&(pLeafNode->GetInteger(i,ContactData_Type-1)==Field_Enumerate)))
				{
					UINT nEnum = 0;
					const TCHAR * lpszEnum = pLeafNode->GetEnumString(i,nEnum);
					
					pug::xml_node enum_node = row.append_child(pug::node_element,_T("Enum"));
					enum_node.append_attribute(_T("Count"),(long)nEnum);
					for(UINT n=0;n<nEnum;n++)
					{
						pug::xml_node enum_Item = enum_node.append_child(pug::node_element,_T("Item"));
						std::string sEnumItem(lpszEnum + n*MAX_NAME);
						
						int nWhere = sEnumItem.find("<",0);
						if(nWhere>=0)
							sEnumItem.replace(nWhere,1,std::string("&lt;"));
						nWhere = sEnumItem.find(">",0);
						if(nWhere>=0)
							sEnumItem.replace(nWhere,1,std::string("&gt;"));
						enum_Item.append_attribute(_T("s"),sEnumItem.c_str());  
					}
				}
				else if( ((pDesc->GetType()==Node_UnitAttrib)&&(pLeafNode->GetInteger(i,LeafNode_Type-1)==Field_IdField))
					|| ((pDesc->GetType()==Node_ContactAttrib)&&(pLeafNode->GetInteger(i,ContactData_Type-1)==Field_IdField)))
				{
					FieldId f = {0};							
					pLeafNode->GetFieldID(i,&f);
					
					pug::xml_node idfield_node = row.append_child(pug::node_element,_T("ID_Field"));
					idfield_node.append_attribute(_T("p"),f.pLeafPath);
				}
			}
		}
	}
}

BOOL cWorldImp::HasChinese(const TCHAR * lpszString)
{
	const TCHAR * lpszSrc = lpszString;
	while(lpszSrc[0] != 0)
	{
		if((char)(lpszSrc[0]) < 0)
			return TRUE;
		lpszSrc ++;
	}
	return FALSE;
}
BOOL cWorldImp::Exclude(const TCHAR * lpszPath,int nField)
{
	struct stFilter 
	{
		const TCHAR * lpszPath;
		int nField;
	};

	stFilter filter[] = {
		{_T("Root/UnitTree/map/UnitTemplate"),3},
		{_T("Root/UnitTree/map/UnitTemplate"),4},
		{_T("Root/UnitTree/NPC/UnitTemplate"),79},
		{_T("Root/ContactTree/LurkShape/ContactData"),1}
	};

	UINT nCount = sizeof(filter)/sizeof(stFilter);
	for(UINT n=0;n<nCount;n++)
	{
		if(!_tcscmp(lpszPath,filter[n].lpszPath) && (nField == filter[n].nField))
			return TRUE;
	}
	return FALSE;
}

void cWorldImp::LoadToXmlStringTable(HANDLE hXmlNode)					// 从Xml节点读取
{
	pug::xml_node * pXmlNode = (pug::xml_node * )hXmlNode;
	std::string strName = pXmlNode->attribute("name").getvalue();
	std::string strPath = pXmlNode->attribute("path").getvalue();
	UINT nRows = (UINT)(long)pXmlNode->attribute("Rows");

	const cBaseNode * pNode = m_pWorldRoot->GetNodeByPath(strPath.c_str());
	if(!pNode)
		return;

	// 这样修改节点名字确实是危险的,是多语种化时为了少改动,做的权宜之计.
	TCHAR * lpszName = (TCHAR *)pNode->GetName();
	_tcscpy(lpszName,strName.c_str());

	// 树节点
	const cNodeDesc * pDesc = pNode->GetDesc();
	if(pDesc->IsLeaf())
	{
		// 叶节点
		cLeafNode * pLeafNode = (cLeafNode *)pNode;
		cLeafNodeDesc * pLeafDesc = (cLeafNodeDesc *)pLeafNode->GetDesc(); 
		Node_Type nt = pLeafDesc->GetType();
		pug::xml_node row = pXmlNode->first_element_by_name(_T("Row"));
		if(!row.empty())
		{
			int i = 0;
			do
			{
				pug::xml_node field = row.first_element_by_name(_T("Field"));
				if(!field.empty())
				{
					do
					{
						long f = field.attribute(_T("f"));
						std::string v = field.attribute(_T("v")).getvalue();
						const LeafField * lf = pLeafDesc->GetField(f);
						if(lf && (lf->m_eType == Field_String) )
							pLeafNode->SetString(i,f,(TCHAR*)v.c_str()); 
					}
					while(field.moveto_next_sibling(_T("Field")));
				}

				// 特例
				if( ((pDesc->GetType()==Node_UnitAttrib)&&(pLeafNode->GetInteger(i,LeafNode_Type-1)==Field_Enumerate))
					|| ((pDesc->GetType()==Node_ContactAttrib)&&(pLeafNode->GetInteger(i,ContactData_Type-1)==Field_Enumerate)))
				{
					pug::xml_node enum_node = row.first_element_by_name(_T("Enum"));
					if(!enum_node.empty())
					{
						UINT nEnum = (long)enum_node.attribute(_T("Count"));
						pug::xml_node enum_Item = enum_node.first_element_by_name(_T("Item"));
						if(!enum_Item.empty())
						{
							TCHAR * lpszEnum = new TCHAR[nEnum * MAX_NAME];
							ZeroMemory(lpszEnum,sizeof(TCHAR)*nEnum * MAX_NAME);
							UINT nItems = 0;

							do
							{
								std::string value = enum_Item.attribute(_T("s")).getvalue();
								int nWhere = value.find("&lt;",0);
								if(nWhere>=0)
									value.replace(nWhere,4,std::string("<"));
								nWhere = value.find("&gt;",0);
								if(nWhere>=0)
									value.replace(nWhere,4,std::string(">"));


								lstrcpyn(lpszEnum + nItems * MAX_NAME,value.c_str(),MAX_NAME-1);
								nItems ++;
							}
							while(enum_Item.moveto_next_sibling(_T("Item")));
							
							assert( nItems == nEnum);

							pLeafNode->SetEnumString(i,lpszEnum,nEnum); 
							delete []lpszEnum;
						}
					}
				}
				else if( ((pDesc->GetType()==Node_UnitAttrib)&&(pLeafNode->GetInteger(i,LeafNode_Type-1)==Field_IdField))
					|| ((pDesc->GetType()==Node_ContactAttrib)&&(pLeafNode->GetInteger(i,ContactData_Type-1)==Field_IdField)))
				{
					pug::xml_node idfield_node = row.first_element_by_name(_T("ID_Field"));
					if(!idfield_node.empty())
					{
						FieldId f;							
						f.nDispField = (long)idfield_node.attribute(_T("Field"));
						f.nValField = (long)idfield_node.attribute(_T("Value"));
						std::string sPath = (std::string)idfield_node.attribute(_T("Path")).getvalue();
						f.pLeafPath = (TCHAR*)sPath.c_str(); 
						if(f.nDispField && !sPath.empty())
							pLeafNode->SetFieldID(i,&f); 
					}
				}
				i ++;
			}
			while(row.moveto_next_sibling(_T("Row")));

			assert( i == nRows);

			if(pDesc->GetType()==Node_UnitAttrib)
			{
				// 要刷新自己对数据模板的描述.
				cTemplateLeafNodeDesc * pTemplateDesc = (cTemplateLeafNodeDesc*)((cAttribLeafNode*)pLeafNode)->GetTargetDesc();
				pTemplateDesc->Build(); 
			}
			else if(pDesc->GetType()==Node_ContactAttrib)
			{
				// 要刷新自己对数据模板的描述.
				cContactDataLeafNodeDesc * pContactDesc = (cContactDataLeafNodeDesc*)((cContactAttribLeafNode*)pLeafNode)->GetTargetDesc();
				pContactDesc->Build();
			}
			pLeafNode->BuildIndex(); 
		}
	}
}

DefineString CWorldDefine::GetStringType(std::string &Define)
{
	std::string sDefineType[] = {"enum","FieldId","table","tree"};
	for(int i=DSEnum;i<DS_Max;i++)
	{
		if(sDefineType[i] == Define)
			return (DefineString)i;
	}
	return (DefineString)-1;
}

CWorldDefine::CWorldDefine()
{
	TCHAR szBuf[MAX_PATH] = {0};
	if(GetEnvironmentVariable(_T("LogicDefine"),szBuf,MAX_PATH) == 0)
		_tcscpy(szBuf,_T(".\\WorldDefine_gb2312.xml"));
	
	// 打开文件
	std::locale loc = std::locale::global(std::locale(""));
	std::ifstream is(szBuf, std::ios::in | std::ios::binary);
	std::locale::global(std::locale(loc )); 
	if (is)
	{
		// 把文件读入内存
		is.seekg(0, std::ios::end);
		int size = (int)is.tellg();
		TCHAR *bits = new char[size + 1];
		is.seekg(0, std::ios::beg);
		is.read(bits, size);
		bits[size] = 0;

		// 解析 xml 文件
		pug::xml_parser xml;
		if(xml.parse_file_in_mem(bits))
		{
			pug::xml_node root = xml.document();
			pug::xml_node node = root.first_element_by_name(_T("defines"));
			if(node.empty())
				return;

			pug::xml_node define = node.first_element_by_name(_T("string"));
			if(!define.empty())
			{
				do
				{
					std::string sType = define.attribute(_T("type")).getvalue();
					DefineString eType = GetStringType(sType);
					if(eType >= 0 && eType<DS_Max)
					{
						std::string sName = define.attribute(_T("name")).getvalue();
						
						std::map<std::string,std::vector<std::string>>::iterator it = m_mapStrings[eType].find(sName);
						assert(it == m_mapStrings[eType].end());

						std::vector<std::string> vecString;
						pug::xml_node item = define.first_element_by_name(_T("item"));
						if(!item.empty())
						{
							do
							{
								std::string sString = item.attribute(_T("string")).getvalue();
								vecString.push_back(sString);
							}
							while(item.moveto_next_sibling(_T("item")));
						}

						std::pair<std::string,std::vector<std::string>> p(sName,vecString);
						m_mapStrings[eType].insert(p);
					}
				}
				while(define.moveto_next_sibling(_T("string")));
			}
		}
		delete []bits;
	}
}
TCHAR * CWorldDefine::Str(DefineString eType,const TCHAR * lpszName,UINT nIndex)
{
	if(eType >= 0 && eType<DS_Max)
	{
		std::string sName(lpszName);
		std::map<std::string,std::vector<std::string>>::iterator it = m_mapStrings[eType].find(sName);
		if(it != m_mapStrings[eType].end())
		{
			std::vector<std::string>& vecString = it->second;
			if(nIndex < vecString.size())
			{
				return (TCHAR *)vecString[nIndex].c_str();
			}
		}
	}	
	return NULL;
}
