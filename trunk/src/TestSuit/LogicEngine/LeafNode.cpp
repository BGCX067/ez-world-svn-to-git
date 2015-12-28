#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <malloc.h>
#include <fstream>
#include <strstream>
#include <shlwapi.h>
#include "LogicEngine.h"
#include "TreeNode.h"
#include "LeafNode.h"
#include "pugxml.h"			// xml parser
#include "theworld.h"
#include <algorithm>
#include <functional> 
#define OVERLOAD
#include "overload.h"
// #define _CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>
// #ifdef _DEBUG
// #define   new   new(_NORMAL_BLOCK,   __FILE__,   __LINE__)
// #endif
#define EXCEL_CELL_END	_T("\r\n")
extern cWorldImp g_NodeStreamImp;

static char	szNumber[16];

#define CONTACT_NAME_COL	1
#define CONTACT_ALIAS_COL	2
#define UNIT_NAME_COL	1
#define UNIT_ALIAS_COL	2

// todo:所有的字符串拷贝增加长度校验以免出界。

void SplitString(std::string str,std::vector<std::string>& vecString,std::string spliter)
{
	// 根据 , 分开
	size_t offset1 = 0; 
	size_t offset2 = 0;
	do
	{
		offset2=str.find(spliter.c_str(),offset1);
		std::string e = str.substr(offset1, offset2-offset1);
		if(!e.empty())
			vecString.push_back(e); 
		offset1 = offset2+spliter.size();
	}
	while(offset2 != -1);
}

cLeafNodeDesc_Imp::cLeafNodeDesc_Imp(Node_Type type, 
				  TCHAR * lpszName, 
				  BOOL bFiled,
				  BOOL bExclusive, 
				  LeafField * pFields
		)
			:m_eType(type)
			,m_bFiled(bFiled)
			,m_bExclusive(bExclusive)
			,m_nFields(0)
			,m_pFields(pFields)
			,m_nTempLength(0)
			,m_nInstLength(0)
			,m_nIDField(-1)
{
	lstrcpy(m_szName,lpszName);
	
	if(m_pFields)
	{
		m_pFields[m_nFields].m_nTemplateOffset = 0;
		m_pFields[m_nFields].m_nInstanceOffset = 0;
		while(m_pFields[m_nFields].m_lpszName)
		{
			Field f = {m_pFields + m_nFields,m_nFields};
			m_vecFields.push_back(f);
			if(m_nIDField == -1 && lstrcmpi(f.f->m_lpszName,_T("ID")) == 0)
				m_nIDField = m_nFields;
			
			m_nTempLength += m_pFields[m_nFields].m_nLength;
			m_pFields[m_nFields+1].m_nTemplateOffset=m_pFields[m_nFields].m_nTemplateOffset + m_pFields[m_nFields].m_nLength;  
			m_pFields[m_nFields+1].m_nInstanceOffset=m_pFields[m_nFields+1].m_nTemplateOffset;
			m_nFields++;
		}
		m_nTempLength += m_pFields[m_nFields-1].m_nLength;
		m_nInstLength += m_pFields[m_nFields-1].m_nLength;

		if(m_nFields > 1)
			std::sort(m_vecFields.begin(),m_vecFields.end());
	}
}

cLeafNodeDesc_Imp::~cLeafNodeDesc_Imp()
{
	m_eType = (Node_Type)-1;
	m_szName[0] = 0;
	m_bFiled = FALSE;
	m_bExclusive = FALSE;
	m_nFields = 0;
	m_pFields = NULL;
}

const LeafField * cLeafNodeDesc_Imp::GetField(UINT n)const 
{
	/*
	 *	------- edit by machaoyang -------
	 *	n可能会超出界限，这里添加的判断
	*/
	if(m_pFields && n<m_nFields)
		return m_pFields+n;
	return NULL;
}
UINT cLeafNodeDesc_Imp::FindField(const TCHAR * lpszField)const 
{
	// 特别为 "ID" 字段加速
	if(lstrcmpi(lpszField,_T("ID")) == 0 && m_nIDField != -1)
		return m_nIDField;
	
	if(m_nFields>1)
	{
		LeafField lf = {(TCHAR*)lpszField};
		const Field f = {&lf,0};
		std::vector<Field>::const_iterator it = std::find(m_vecFields.begin(),m_vecFields.end(),f);
		if(it != m_vecFields.end())
			return it->idx;
	}
	else
	{
		if(m_pFields!=NULL)
		{
			if(!lstrcmpi(lpszField,m_pFields[0].m_lpszName))
				return 0;
		}	
	}
	return (UINT)-1;
}

UINT cLeafNodeDesc_Imp::FindFieldByAlias(const TCHAR * lpszFieldAlias)const 
{
	for(UINT i=0; i<m_nFields;i++)
	{
		if(!lstrcmpi(lpszFieldAlias,m_pFields[i].m_lpszAlias))
			return i;
	}
	return (UINT)-1;
}

NodeTable g_NodeTable[];	

cLeafNode_Imp::cLeafNode_Imp(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent)
	:m_pNodeDesc((cLeafNodeDesc *)pDesc)
	,m_pParent(pParent)
	,m_pBuffer(NULL)
	,m_pRows(NULL)
	,m_nRows(0)
	,m_nPreAllocRows(0)
#ifdef _DEBUG
	,m_pFieldValues(NULL)
#endif
	,m_ppIndices(NULL)
	,m_pKeys(NULL)
	,m_nIndices(0)
{
	lstrcpy(m_szName,lpszName);
	lstrcpy(m_szAlias,lpszAlias);
	
	// 如果类型是单元模板,需要找到自己的描述叶节点
	if(pDesc->GetType() == Node_UnitTemplate || pDesc->GetType() == Node_ContactData)
	{
		m_pNodeDesc = FindNodeDesc();
		assert(m_pNodeDesc);
	}
}
cLeafNode_Imp::~cLeafNode_Imp()
{
	delete []m_pBuffer;
	delete []m_pRows;

	for(UINT i=0;i<m_nIndices;i++)
	{
		if(m_ppIndices[i])
			delete []m_ppIndices[i]; 
	}
	delete []m_ppIndices;
	delete []m_pKeys;
	
	m_pNodeDesc = NULL;
	m_pParent = NULL;
	m_szName[0] = NULL;
	
	m_pBuffer = NULL;
	m_pRows = NULL;
	m_nRows = 0;
	m_nPreAllocRows = 0;
	
	m_ppIndices = NULL;
	m_nIndices = 0;
	m_pKeys = NULL;

#ifdef _DEBUG
	delete []m_pFieldValues;
	m_pFieldValues = NULL;
#endif

	if (!m_mapFormat.empty())
	{
		m_mapFormat.clear();
	}
	if (!m_mapRowDesc.empty())
	{
		m_mapRowDesc.clear();
	}
	if (!m_mapFieldType.empty())
	{
		m_mapFieldType.clear();
	}
}

void cLeafNode_Imp::BuildRelation()
{
	if(m_pNodeDesc->GetType() == Node_UnitTemplate)
	{
		const cUnit * pUnit = ((cTreeNode *)m_pParent)->GetUnit();  
		const cLeafNode * pLimite = pUnit->GetLeaf(Node_UnitLimite); 
		const cLeafNode * pEffect = pUnit->GetLeaf(Node_UnitEffect); 
		const cLeafNode * pBuff	  = pUnit->GetLeaf(Node_BuffEffect); 
		for(UINT n=0;n<m_nRows;n++)
			m_pRows[n].BuildRelation(pLimite,pEffect,pBuff);  
	}
}

const cNodeDesc * cLeafNode_Imp::GetDesc()const 
{
	return m_pNodeDesc;
}	

const TCHAR * cLeafNode_Imp::GetPath(TCHAR * lpszBuf,size_t size)const 			// 取得自己的路径
{
	// todo: 增加根据 size 的 buf 超界判断
	if(m_pParent)
		m_pParent->GetPath(lpszBuf,size);
	lstrcat(lpszBuf,m_szAlias); 
	return lpszBuf;
}

const TCHAR * cLeafNode_Imp::GetString(UINT row,UINT col)const 
{
	if(row < m_nRows)
		return m_pRows[row].GetString(col);
	return NULL;
}
int cLeafNode_Imp::GetInteger(UINT row,UINT col)const 
{
	if(row < m_nRows)
		return m_pRows[row].GetInteger(col); 
	return 0;
}
float cLeafNode_Imp::GetFloat(UINT row,UINT col)const 						// 取得指定位置的值
{
	if(row < m_nRows)
		return m_pRows[row].GetFloat(col); 
	return 0.0f;
}
void cLeafNode_Imp::SetString(UINT row,UINT col,const TCHAR * string)
{
	ValidRow(row+1,TRUE);
	m_pRows[row].SetString(col,string);
}
void cLeafNode_Imp::SetInteger(UINT row,UINT col,int nValue)
{
	ValidRow(row+1,TRUE);
	m_pRows[row].SetInteger(col,nValue);
}
void cLeafNode_Imp::SetFloat(UINT row,UINT col,float fValue)
{
	ValidRow(row+1,TRUE);
	m_pRows[row].SetFloat(col,fValue);
}

const TCHAR * cLeafNode_Imp::GetString(UINT row,const TCHAR * lpszField)const 
{
	GetDesc();
	UINT col = m_pNodeDesc->FindField(lpszField);
	if(col != (UINT)-1)
		return GetString(row,col);
	return NULL;
}
int cLeafNode_Imp::GetInteger(UINT row,const TCHAR * lpszField)const 
{
	GetDesc();
	UINT col = m_pNodeDesc->FindField(lpszField);
	if(col != (UINT)-1)
		return GetInteger(row,col);
	return 0;
}
float cLeafNode_Imp::GetFloat(UINT row,const TCHAR * lpszField)const 						// 取得指定位置的值
{
	GetDesc();
	UINT col = m_pNodeDesc->FindField(lpszField);
	if(col != (UINT)-1)
		return GetFloat(row,col);
	return 0.0f;
}
void cLeafNode_Imp::SetString(UINT row,const TCHAR * lpszField,const TCHAR * string)
{
	GetDesc();
	UINT col = m_pNodeDesc->FindField(lpszField);
	if(col != (UINT)-1)
		SetString(row,col,string);
}
void cLeafNode_Imp::SetInteger(UINT row,const TCHAR * lpszField,int nValue)
{
	GetDesc();
	UINT col = m_pNodeDesc->FindField(lpszField);
	if(col != (UINT)-1)
		SetInteger(row,col,nValue);
}
void cLeafNode_Imp::SetFloat(UINT row,const TCHAR * lpszField,float fValue)
{
	GetDesc();
	UINT col = m_pNodeDesc->FindField(lpszField);
	if(col != (UINT)-1)
		SetFloat(row,col,fValue);
}

const cBaseNode * cLeafNode_Imp::GetNodeByPath(const TCHAR * lpszPath)const 
{
	if(_stricmp(lpszPath,m_szAlias))
		return NULL;
	return this;
}

int cLeafNode_Imp::GetFieldIndex(const TCHAR * lpszString)const 
{
	cLeafNodeDesc * pLeafNodeDesc = (cLeafNodeDesc *)GetDesc();
	UINT nFieldCount = pLeafNodeDesc->GetFieldCount(); 
	for(UINT i=0;i<nFieldCount;i++)
	{
		if(!lstrcmpi(pLeafNodeDesc->GetField(i)->m_lpszName,lpszString))
			return i;
	}
	return -1;
}

HRESULT cLeafNode_Imp::ImportFromExcel(const TCHAR * lpszFile)
{
	std::locale loc = std::locale::global(std::locale(""));
	std::ifstream is(lpszFile, std::ios::in | std::ios::binary);
	std::locale::global(std::locale(loc )); 
	if (!is)
		return IEXCEL_NOFILE;

	is.seekg(0, std::ios::end);
	int size = (int)is.tellg();
	char * pBits = new char[size + 1];

	is.seekg(0, std::ios::beg);
	is.read(pBits, size);
	pBits[size] = 0;
	
	// 转换编码
	{
		int uSize = ::MultiByteToWideChar(CP_UTF8,0,pBits,size+1,NULL,0);
		wchar_t * pUstr = new wchar_t[uSize+1];
		size = ::MultiByteToWideChar(CP_UTF8,0,pBits,size+1,pUstr,uSize);

		size = ::WideCharToMultiByte(CP_ACP,0,pUstr,-1,NULL,0,NULL,NULL);
		delete []pBits;
		pBits = new char[size + 1];
		size = ::WideCharToMultiByte(CP_ACP,0,pUstr,-1,pBits,size,NULL,NULL);
		pBits[size]=0;
	}


	TCHAR * pBuffer = (TCHAR * )pBits;

	pug::xml_parser xml;
	// xml parser 释放 pBuffer
	if(xml.parse_file_in_mem(pBuffer))
	{
//		if(m_nPreAllocRows)
//		{
//			UINT nLen = m_pNodeDesc->GetRowLength();  
//			ZeroMemory(m_pBuffer, sizeof(BYTE)*m_nPreAllocRows*nLen);
//
//#ifdef _DEBUG
//			UINT nFields = m_pNodeDesc->GetFieldCount();  
//			ZeroMemory(m_pFieldValues, sizeof(FieldValue)*m_nPreAllocRows*nFields);
//#endif
//		}
		
		pug::xml_node root = xml.document();
		pug::xml_node node = root.first_element_by_path("Workbook/Worksheet/Table","/");
		if(node.has_attribute("ss:ExpandedColumnCount") && node.has_attribute("ss:ExpandedRowCount"))
		{
			int nCols = (int)node.attribute("ss:ExpandedColumnCount");
			int nRows = (int)node.attribute("ss:ExpandedRowCount")-1;

			ValidRow(nRows,FALSE);

			if(nCols < 1)
				return IEXCEL_NOCOL;
			if(nRows < 0)
				return IEXCEL_NOROW;

			bool bRet;

			// 清除以前数据
			UINT nRealRow = 0;
			
			bRet = node.moveto_child("Row");					// Row
			
			// 为每列找到相应的对应域
			int * pnColIndices = new int[nCols];
			FillMemory(pnColIndices,sizeof(int)*nCols,-1);
			bRet = node.moveto_child("Cell");					// Cell
			for(int i=0;i<nCols;i++)
			{
				bRet = node.moveto_child("Data");				// Data
				if(bRet)
				{
					std::string value = node.child_value(); 
					pnColIndices[i] = GetFieldIndex(value.c_str()); 
					bRet = node.moveto_parent();					// Cell
				}
				if(!node.moveto_next_sibling())					// Next Cell
					break;
			}
			bRet = node.moveto_parent(); 
			bRet = node.moveto_next_sibling();

			// 读取实际数据
			for(int i=0; i<nRows;i++)
			{
				bRet = node.moveto_child("Cell");							// Cell
				int nNextCell = -1;
				for(int j=0;j<nCols;j++)
				{
					// Excel 遇到空格时 有时候会不产生节点.
					if(nNextCell>=0)
					{
						if(nNextCell > j)
							continue;
						else
							nNextCell = -1;
					}
					else
					{
						if(node.has_attribute("ss:Index"))
						{
							nNextCell = (int)node.attribute("ss:Index")-1;
							continue;
						}
					}

					
					if(pnColIndices[j]>=0)
					{
						bRet = node.moveto_child("Data");					// Data
						if(bRet)
						{
							std::string value = node.child_value(); 
							// todo:去除Excel的CELL带有\r\n

							if(i>=(int)GetBaseRows())
								SetString(i,UINT(pnColIndices[j]),value.c_str()) ;	//填写
							bRet = node.moveto_parent();						// Cell
							nRealRow = i;
						}

						if((m_pNodeDesc->GetType() == Node_UnitAttrib) 
							&& (pnColIndices[j] == (LeafNode_Type-1)) && node.moveto_child("Comment"))
						{
							((cAttribLeafNode*)this)->ImportEnum(&node,i);
						}
					}
					else
					{
						int  test = -1;
					}
					bRet = node.moveto_next_sibling();						// Next Cell
					if(!bRet)
						break;
				}
				bRet = node.moveto_parent(); 
				bRet = node.moveto_next_sibling();
			}

			
			m_nRows = nRealRow + 1 - GetBaseRows();
			delete []pnColIndices;
		}
	}

	delete []pBits;
	return S_OK;
}

extern TCHAR * lpszBoolean[];
HRESULT cLeafNode_Imp::ExportToExcel(const TCHAR * lpszFile)const 
{
	std::locale loc = std::locale::global(std::locale(""));
	std::ofstream os(lpszFile, std::ios::trunc);
	std::locale::global(std::locale(loc )); 
	if (!os)
		return EEXCEL_FILEERR;

	pug::xml_parser xml;
	xml.create();
	pug::xml_node root = xml.document();
	pug::xml_node pi;
	pug::xml_node element,row,cell,data;
	
	////////////////////////////////////////////////////////////////////
	//模板

	// <?xml version="1.0"?>
	pi = root.append_child(pug::node_pi,_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	
	//<?mso-application progid="Excel.Sheet"?>
	pi = root.append_child(pug::node_pi,_T("mso-application"));
	pi.attribute(_T("progid")) = _T("Excel.Sheet");

	//<Workbook>
	element = root.append_child(pug::node_element,_T("Workbook"));
	element.append_attribute(_T("xmlns"),_T("urn:schemas-microsoft-com:office:spreadsheet")); 
	element.append_attribute(_T("xmlns:o"),_T("urn:schemas-microsoft-com:office:office")); 
	element.append_attribute(_T("xmlns:x"),_T("urn:schemas-microsoft-com:office:excel")); 
	element.append_attribute(_T("xmlns:ss"),_T("urn:schemas-microsoft-com:office:spreadsheet")); 
	element.append_attribute(_T("xmlns:html"),_T("http://www.w3.org/TR/REC-html40")); 

	//<Worksheet>
	element = element.append_child(pug::node_element,_T("Worksheet"));
	element.append_attribute(_T("ss:Name"),_T("Sheet1")); 
	
	//<Table>
	UINT nCols = m_pNodeDesc->GetFieldCount();
	if(m_pNodeDesc->GetType()==Node_UnitTemplate)
	{
		UINT nRealCols = 0;
		for(UINT i=0;i<nCols;i++)
		{
			const LeafField * pField = m_pNodeDesc->GetField(i); 
			if(pField->m_dwFlags & FIELD_TEMPLATE)
				nRealCols ++;
		}
		nCols = nRealCols;
	}

	element = element.append_child(pug::node_element,_T("Table"));
	element.append_attribute(_T("ss:ExpandedColumnCount"),(long)nCols);						// 列数
	element.append_attribute(_T("ss:ExpandedRowCount"),(long)GetRows()+1);						// 行数
	element.append_attribute(_T("x:FullColumns"),_T("1")); 
	element.append_attribute(_T("x:FullRows"),_T("1")); 
	element.append_attribute(_T("ss:DefaultColumnWidth"),_T("54")); 
	element.append_attribute(_T("ss:DefaultRowHeight"),_T("14.25")); 

	////////////////////////////////////////////////////////////////////
	//表头
	row = element.append_child(pug::node_element,_T("Row"));
	row.append_attribute(_T("ss:AutoFitHeight"),0l); 
	UINT nRealCol = 0;

	for(UINT i=0; i<m_pNodeDesc->GetFieldCount();i++)
	{
		const LeafField * pField = m_pNodeDesc->GetField(i);
		if((m_pNodeDesc->GetType()==Node_UnitTemplate) && !(pField->m_dwFlags & FIELD_TEMPLATE))
			continue;
		
		nRealCol ++;
		cell = row.append_child(pug::node_element,"Cell");
		
		data = cell.append_child(pug::node_element,"Data");
		data.attribute("ss:Type") = "String";
		data.set_child_value(pug::node_pcdata,pField->m_lpszName);    

		// 如果是枚举型，写入数据有效性。
		if((pField->m_eType == Field_Enumerate) || (pField->m_eType == Field_Boolean))
		{
			pug::xml_node _node = root.first_element_by_path("Workbook/Worksheet");
			pug::xml_node dv = _node.append_child(pug::node_element,"DataValidation");
			dv.attribute("xmlns") = "urn:schemas-microsoft-com:office:excel";
			pug::xml_node range = dv.append_child(pug::node_element,"Range");

			std::strstream streamRange;
			std::string strRange;
			streamRange << "C" <<  nRealCol; 
			streamRange >> strRange;

			range.set_child_value(pug::node_pcdata,strRange);
			pug::xml_node type = dv.append_child(pug::node_element,"Type");
			type.set_child_value(pug::node_pcdata,"List");
			dv.append_child(pug::node_element,"CellRangeList");
			pug::xml_node value = dv.append_child(pug::node_element,"Value");
			std::string strValue;
			strValue += "&quot;";
			
			if(pField->m_eType == Field_Enumerate)
			{
				if(pField->m_nMax > 0)
				{
					int n;
					for(n=0;n<(pField->m_nMax-pField->m_nMin-1);n++)
					{
						strValue += pField->m_ppStrings[n];
						strValue += ",";
					}
					strValue += pField->m_ppStrings[n];
				}
				else
				{
					strValue = _T("NULL");
				}
				
			}
			else
			{
				strValue += lpszBoolean[0];
				strValue += ",";
				strValue += lpszBoolean[1];
			}
			
			strValue += "&quot;";
			value.set_child_value(pug::node_pcdata,strValue);	
		}
	}

	////////////////////////////////////////////////////////////////////
	//数据
	for(UINT i=0; i<GetRows();i++)
	{
		row = element.append_child(pug::node_element,_T("Row"));
		row.append_attribute(_T("ss:AutoFitHeight"),0l); 
		for(UINT j=0;j<m_pNodeDesc->GetFieldCount();j++)
		{
			const LeafField * pField = m_pNodeDesc->GetField(j);
			if((m_pNodeDesc->GetType()==Node_UnitTemplate) && !(pField->m_dwFlags & FIELD_TEMPLATE))
				continue;

			cell = row.append_child(pug::node_element,"Cell");
			
			data = cell.append_child(pug::node_element,"Data");
			if( (m_pNodeDesc->GetField(j)->m_eType == Field_Integer) || 
				(m_pNodeDesc->GetField(j)->m_eType == Field_Percent) ||
				(m_pNodeDesc->GetField(j)->m_eType == Field_Dynamic) ||
				(m_pNodeDesc->GetField(j)->m_eType == Field_Float))
				data.attribute("ss:Type") = _T("Number");
			else if(m_pNodeDesc->GetField(j)->m_eType == Field_Boolean)
				data.attribute("ss:Type") = _T("Boolean");
			else // Field_String,Field_Enumerate,Field_IdField
				data.attribute("ss:Type") = _T("String");				
			data.set_child_value(pug::node_pcdata,GetString(i,j));    

			// 特殊
			if( (m_pNodeDesc->GetType() == Node_UnitAttrib) 
				&& (j == (LeafNode_Type-1)) && ((FieldType)GetInteger(i,j) == Field_Enumerate))
			{
				((cAttribLeafNode*)this)->ExportEnum(&cell,i); 
			}
		}
	}

	std::strstream str; 
	str << root;
	char * pBits = str.str(); 
	int size = (int)str.pcount(); 
	//os.write(pBits,size); 
	//return S_OK;
	
	// 转换编码
	{
		int uSize = ::MultiByteToWideChar(CP_ACP,0,pBits,size,NULL,0);
		wchar_t * pUstr = new wchar_t[uSize+1];
		ZeroMemory(pUstr,sizeof(wchar_t)*(uSize+1));
		::MultiByteToWideChar(CP_ACP,0,pBits,size+1,pUstr,uSize);	//pBits->>pUstr

		size = ::WideCharToMultiByte(CP_UTF8,0,pUstr,-1,NULL,0,NULL,NULL);
		pBits = new char[size + 1];
		ZeroMemory(pBits,sizeof(char)*(size+1));
		::WideCharToMultiByte(CP_UTF8,0,pUstr,-1,pBits,size,NULL,NULL);//pUstr->>pBits

		delete []pUstr;
	}

	os.write(pBits,size); 
	delete []pBits;

	return S_OK;
}


HRESULT cLeafNode_Imp::ExportNode(const TCHAR * lpszFile,int nFields,int * pnFields) const
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
		pug::xml_node node = root.append_child(pug::node_element,_T("Node"));
		node.append_attribute(_T("Name"),GetName());
		
		pug::xml_node enums = node.append_child(pug::node_element,_T("Enums"));
		for(int j=0;j<nFields;j++)
		{
			const LeafField * f = m_pNodeDesc->GetField(pnFields[j]);
			if(f->m_eType == Field_Enumerate)
			{
				pug::xml_node Enum = enums.append_child(pug::node_element,_T("Enum"));
				Enum.append_attribute(_T("Name"),f->m_lpszName);
				if(f->m_nMax > 0)
				{
					for(int n=0;n<(f->m_nMax-f->m_nMin);n++)
					{
						pug::xml_node item = Enum.append_child(pug::node_element,_T("Item"));
						item.append_attribute(_T("Name"),f->m_ppStrings[n]);
					}
				}
			}
		}

		pug::xml_node rows = node.append_child(pug::node_element,_T("Rows"));
		UINT nRows = GetRows();
		node.append_attribute(_T("Count"),(long)nRows);
		for(UINT i=0;i<nRows;i++)
		{
			pug::xml_node row = rows.append_child(pug::node_element,_T("Row"));
			for(int j=0;j<nFields;j++)
			{
				const LeafField * f = m_pNodeDesc->GetField(pnFields[j]);
				if(f->m_nTemplateOffset>=0)
				{
					pug::xml_node field = row.append_child(pug::node_element,_T("Field"));
					field.append_attribute(_T("Name"),f->m_lpszName);
					field.append_attribute(_T("Alias"),f->m_lpszAlias);
					
					if(f->m_eType != Field_Enumerate)
					{
						const TCHAR *lpszValue = GetString(i,pnFields[j]);
						if(lpszValue)
						{
							field.set_child_value(pug::node_pcdata,lpszValue);  
						}
						else
						{
							field.set_child_value(pug::node_pcdata,_T(""));  
						}
					}
					else
					{
						TCHAR szNum[64];
						int nValue = GetInteger(i,pnFields[j]);
						field.set_child_value(pug::node_pcdata,_itot(nValue,szNum,10));  
					}
				}
			}
		}
		os << root;
		return S_OK;
	}	
	return S_FALSE;	
}

void cLeafNode_Imp::SetRows(UINT nRow)
{
	ValidRow(nRow,TRUE);
	m_nRows = nRow;
}

void cLeafNode_Imp::ValidRow(UINT nRow,BOOL bAllocMore)
{
	if(nRow > m_nRows)
	{
		if(nRow > m_nPreAllocRows)
		{
			UINT nPreAllocRows = m_nPreAllocRows;
			if(bAllocMore)
				nPreAllocRows = ((nRow/ 16) + 1) * 16;
			else
				nPreAllocRows = nRow;

			UINT nLen = m_pNodeDesc->GetRowLength();  
			BYTE * pBuffer = new BYTE[nPreAllocRows * nLen];
			cLeafRow_Imp * pRows = new cLeafRow_Imp[nPreAllocRows];
			ZeroMemory(pBuffer, sizeof(BYTE)*nPreAllocRows*nLen);

#ifdef _DEBUG
			UINT nFields = m_pNodeDesc->GetFieldCount();  
			FieldValue * pFieldValues = new FieldValue[nPreAllocRows*nFields];
			ZeroMemory(pFieldValues, sizeof(FieldValue)*nPreAllocRows*nFields);
#endif
			if(m_nRows)
			{
				MoveMemory(pBuffer,m_pBuffer,sizeof(BYTE)*m_nPreAllocRows*nLen);
				delete []m_pBuffer;
				delete []m_pRows;
#ifdef _DEBUG
				delete []m_pFieldValues;
#endif
			}
			for(UINT i=0;i<nPreAllocRows;i++)
			{
#ifdef _DEBUG
				pRows[i].Attach(m_pNodeDesc, pBuffer+i*nLen, pFieldValues+i*nFields); 
				for(UINT j=0;j<nFields;j++)
				{
					const LeafField * pField = m_pNodeDesc->GetField(j);
					if(pField->m_nTemplateOffset >= 0)
					{
						(pFieldValues+i*nFields+j)->fv = pField->m_lpszName;
						switch(m_pNodeDesc->GetField(j)->m_eType)
						{
						case Field_String:
							(pFieldValues+i*nFields+j)->s = (TCHAR *)(pBuffer+i*nLen + pField->m_nTemplateOffset);
							break;
						case Field_Enumerate:
						case Field_Integer:
						case Field_Dynamic:
						case Field_Boolean:
						case Field_Percent:
						case Field_IdField:
							(pFieldValues+i*nFields+j)->n = (int *)(pBuffer+i*nLen + pField->m_nTemplateOffset);
							break;
						case Field_Float:
							(pFieldValues+i*nFields+j)->f = (float *)(pBuffer+i*nLen + pField->m_nTemplateOffset);
							break;
						}
					}
				}
#else
				pRows[i].Attach(m_pNodeDesc, pBuffer+i*nLen); 
#endif
			}
			m_nPreAllocRows = nPreAllocRows;
			m_pBuffer = pBuffer;
			m_pRows = pRows;
#ifdef _DEBUG
			m_pFieldValues = pFieldValues;
#endif
		}	
		m_nRows = nRow;
	}
}

UINT cLeafNode_Imp::FindRowByID(int nValue)const 
{
	UINT nIDField = m_pNodeDesc->GetIDField();
	if(nIDField != -1)
		return FindRow(nIDField,nValue);
	return (UINT)-1;
}

UINT cLeafNode_Imp::FindRow(UINT nField,int nValue)const 
{
	if(m_pKeys && nField == m_pKeys[0].nCols)	// 只有第一列索引可以完整查找行
	{
		cQuery q(nValue);
		ResultRows rr = Query(q);
		return rr.nCount?rr.nFirst:-1;
	}

	for(UINT i=0; i<m_nRows;i++)
	{
		if(GetInteger(i,nField) == nValue)
			return i;
	}
	return (UINT)-1;
}

UINT cLeafNode_Imp::FindRow(const TCHAR * lpcstrField,int nValue)const 
{
	UINT nField = m_pNodeDesc->FindField(lpcstrField);
	if(nField != (UINT)-1)
	{
		return FindRow(nField,nValue);
	}
	return (UINT)-1;
}
UINT cLeafNode_Imp::FindRow(UINT nField,const TCHAR * lpcstrValue)const 
{
	// 米威修改:主要原因是下面从0行开始比较某列的字符串,但是以前用的是_tcsncmp,
	// 假如逻辑引擎某表中有'蛇精'(ID=0)和'蛇'(ID=100)那么你用按下面查找'蛇'
	// 就一直找到的是蛇镜子.;

	/*
	size_t len = lstrlen(lpcstrValue);
	for(UINT i=0; i<m_nRows;i++)
	{
		const TCHAR * lpszName = GetString(i,nField);
		if(!_tcsncmp(lpszName,lpcstrValue,len))
			return i;
	}
	return (UINT)-1;
	*/
	for (UINT i = 0; i < m_nRows; i++)
	{
		const TCHAR * lpszName = GetString(i,nField);
		if (!_tcscmp(lpszName, lpcstrValue))
		{
			return i;
		}
	}
	return -1;
}
UINT cLeafNode_Imp::FindRow(const TCHAR * lpcstrField,const TCHAR * lpcstrValue)const 
{
	UINT nField = m_pNodeDesc->FindField(lpcstrField);
	if(nField != (UINT)-1)
	{
		return FindRow(nField,lpcstrValue);
	}
	return (UINT)-1;
}


static UINT g_nKeyCols[MAX_INDEX_FIELD] = {0};
static UINT g_nKeys = 0;
int RowCompare(const void * r1, const void * r2)
{
	int nRet = 0;
	const cLeafRow_Imp *row1 = (const cLeafRow_Imp *)r1;
	const cLeafRow_Imp *row2 = (const cLeafRow_Imp *)r2;

	for(UINT x=0;x<g_nKeys;x++)
	{
		int n1 = row1->GetInteger(g_nKeyCols[x]);
		int n2 = row2->GetInteger(g_nKeyCols[x]);
		nRet = n1 - n2;
		if(n1 != n2)	// 只有相等的情况下，才需要继续比较下个字段
			break;
	}
	return nRet;
}

void cLeafNode_Imp::BuildIndex()
{
	// 没有数据的不用建立索引
	if(!m_nRows)
		return;

	// 统计索引域
	UINT n = 0,nKeyCols[MAX_INDEX_FIELD];
	for(UINT i=0;i<m_pNodeDesc->GetFieldCount();i++)
	{
		const LeafField * f = m_pNodeDesc->GetField(i);
		if(f->m_nKeyOrder>0)
		{
			nKeyCols[n] = i;
			n ++;
		}
	}
	if( n == 0 )
	{
		UINT nIDField = m_pNodeDesc->GetIDField();
		if(nIDField == -1)
			return;
		nKeyCols[0] = nIDField;
		n ++;
	}

	// 把数据按照索引域的顺序排序(冒泡排序法)
	g_nKeys = n;
	CopyMemory(g_nKeyCols,nKeyCols,sizeof(UINT)*n);
	qsort(m_pRows,m_nRows,sizeof(cLeafRow_Imp),RowCompare);

	// 释放原来的索引
	for(UINT i=0;i<m_nIndices;i++)
	{
		if(m_ppIndices && m_ppIndices[i])
			delete []m_ppIndices[i]; 
	}
	delete []m_ppIndices;
	delete []m_pKeys;

	// 只有1列索引，是不用建立的。
	m_pKeys = new Key[n];
	ZeroMemory(m_pKeys,sizeof(Key)*n);
	m_pKeys[n-1].nCols = nKeyCols[n-1];
	if(n <= 1)
		return;
	
	// 建立索引 倒数行不一定安全。列数据不一定是完全的。
	m_nIndices = n-1;
	m_ppIndices = new Index * [m_nIndices];
	Index * pIndices = new Index[m_nRows];
	for(UINT x=0;x<m_nIndices;x++)
	{
		ZeroMemory(pIndices,sizeof(Index) * m_nRows);
		m_pKeys[x].nCols = nKeyCols[x];

		UINT nIndices = (x==0)?1:m_pKeys[x-1].nIndices;  
		for(UINT y=0;y<nIndices;y++)
		{
			int nBeginRow = (x==0)?0:m_ppIndices[x-1][y].nStart;
			int nLast = GetInteger(nBeginRow,nKeyCols[x]);
			pIndices[m_pKeys[x].nIndices].nValue = nLast;
			pIndices[m_pKeys[x].nIndices].nStart = nBeginRow;
			pIndices[m_pKeys[x].nIndices].nKey = x;
			
			UINT nRows = (x==0)?m_nRows:m_ppIndices[x-1][y].nCount;  
			for(UINT i=nBeginRow;i<nBeginRow+nRows;i++)
			{
				int nRow = i;
				int n1 = GetInteger(nRow,nKeyCols[x]);
				if( nLast != n1 )
				{
					nLast = n1;
					m_pKeys[x].nIndices ++;
					pIndices[m_pKeys[x].nIndices].nValue = n1;
					pIndices[m_pKeys[x].nIndices].nStart = i;
					pIndices[m_pKeys[x].nIndices].nKey = x;
				}
				pIndices[m_pKeys[x].nIndices].nCount++;
			}
			m_pKeys[x].nIndices ++;
		}
		
		m_ppIndices[x] = new Index[m_pKeys[x].nIndices];
		CopyMemory(m_ppIndices[x],pIndices,sizeof(Index) * m_pKeys[x].nIndices);
	}
	delete []pIndices;

	// 把行兑换成索引的偏移量
	if(n<=2)
		return;
	for(UINT x=0;x<m_nIndices-1;x++)
	{
		for(UINT y=0;y<m_pKeys[x].nIndices;y++)
		{
			BOOL bFound = FALSE; 
			for(UINT z=0;z<m_pKeys[x+1].nIndices;z++)
			{
				if(m_ppIndices[x+1][z].nStart == m_ppIndices[x][y].nStart)
				{
					m_ppIndices[x][y].nIndexFirst = z;
					bFound = TRUE;
				}
			}

			if(bFound)
			{
				UINT nCount = 0;
				for(UINT z=m_ppIndices[x][y].nIndexFirst;z<m_pKeys[x+1].nIndices;z++)
				{
					nCount += m_ppIndices[x+1][z].nCount;
					if(nCount >= m_ppIndices[x][y].nCount)
					{
						m_ppIndices[x][y].nIndexNum = z - m_ppIndices[x][y].nIndexFirst + 1;
						break;
					}
				}
			}					
		}		
	}
}

ResultRows cLeafNode_Imp::Query(cQuery q,BOOL bLastResult)const 
{
	ResultRows r;
	r.nKey = 0;
	r.nFirst = (UINT)-1;
	r.nCount = 0;
	
	UINT nQCount = q.GetCount();
	if(!nQCount)
		return r;
	
	// 无索引
	if(m_nRows)
	{
		assert(m_pKeys);
	}
	//if(!m_pKeys)
	//	BuildIndex(); 
	
	// 字段数量是否符合
	if(nQCount <= m_nIndices+1)
	{	
		// 先在索引中寻找
		r.nCount = m_nIndices?m_pKeys[0].nIndices:0;
		for(UINT x=0;x<m_nIndices;x++)
		{
			int nCurPos = DichotomySearch(x,r.nCount,r.nFirst,q.GetValue(x));
			if(nCurPos<0)	// 没找到
			{
				r.nKey = 0;
				r.nFirst = (UINT)-1;
				r.nCount = 0;
				return r;
			}
			
			if((x == m_nIndices-1) || (bLastResult&&(x == (nQCount-1)))) // 最后一个索引，或者条件数量用完了
			{
				r.nKey = 0;
				r.nCount = m_ppIndices[x][nCurPos].nCount;  
				r.nFirst = m_ppIndices[x][nCurPos].nStart; 
			}
			else
			{
				r.nKey = x + 1;
				r.nCount = m_ppIndices[x][nCurPos].nIndexNum;  
				r.nFirst = m_ppIndices[x][nCurPos].nIndexFirst; 
			}
			if(x == (nQCount-1))
				return r;	// 条件数量用完了
			r.nFirst--;
		}
		
		// 再在记录中找
		if(r.nCount == 0)
			r.nCount = m_nRows;

		// 用 nCount 和 Start 查找需要的值
		int nCurPos = DichotomySearch(m_nIndices+1,r.nCount,r.nFirst,q.GetValue(m_nIndices));
		if(nCurPos<0)
		{
			r.nKey = 0;
			r.nFirst = (UINT)-1;
			r.nCount = 0;
			return r;
		}
		r.nKey = 0;
		r.nCount = 1;
		r.nFirst = nCurPos;
	}
	return r;
}

const cLeafRow * cLeafNode_Imp::Result(ResultRows * r,UINT n)const 
{
	if(r == NULL)
		return NULL;

	if(r->nCount <= n)
		return NULL;

	// 结果集中是行号
	if(r->nKey ==0)
		return GetRow(r->nFirst + n);

	// 结果集中是索引
	if( (r->nKey < m_nIndices) && (r->nFirst < m_pKeys[r->nKey].nIndices) && (n<r->nCount) )
		return GetRow(m_ppIndices[r->nKey][r->nFirst + n].nStart);  

	return NULL;
}

// 1.值的类型 2.当前位置与目标值的比较
int cLeafNode_Imp::DichotomySearch(UINT nWhich, int nArrayCount,int nStart,int nValue)const 
{
	int nStartPos, nEndPos, nCurPos, nResult;
	nStartPos = nStart;
	nEndPos = nArrayCount+nStart+1;
	nCurPos = (nEndPos+nStartPos)/2;
	while( (nCurPos > nStartPos) && (nCurPos < nEndPos) )
	{
		if(nWhich<m_nIndices)
			nResult = m_ppIndices[nWhich][nCurPos].nValue - nValue;				// 索引
		else
			nResult = GetInteger(nCurPos,m_pKeys[m_nIndices].nCols) - nValue;	// 数据表
		if (nResult<0)
		{
			nStartPos = nCurPos;
			nCurPos = (nEndPos+nStartPos)/2;
		}
		else if (nResult>0)
		{
			nEndPos = nCurPos;
			nCurPos = (nEndPos+nStartPos)/2;
		}
		else 
		{
			return nCurPos;
		}
	}
	return -1;
}

cLeafNodeDesc * cLeafNode_Imp::FindNodeDesc()
{
	if(m_pNodeDesc->GetType()==Node_UnitTemplate)
	{
		cTreeNode * pParent = (cTreeNode*)m_pParent;
		for(UINT n=0;n<pParent->GetChildren();n++)
		{
			// 寻找依据是相同父节点的 Node_UnitAttrib 节点
			cBaseNode * pNode = pParent->GetChildNode(n);
			if(pNode->GetDesc()->GetType() == Node_UnitAttrib)
				return ((cAttribLeafNode *)pNode)->GetTargetDesc();
		}
	}
	else if(m_pNodeDesc->GetType()==Node_ContactData)
	{
		cTreeNode * pParent = (cTreeNode*)m_pParent;
		for(UINT n=0;n<pParent->GetChildren();n++)
		{
			// 寻找依据是相同父节点的 Node_UnitAttrib 节点
			cBaseNode * pNode = pParent->GetChildNode(n);
			if(pNode->GetDesc()->GetType() == Node_ContactAttrib)
				return ((cContactAttribLeafNode *)pNode)->GetTargetDesc();
		}
	}

	return NULL;
}

void cLeafNode_Imp::RefreshUnitTemplateNode()
{
	// 空表刷新优化
	if(m_nRows == 0)
	{
		((cTemplateLeafNodeDesc *)m_pNodeDesc)->Build();
		return;
	}
	
	// 非空表刷新
	// 先保存到 xml 节点
	pug::xml_parser xml;
	xml.create();
	pug::xml_node root = xml.document();
	pug::xml_node pi = root.append_child(pug::node_pi,_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");

	pug::xml_node node = root.append_child(pug::node_element,_T("Node"));
	node.append_attribute(_T("Rows"),(long)m_nRows);
	for(UINT i=0;i<m_nRows;i++)
	{
		pug::xml_node row = node.append_child(pug::node_element,_T("Row"));
		for(UINT j=0;j<m_pNodeDesc->GetFieldCount();j++)
		{
			const LeafField * f = m_pNodeDesc->GetField(j);
			pug::xml_node field = row.append_child(pug::node_element,_T("Field"));
			field.append_attribute(_T("Name"),f->m_lpszName);
			
			switch(f->m_eType)
			{
			case Field_String:
			case Field_Float:
			case Field_Integer:
			case Field_Boolean:
			case Field_Percent:
			case Field_IdField:
				{
					const TCHAR * pValue = GetString(i,j);
					field.append_attribute(_T("Type"),0l);		// 字符串
					field.append_attribute(_T("Value"),pValue);  
				}
				break;
			case Field_Enumerate:
				{
					long nValue = GetInteger(i,j);
					field.append_attribute(_T("Type"),1l);		// 整数值
					field.append_attribute(_T("Value"),nValue);  
				}
				break;
			}
		}
	}

	// 删除以前的内存
	delete []m_pBuffer;
	delete []m_pRows;
	for(UINT i=0;i<m_nIndices;i++)
	{
		if(m_ppIndices[i])
			delete []m_ppIndices[i]; 
	}
	delete []m_ppIndices;
	delete []m_pKeys;
	m_pBuffer = NULL;
	m_pRows = NULL;
	m_ppIndices = NULL;
	m_pKeys = NULL;
	m_nRows = 0;
	m_nPreAllocRows =  0;
#ifdef _DEBUG
	delete []m_pFieldValues;
	m_pFieldValues = NULL;
#endif
	
	// 重新建立节点描述
	((cTemplateLeafNodeDesc *)m_pNodeDesc)->Build();

	// 从 XML 节点恢复数据
	UINT nRows = (long)node.attribute(_T("Rows"));
	pug::xml_node row = node.first_element_by_name(_T("Row"));
	ValidRow(nRows,FALSE); 	
	for(UINT i=0;i<nRows;i++)
	{
		pug::xml_node field = row.first_element_by_name(_T("Field"));
		if(!field.empty())
		{
			do
			{			
				const TCHAR * lpszName = (const TCHAR *)field.attribute(_T("Name"));
				long nType = (long)field.attribute(_T("Type"));
				if(nType == 0)
				{
					const TCHAR * lpszValue = (const TCHAR *)field.attribute(_T("Value"));
					SetString(i,lpszName,lpszValue); 
				}
				else if(nType == 1)
				{
					long nValue = (long)field.attribute(_T("Value"));
					SetInteger(i,lpszName,nValue); 
				}
			}
			while(field.moveto_next_sibling());
		}
		if(!row.moveto_next_sibling())
			break;
	}

	// 重新建立索引
	BuildIndex();  
}

void cLeafNode_Imp::RefreshContactDataNode()
{
	// 空表刷新优化
	if(m_nRows == 0)
	{
		((cContactDataLeafNodeDesc *)m_pNodeDesc)->Build();
		return;
	}
	
	// 非空表刷新
	// 先保存到 xml 节点
	pug::xml_parser xml;
	xml.create();
	pug::xml_node root = xml.document();
	pug::xml_node pi = root.append_child(pug::node_pi,_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");

	pug::xml_node node = root.append_child(pug::node_element,_T("Node"));
	node.append_attribute(_T("Rows"),(long)m_nRows);
	for(UINT i=0;i<m_nRows;i++)
	{
		pug::xml_node row = node.append_child(pug::node_element,_T("Row"));
		for(UINT j=0;j<m_pNodeDesc->GetFieldCount();j++)
		{
			const LeafField * f = m_pNodeDesc->GetField(j);
			pug::xml_node field = row.append_child(pug::node_element,_T("Field"));
			field.append_attribute(_T("Name"),f->m_lpszName);
			
			switch(f->m_eType)
			{
			case Field_String:
			case Field_Float:
			case Field_Integer:
			case Field_Boolean:
			case Field_Percent:
			case Field_IdField:
				{
					const TCHAR * pValue = GetString(i,j);
					field.append_attribute(_T("Type"),0l);		// 字符串
					field.append_attribute(_T("Value"),pValue);  
				}
				break;
			case Field_Enumerate:
				{
					long nValue = GetInteger(i,j);
					field.append_attribute(_T("Type"),1l);		// 整数值
					field.append_attribute(_T("Value"),nValue);  
				}
				break;
			}
		}
	}

	// 删除以前的内存
	delete []m_pBuffer;
	delete []m_pRows;
	for(UINT i=0;i<m_nIndices;i++)
	{
		if(m_ppIndices[i])
			delete []m_ppIndices[i]; 
	}
	delete []m_ppIndices;
	delete []m_pKeys;
	m_pBuffer = NULL;
	m_pRows = NULL;
	m_ppIndices = NULL;
	m_pKeys = NULL;
	m_nRows = 0;
	m_nPreAllocRows =  0;
#ifdef _DEBUG
	delete []m_pFieldValues;
	m_pFieldValues = NULL;
#endif
	
	// 重新建立节点描述
	((cContactDataLeafNodeDesc *)m_pNodeDesc)->Build();

	// 从 XML 节点恢复数据
	UINT nRows = (long)node.attribute(_T("Rows"));
	pug::xml_node row = node.first_element_by_name(_T("Row"));
	ValidRow(nRows,FALSE); 	
	for(UINT i=0;i<nRows;i++)
	{
		pug::xml_node field = row.first_element_by_name(_T("Field"));
		if(!field.empty())
		{
			do
			{			
				const TCHAR * lpszName = (const TCHAR *)field.attribute(_T("Name"));
				long nType = (long)field.attribute(_T("Type"));
				if(nType == 0)
				{
					const TCHAR * lpszValue = (const TCHAR *)field.attribute(_T("Value"));
					SetString(i,lpszName,lpszValue); 
				}
				else if(nType == 1)
				{
					long nValue = (long)field.attribute(_T("Value"));
					SetInteger(i,lpszName,nValue); 
				}
			}
			while(field.moveto_next_sibling());
		}
		if(!row.moveto_next_sibling())
			break;
	}

	// 重新建立索引
	BuildIndex();  
}

void cLeafNode_Imp::OnAttribDefChanged()
{
	if(m_pNodeDesc->GetType()==Node_UnitTemplate)
	{
		RefreshUnitTemplateNode();
	}
	else
	{
		RefreshContactDataNode();
	}
}

BOOL cLeafNode_Imp::LoadLeafGridFormat(TCHAR *szPath)
{
	TCHAR szFileName[MAX_PATH] = {0};
	_tcscpy(szFileName,szPath);
	TCHAR * lpszEnd = &szFileName[_tcslen(szFileName)];
	TCHAR * lpszFileExt = ::PathFindExtension(szFileName);
	memset(lpszFileExt,0x0,sizeof(TCHAR)*(lpszEnd-lpszFileExt));
	_tcscat(szFileName,_T("_format\\"));
	// 如果目录不存在,需要创建新目录
	if(!PathFileExists(szFileName) || !PathIsDirectory(szFileName))
	{
		if(!CreateDirectory(szFileName,NULL))
		{
			TCHAR szBuf[MAX_PATH];
			_stprintf(szBuf,_T("无法创建目录 %s"),szFileName);
			MessageBox(NULL,szBuf,_T("衰吧"),MB_OK);
			return FALSE;
		}
	}
	const TCHAR *lpszFile = GetPath(&szFileName[_tcslen(szFileName)],MAX_PATH);
	lpszFile = MkPathByName(const_cast<TCHAR *>(lpszFile));
	_tcscat(szFileName,_T(".xml"));
	
	lpszFile = &szFileName[0];
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
			pug::xml_node top = root.first_element_by_name(ROOT);
			if(top.empty())
				return FALSE;

			//格式字段
			{
				pug::xml_node styles = top.first_element_by_name(STYLES);
				if(styles.empty())
					return FALSE;
				int nCnt = (long)styles.attribute(COUNT);
				if (nCnt > 0)
				{
					if (!m_mapFormat.empty())
					{
						m_mapFormat.clear();
					}
					
					pug::xml_node style = styles.first_element_by_name(STYLE);
					for (int i=0;i<nCnt;i++)
					{
						struct FiledFormat ff;
						ff.nID = (long)style.attribute(ID);
						ff.font.dwBackColor = (long)style.attribute(BGCOLOR);
						ff.font.szFontName = style.attribute(FONTNAME).getvalue();
						ff.font.nFontSize = (long)style.attribute(FONTSIZE_);
						ff.font.dwForeColor = (long)style.attribute(FRCOLOR);
						ff.font.bFontBold = (bool)style.attribute(FONTBOLD);
						ff.font.bFontItalic = (bool)style.attribute(FONTITALIC);
						ff.font.bFontUnderline = (bool)style.attribute(FONTUNDERLINE);
						ff.font.bFontStrikethru = (bool)style.attribute(FONTSTRIKETHRU);

						m_mapFormat[ff.nID] = ff;
						if(!style.moveto_next_sibling())
						{
							break;
						}
					}
				}
			}




			//行说明字段
			{
				pug::xml_node desces = top.first_element_by_name(DESCES);
				if(desces.empty())
					return FALSE;
				int nCnt = (long)desces.attribute(COUNT);
				if (nCnt > 0)
				{
					if (!m_mapRowDesc.empty())
					{
						m_mapRowDesc.clear();
					}
					pug::xml_node desc = desces.first_element_by_name(DESC);
					for (int i=0;i<nCnt;i++)
					{
						int nID = (long)desc.attribute(ID);
						std::string comment = desc.attribute(COMMENT).getvalue();
						
						m_mapRowDesc[nID] = comment;
						if(!desc.moveto_next_sibling())
						{
							break;
						}
					}
				}
			}


			//列宽格式
			{
				pug::xml_node columes = top.first_element_by_name(COLUMES);
				if(columes.empty())
					return FALSE;
				int nCnt = (long)columes.attribute(COUNT);
				if (nCnt > 0)
				{
					if (!m_mapColWidth.empty())
					{
						m_mapColWidth.clear();
					}
					pug::xml_node colume = columes.first_element_by_name(COLUME);
					for (int i=0;i<nCnt;i++)
					{
						int nID = (long)colume.attribute(ID);
						int nWidthD = (long)colume.attribute(WIDTH);

						m_mapColWidth[nID] = nWidthD;
						if(!colume.moveto_next_sibling())
						{
							break;
						}
					}
				}
			}

			//行高格式
			{
				pug::xml_node rowes = top.first_element_by_name(ROWES);
				if(rowes.empty())
					return FALSE;
				int nCnt = (long)rowes.attribute(COUNT);
				if (nCnt > 0)
				{
					if (!m_mapRowHeight.empty())
					{
						m_mapRowHeight.clear();
					}
					pug::xml_node row = rowes.first_element_by_name(ROW);
					for (int i=0;i<nCnt;i++)
					{
						int nID = (long)row.attribute(ID);
						int nHeight = (long)row.attribute(HEIGHT);

						m_mapRowHeight[nID] = nHeight;
						if(!row.moveto_next_sibling())
						{
							break;
						}
					}
				}
			}

			//单元格格式
			{
				pug::xml_node celles = top.first_element_by_name(CELLES);
				if(celles.empty())
					return FALSE;
				int nCnt = (long)celles.attribute(COUNT);
				if (nCnt > 0)
				{
					if (!m_mapFieldType.empty())
					{
						m_mapFieldType.clear();
					}
					pug::xml_node cell = celles.first_element_by_name(CELL);
					for (int i=0;i<nCnt;i++)
					{
						int nID = (long)cell.attribute(ID);
						int nStyleID = (long)cell.attribute(STYLEID);

						m_mapFieldType[nID] = nStyleID;
						if(!cell.moveto_next_sibling())
						{
							break;
						}
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL cLeafNode_Imp::SaveLeafGridFormat(TCHAR *szPath)
{
	TCHAR szFileName[MAX_PATH] = {0};
	_tcscpy(szFileName,szPath);
	TCHAR * lpszEnd = &szFileName[_tcslen(szFileName)];
	TCHAR * lpszFileExt = ::PathFindExtension(szFileName);
	memset(lpszFileExt,0x0,sizeof(TCHAR)*(lpszEnd-lpszFileExt));
	_tcscat(szFileName,_T("_format\\"));
	// 如果目录不存在,需要创建新目录
	if(!PathFileExists(szFileName) || !PathIsDirectory(szFileName))
	{
		if(!CreateDirectory(szFileName,NULL))
		{
			TCHAR szBuf[MAX_PATH];
			_stprintf(szBuf,_T("无法创建目录 %s"),szFileName);
			MessageBox(NULL,szBuf,_T("衰吧"),MB_OK);
			return FALSE;
		}
	}
	const TCHAR *lpszFile = GetPath(&szFileName[_tcslen(szFileName)],MAX_PATH);
	lpszFile = MkPathByName(const_cast<TCHAR *>(lpszFile));
	_tcscat(szFileName,_T(".xml"));

	lpszFile = &szFileName[0];

	
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

		pug::xml_node top = root.append_child(pug::node_element,ROOT);
		{
			//格式字段
			{			
				pug::xml_node styles = top.append_child(pug::node_element,STYLES);
				int nCnt = m_mapFormat.size();
				styles.append_attribute(COUNT,(long)nCnt);
				if (nCnt > 0)
				{
					std::map<UINT,struct FiledFormat>::iterator beg = m_mapFormat.begin();
					for (;beg!=m_mapFormat.end();beg++)
					{
						pug::xml_node style = styles.append_child(pug::node_element,STYLE);
						struct FiledFormat *pFf = &(beg->second);
						{
							style.append_attribute(ID,(long)pFf->nID);
							style.append_attribute(BGCOLOR,(long)pFf->font.dwBackColor);
							style.append_attribute(FONTNAME,(const TCHAR *)pFf->font.szFontName.c_str());
							style.append_attribute(FONTSIZE_,(long)pFf->font.nFontSize);
							style.append_attribute(FRCOLOR,(long)pFf->font.dwForeColor);
							if (pFf->font.bFontBold)
							{
								style.append_attribute(FONTBOLD,1L);
							}
							if (pFf->font.bFontItalic)
							{
								style.append_attribute(FONTITALIC,1L);
							}
							if (pFf->font.bFontUnderline)
							{
								style.append_attribute(FONTUNDERLINE,1L);
							}
							if (pFf->font.bFontStrikethru)
							{
								style.append_attribute(FONTSTRIKETHRU,1L);
							}			

						}
					}
				}
			}

			{			
				//行说明字段
				pug::xml_node desces = top.append_child(pug::node_element,DESCES);
				int nCnt = m_mapRowDesc.size();
				desces.append_attribute(COUNT,(long)nCnt);
				if (nCnt > 0)
				{
					std::map<UINT,std::string>::iterator beg = m_mapRowDesc.begin();
					for (;beg!=m_mapRowDesc.end();beg++)
					{
						pug::xml_node desc = desces.append_child(pug::node_element,DESC);
						desc.append_attribute(ID,(long)beg->first);
						desc.append_attribute(COMMENT,beg->second);
					}
				}
			}


			{			
				//列宽字段
				pug::xml_node columes = top.append_child(pug::node_element,COLUMES);
				int nCnt = m_mapColWidth.size();
				columes.append_attribute(COUNT,(long)nCnt);
				if (nCnt > 0)
				{
					std::map<UINT,DWORD>::iterator beg = m_mapColWidth.begin();
					for (;beg!=m_mapColWidth.end();beg++)
					{
						pug::xml_node colume = columes.append_child(pug::node_element,COLUME);
						colume.append_attribute(ID,(long)beg->first);
						colume.append_attribute(WIDTH,(long)beg->second);
					}
				}
			}

			{			
				//行高字段
				pug::xml_node rowes = top.append_child(pug::node_element,ROWES);
				int nCnt = m_mapRowHeight.size();
				rowes.append_attribute(COUNT,(long)nCnt);
				if (nCnt > 0)
				{
					std::map<UINT,DWORD>::iterator beg = m_mapRowHeight.begin();
					for (;beg!=m_mapRowHeight.end();beg++)
					{
						pug::xml_node row = rowes.append_child(pug::node_element,ROW);
						row.append_attribute(ID,(long)beg->first);
						row.append_attribute(HEIGHT,(long)beg->second);
					}
				}
			}

			{			
				//单元格格式
				pug::xml_node celles = top.append_child(pug::node_element,CELLES);
				int nCnt = m_mapFieldType.size();
				celles.append_attribute(COUNT,(long)nCnt);
				if (nCnt > 0)
				{
					std::map<UINT,UINT>::iterator beg = m_mapFieldType.begin();
					for (;beg!=m_mapFieldType.end();beg++)
					{
						pug::xml_node cell = celles.append_child(pug::node_element,CELL);
						cell.append_attribute(ID,(long)beg->first);
						cell.append_attribute(STYLEID,(long)beg->second);
					}
				}
			}
		}

		os << root;

		return TRUE;
	}

	return FALSE;	
}

TCHAR * cLeafNode_Imp::MkPathByName(TCHAR * lpName)
{
	int nLen = _tcslen(lpName);
	while(nLen)
	{
		if (lpName[nLen] == _T('/'))
		{
			lpName[nLen] = _T('_');
		}
		nLen--;
	}
	return &lpName[0];
}

cTemplateLeafNodeDesc::cTemplateLeafNodeDesc(cNodeDesc * pDesc,cLeafNode * pSource,cLeafNode * pBase)
	:cLeafNodeDesc_Imp(pDesc->GetType(),(TCHAR *)pDesc->GetDescName(),pDesc->IsFiled(),pDesc->IfExclusive(),NULL)   
	,m_pBaseDesc(pBase?(cTemplateLeafNodeDesc*)((cAttribLeafNode*)pBase)->GetTargetDesc():NULL)
	,m_pSourceNode(pSource)
	,m_pFieldNames(NULL)
	,m_pRefFields(NULL)
{
	m_mapFieldEnum.clear();
	m_mapFieldID.clear();
}
cTemplateLeafNodeDesc::~cTemplateLeafNodeDesc()
{
	//delete []m_pFieldIds;
	for(UINT n=0;n<m_nFields;n++)
		delete []m_pFields[n].m_ppStrings; 
	delete []m_pFieldNames;
	delete []m_pRefFields;
	delete []m_pFields;
	m_pSourceNode = NULL;
	
	
	std::map<UINT,FieldEnum *>::iterator beg = m_mapFieldEnum.begin();
	for(;beg!=m_mapFieldEnum.end();beg++)
	{
		FieldEnum *pFieldEnum = beg->second;
		if(pFieldEnum)
		{
			delete []pFieldEnum->pString;
			delete pFieldEnum;
		}
	}
	m_mapFieldEnum.clear();


	std::map<UINT,FieldId *>::iterator begID = m_mapFieldID.begin();
	for(;begID!=m_mapFieldID.end();begID++)
	{
		FieldId *pFieldID = begID->second;
		delete pFieldID;
	}
	m_mapFieldID.clear();
}

UINT cTemplateLeafNodeDesc::GetFieldCount()const 
{
	UINT nBaseFields = m_pBaseDesc?m_pBaseDesc->GetFieldCount():0;
	return m_nFields + nBaseFields;
}
const LeafField * cTemplateLeafNodeDesc::GetField(UINT n)const 
{
	UINT nBaseFields = m_pBaseDesc?m_pBaseDesc->GetFieldCount():0;
	if(n<nBaseFields)
		return m_pBaseDesc->GetField(n);
	return cLeafNodeDesc_Imp::GetField(n-nBaseFields);
}
UINT cTemplateLeafNodeDesc::FindField(const TCHAR * lpszField)const 
{
	if(m_pBaseDesc)
	{
		UINT nCol = m_pBaseDesc->FindField(lpszField);
		if(nCol != -1)
			return nCol;
	}
	UINT nBaseFields = m_pBaseDesc?m_pBaseDesc->GetFieldCount():0;
	UINT nFoundCol = cLeafNodeDesc_Imp::FindField(lpszField);
	return (nFoundCol!=-1)?(nFoundCol+nBaseFields):nFoundCol;
}
UINT cTemplateLeafNodeDesc::FindFieldByAlias(const TCHAR * lpszFieldAlias)const 
{
	if(m_pBaseDesc)
	{
		UINT nCol = m_pBaseDesc->FindFieldByAlias(lpszFieldAlias);
		if(nCol != -1)
			return nCol;
	}
	UINT nBaseFields = m_pBaseDesc?m_pBaseDesc->GetFieldCount():0;
	UINT nFoundCol = cLeafNodeDesc_Imp::FindFieldByAlias(lpszFieldAlias);
	return (nFoundCol!=-1)?(nFoundCol+nBaseFields):nFoundCol;
}
UINT cTemplateLeafNodeDesc::GetRowLength()const 
{
	UINT nBaseTempLength = m_pBaseDesc?m_pBaseDesc->GetRowLength():0;
	return m_nTempLength + nBaseTempLength;
}
UINT cTemplateLeafNodeDesc::GetInstLength()const 
{
	// yuanke.lou 实例在void cTemplateLeafNodeDesc::Build()中已经计算了基类中的实例字段长度，这里在计算导致重复，虽然没有致命影响，但是浪费了一些内存
	//UINT nBaseInstLength = m_pBaseDesc?m_pBaseDesc->GetInstLength():0;
	//return m_nInstLength + nBaseInstLength;
	return m_nInstLength;
}

UINT cTemplateLeafNodeDesc::GetIDField() const
{
	if(m_pBaseDesc)
	{
		UINT nBaseIDField = m_pBaseDesc->GetIDField();
		if(nBaseIDField != -1)
			return nBaseIDField;
	}
	return m_pBaseDesc?m_pBaseDesc->GetFieldCount()+m_nIDField:m_nIDField;
}

void cTemplateLeafNodeDesc::SetEnumString(UINT nRow,const TCHAR * lpszEnum,UINT nNum)
{
	// 不能从继承类设置基类的枚举
	UINT nBaseFields = m_pBaseDesc?m_pBaseDesc->GetFieldCount():0;
	if(nRow < nBaseFields)
		return;
	
	nRow -= nBaseFields;

	UINT nOldNum = 0;
	FieldEnum *pEnum = NULL;
	if (lpszEnum)
	{
		FieldEnum * pFieldEnum = new FieldEnum;
		ZeroMemory(pFieldEnum,sizeof(FieldEnum));
		pFieldEnum->nNum = nNum;
		pFieldEnum->pString = new TCHAR[nNum * MAX_NAME];
		ZeroMemory(pFieldEnum->pString,sizeof(TCHAR)*nNum*MAX_NAME);
		for(UINT i=0;i<nNum;i++)
		{
			lstrcpy(pFieldEnum->pString + i * MAX_NAME,lpszEnum + i * MAX_NAME);
		}

		std::map<UINT,FieldEnum *>::const_iterator it = m_mapFieldEnum.find(nRow);
		if(it != m_mapFieldEnum.end())
		{
			if(it->second)
			{
				if(it->second->pString)
					delete []it->second->pString;
				delete it->second;
			}
		}

		m_mapFieldEnum[nRow] = pFieldEnum;
		pEnum = pFieldEnum;
	}

	
	if(m_pFields/* && (nRow<=m_nFields)*/ && pEnum)
	{
		// 不管原有列枚举值的是否变动，只需重新设置枚举信息即可
		if(nRow<m_nFields)
		{
			UINT nOldNum = m_pFields[nRow].m_nMax;
			if(nOldNum != nNum)
			{
				if(m_pFields[nRow].m_ppStrings)
				{
					delete []m_pFields[nRow].m_ppStrings;
					m_pFields[nRow].m_ppStrings = NULL;
				}
				if(nNum)
					m_pFields[nRow].m_ppStrings = new TCHAR * [nNum];
				m_pFields[nRow].m_nMax = nNum;
			}
			// 内存确保
			if(!m_pFields[nRow].m_ppStrings)
			{
				if(nNum)
					m_pFields[nRow].m_ppStrings = new TCHAR * [nNum];
				m_pFields[nRow].m_nMax = nNum;
			}
			if(lpszEnum && nNum)
			{
				for(int i=0;i<m_pFields[nRow].m_nMax-m_pFields[nRow].m_nMin;i++)
				{
					m_pFields[nRow].m_ppStrings[i] = pEnum->pString + i * MAX_NAME;
				}
			}
		}
		// 新枚举列的追加，先扩展m_pFields属性列内存，然后设置枚举信息
		else
		{
			LeafField * pLeafFields = new LeafField[nRow+1];
			ZeroMemory(pLeafFields,sizeof(LeafField)*(nRow+1));
			if(m_pFields)
			{
				CopyMemory(pLeafFields,m_pFields,sizeof(LeafField)*m_nFields);
				delete []m_pFields;
			}
			m_pFields = pLeafFields;
			m_nFields = nRow+1;
			if(nNum)
				m_pFields[nRow].m_ppStrings = new TCHAR * [nNum];
			m_pFields[nRow].m_nMax = nNum;
			// 设置枚举信息
			if(lpszEnum && nNum)
			{
				for(int i=0;i<m_pFields[nRow].m_nMax-m_pFields[nRow].m_nMin;i++)
				{
					m_pFields[nRow].m_ppStrings[i] = pEnum->pString + i * MAX_NAME;
				}
			}
		}
	}
}
const TCHAR * cTemplateLeafNodeDesc::GetEnumString(UINT nRow,UINT& nNum)const 
{
	UINT nBaseFields = m_pBaseDesc?m_pBaseDesc->GetFieldCount():0;
	if(nRow<nBaseFields)
	{
		return m_pBaseDesc->GetEnumString(nRow,nNum); 
	}
	else /*if((nRow-nBaseFields)<m_nFieldEnums)*/
	{
		nRow -= nBaseFields;
		std::map<UINT,FieldEnum *>::const_iterator it = m_mapFieldEnum.find(nRow);
		if(it != m_mapFieldEnum.end())
		{
			nNum = it->second->nNum;
			return it->second->pString;
		}
	}
	nNum = 0;
	return NULL;
}
void cTemplateLeafNodeDesc::SetFieldID(UINT nRow,FieldId * pFieldId)			// 设置数据属性叶节点的属性域对应
{
	// 不能从继承类设置基类的枚举
	UINT nBaseFields = m_pBaseDesc?m_pBaseDesc->GetFieldCount():0;
	if(nRow < nBaseFields)
		return;
	
	nRow -= nBaseFields;
	FieldId * pId = NULL;
	if (pFieldId)
	{
		FieldId * pField = new FieldId;
		ZeroMemory(pField,sizeof(FieldId));

		if(pFieldId->pLeafPath)
		{
			pField->pLeafPath = new TCHAR[MAX_PATH];
			ZeroMemory(pField->pLeafPath,sizeof(TCHAR)*MAX_PATH);
			lstrcpy(pField->pLeafPath,pFieldId->pLeafPath);
			pField->nDispField = pFieldId->nDispField;
			pField->nValField = pFieldId->nValField;
		}

		std::map<UINT,FieldId *>::const_iterator it = m_mapFieldID.find(nRow);
		if(it != m_mapFieldID.end())
		{
			if(it->second)
			{
				if(it->second->pLeafPath)
					delete it->second->pLeafPath;
				delete it->second;
			}
		}

		m_mapFieldID[nRow] = pField;
		pId = pField;
	}
	
	if(m_pFields && pId)
	{
		if((nRow<m_nFields))
		{
			m_pFields[nRow].m_pFieldId = pId;
		}
		// 新枚举列的追加，先扩展m_pFields属性列内存，然后设置枚举信息
		else
		{
			LeafField * pLeafFields = new LeafField[nRow+1];
			ZeroMemory(pLeafFields,sizeof(LeafField)*(nRow+1));
			if(m_pFields)
			{
				CopyMemory(pLeafFields,m_pFields,sizeof(LeafField)*m_nFields);
				delete []m_pFields;
			}
			m_pFields = pLeafFields;
			m_nFields = nRow+1;

			// 设置ID映射信息
			m_pFields[nRow].m_pFieldId = pId;
		}
	}
}
void cTemplateLeafNodeDesc::GetFieldID(UINT nRow,FieldId * pFieldId)const 			// 取得数据属性叶节点的属性域对应
{
	UINT nBaseFields = m_pBaseDesc?m_pBaseDesc->GetFieldCount():0;
	if(nRow<nBaseFields)
	{
		m_pBaseDesc->GetFieldID(nRow,pFieldId); 
	}
	else /*if((nRow-nBaseFields)<m_nFieldIds) */
	{
		nRow -= nBaseFields;
		std::map<UINT,FieldId *>::const_iterator it = m_mapFieldID.find(nRow);
		if(it != m_mapFieldID.end())
		{
			if(it->second->pLeafPath && pFieldId->pLeafPath)
				_tcscpy(pFieldId->pLeafPath,it->second->pLeafPath);
			*pFieldId = *(it->second);
		}
		else
		{
			pFieldId->pLeafPath = NULL;
			pFieldId->nValField = 0;
			pFieldId->nDispField = 0;
		}
	}
}

void cTemplateLeafNodeDesc::Build()
{
	delete []m_pFieldNames;
	delete []m_pFields;
	m_vecFields.clear();

	m_nFields = 0;
	m_pFields = NULL;

	// 派生类的长度,继承了基类的长度
	if(m_pBaseDesc)
	{
		m_nTempLength = m_pBaseDesc->m_nTempLength;
		m_nInstLength = m_pBaseDesc->m_nInstLength;
	}
	else
	{
		m_nTempLength = 0;
		m_nInstLength = 0;
	}
	// 分配空间，暂时假设所有的行都会成为域
	UINT nRow = m_pSourceNode->GetRows();
	UINT nBaseRows = m_pSourceNode->GetBaseRows();
	if(nRow <= nBaseRows)
		return;

	nRow -= nBaseRows;
	m_pFields = new LeafField[nRow];	
	ZeroMemory(m_pFields,sizeof(LeafField) * nRow);
	m_pFieldNames = new TCHAR[nRow * MAX_NAME * 2];
	ZeroMemory(m_pFieldNames,sizeof(TCHAR) * nRow * MAX_NAME * 2);
	m_pRefFields = new TCHAR[nRow * MAX_NAME * 2];
	ZeroMemory(m_pRefFields,sizeof(TCHAR) * nRow * MAX_NAME * 2);
	

	//if (nRow>=m_nFieldEnums)
	//{
	//	FieldEnum * pFieldEnums = new FieldEnum[nRow+1];
	//	ZeroMemory(pFieldEnums,sizeof(FieldEnum)*(nRow+1));
	//	if(m_pFieldEnums)
	//	{
	//		CopyMemory(pFieldEnums,m_pFieldEnums,sizeof(FieldEnum)*m_nFieldEnums);
	//		delete []m_pFieldEnums;
	//	}
	//	m_pFieldEnums = pFieldEnums;
	//	m_nFieldEnums = nRow;
	//}

	// 0"字段名" 1"基本数值" 2"需要保存" 3"类型" 4"最小值" 5"最大值" 6"长度" 7"说明"
	if(m_pBaseDesc)
		m_nIDField = m_pBaseDesc->GetIDField();

	for(UINT n=0;n<nRow;n++)
	{
		UINT nCurRow = n+nBaseRows;
		
		if((BOOL)m_pSourceNode->GetInteger(nCurRow,LeafNode_Template-1))
			m_pFields[m_nFields].m_dwFlags |= FIELD_TEMPLATE;
		if((BOOL)m_pSourceNode->GetInteger(nCurRow,LeafNode_Instance-1))
			m_pFields[m_nFields].m_dwFlags |= FIELD_INSTANCE;
		if((BOOL)m_pSourceNode->GetInteger(nCurRow,LeafNode_Restore-1))
			m_pFields[m_nFields].m_dwFlags |= FIELD_RESTORED;
		
		m_pFields[m_nFields].m_lpszName = m_pFieldNames + n * MAX_NAME * 2;
		lstrcpy(m_pFields[m_nFields].m_lpszName,m_pSourceNode->GetString(nCurRow,(UINT)(LeafNode_Name-1))); 
		
		m_pFields[m_nFields].m_lpszAlias = m_pFieldNames + n * MAX_NAME * 2 + MAX_NAME;
		lstrcpy(m_pFields[m_nFields].m_lpszAlias,m_pSourceNode->GetString(nCurRow,(UINT)(LeafNode_Alias-1))); 
		if(!m_pFields[m_nFields].m_lpszAlias[0])
			_stprintf(m_pFields[m_nFields].m_lpszAlias,_T("Field%d"),n+1);

		m_pFields[m_nFields].m_lpszRefField = m_pRefFields + n * MAX_NAME * 2;
		lstrcpy(m_pFields[m_nFields].m_lpszRefField,m_pSourceNode->GetString(nCurRow,(UINT)(LeafNode_RefField-1))); 

		m_pFields[m_nFields].m_eType = (FieldType)m_pSourceNode->GetInteger(nCurRow,LeafNode_Type-1);
		m_pFields[m_nFields].m_nLength = m_pSourceNode->GetInteger(nCurRow,LeafNode_Length-1);
		m_pFields[m_nFields].m_nMax = m_pSourceNode->GetInteger(nCurRow,LeafNode_Max-1);
		m_pFields[m_nFields].m_nMin = m_pSourceNode->GetInteger(nCurRow,LeafNode_Min-1);
		m_pFields[m_nFields].m_nKeyOrder = m_pSourceNode->GetInteger(nCurRow,LeafNode_Index-1);
		m_pFields[m_nFields].m_nLimitField = m_pSourceNode->GetInteger(nCurRow,LeafNode_Limit-1);
		m_pFields[m_nFields].m_bIsDeriveField = m_pSourceNode->GetInteger(nCurRow,LeafNode_IsDeriveField-1);

		if(m_pFields[m_nFields].m_nLimitField)
			m_pFields[m_pFields[m_nFields].m_nLimitField-1-nBaseRows].m_nLimitDest = nCurRow+1;

		if(m_pFields[m_nFields].m_dwFlags & FIELD_TEMPLATE)
			m_pFields[m_nFields].m_nTemplateOffset = m_nTempLength;
		else
			m_pFields[m_nFields].m_nTemplateOffset = -1;
		if(m_pFields[m_nFields].m_dwFlags & FIELD_INSTANCE)
			m_pFields[m_nFields].m_nInstanceOffset = m_nInstLength;
		else
			m_pFields[m_nFields].m_nInstanceOffset = -1;

		
		// 目前非字符串的域长度统统为4
		// todo:不允许长度为0 合法性校验。
		if(m_pFields[m_nFields].m_eType != Field_String)
		{
			m_pFields[m_nFields].m_nLength = 4;
		}



		// 枚举字符串最大最小值校验
		if(m_pFields[m_nFields].m_eType == Field_Enumerate)
		{
			std::map<UINT,FieldEnum *>::iterator it = m_mapFieldEnum.find(n);
			if(it!=m_mapFieldEnum.end())
			{
				m_pFields[m_nFields].m_nMin = 0;
				m_pFields[m_nFields].m_nMax = it->second->nNum;
				m_pSourceNode->SetInteger(nCurRow,LeafNode_Min-1,0);
				m_pSourceNode->SetInteger(nCurRow,LeafNode_Max-1,it->second->nNum);
				if(m_pFields[m_nFields].m_ppStrings)
					delete m_pFields[m_nFields].m_ppStrings;
				m_pFields[m_nFields].m_ppStrings = new TCHAR * [it->second->nNum];
				ZeroMemory(m_pFields[m_nFields].m_ppStrings,sizeof(TCHAR*)*(it->second->nNum));
				for(int i=0;i<m_pFields[m_nFields].m_nMax-m_pFields[m_nFields].m_nMin;i++)
				{
					m_pFields[m_nFields].m_ppStrings[i] = it->second->pString + i * MAX_NAME;
				}
			}
		}
		else if(m_pFields[m_nFields].m_eType == Field_IdField)
		{
			std::map<UINT,FieldId *>::const_iterator itId = m_mapFieldID.find(n);
			if(itId!=m_mapFieldID.end())
			{
				// 根据叶节点路径取得叶节点
				if(itId->second->pLeafPath != NULL)
				{
					cLeafNode * pLeafNode = (cLeafNode *)g_NodeStreamImp.GetRoot()->GetNodeByPath(itId->second->pLeafPath); 
					UINT nNum = pLeafNode->GetRows(); 
					
					m_pFields[m_nFields].m_nMin = 0;
					m_pFields[m_nFields].m_nMax = nNum;
					m_pSourceNode->SetInteger(nCurRow,LeafNode_Min-1,0);
					m_pSourceNode->SetInteger(nCurRow,LeafNode_Max-1,nNum);
					m_pFields[m_nFields].m_pFieldId = itId->second;
				}
			}
		}

		if(m_pFields[m_nFields].m_dwFlags & FIELD_TEMPLATE)
			m_nTempLength += m_pFields[m_nFields].m_nLength;
		if(m_pFields[m_nFields].m_dwFlags & FIELD_INSTANCE)
			m_nInstLength += m_pFields[m_nFields].m_nLength;

		Field f = {m_pFields + m_nFields,m_nFields};
		m_vecFields.push_back(f);
		if(m_nIDField == -1 && lstrcmpi(m_pFields[m_nFields].m_lpszName,_T("ID")) == 0)
			m_nIDField = m_nFields;
		
		m_nFields ++;
		//}
	}
}

cLeafRow_Imp::cLeafRow_Imp()
	:m_pNodeDesc(NULL)
	,m_pBuffer(NULL)
#ifdef _DEBUG
	,m_pFieldValues(NULL)
#endif
	,m_ppLimitFields(NULL)
	,m_ppEffectValues(NULL)
	,m_nLimitFields(0)
	,m_nEffectValues(0)
	,m_ppBuffEffect(0)
	,m_nBuffEffects(0)
{
}

cLeafRow_Imp::~cLeafRow_Imp()
{
	m_pNodeDesc = NULL;
	m_pBuffer = NULL;
#ifdef _DEBUG
	m_pFieldValues = NULL;
#endif
	delete []m_ppLimitFields;
	delete []m_ppEffectValues;
	delete []m_ppBuffEffect;
	m_ppLimitFields = NULL;
	m_ppEffectValues = NULL;
	m_ppBuffEffect = NULL;
	m_nLimitFields = 0;
	m_nEffectValues = 0;
	m_nBuffEffects = 0;
}

#ifdef _DEBUG
void cLeafRow_Imp::Attach(cLeafNodeDesc * pDesc,BYTE * pBuffer,FieldValue * pFieldValues)
{
	m_pNodeDesc = pDesc;
	m_pBuffer = pBuffer;
	m_pFieldValues = pFieldValues;
}
#else
void cLeafRow_Imp::Attach(cLeafNodeDesc * pDesc,BYTE * pBuffer)
{
	m_pNodeDesc = pDesc;
	m_pBuffer = pBuffer;
}
#endif

const TCHAR * cLeafRow_Imp::GetString(UINT col)const 
{
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if(pField && (pField->m_nTemplateOffset>=0) )
	{
		switch(pField->m_eType)
		{
		case Field_String:
			{
				return (char *)m_pBuffer + pField->m_nTemplateOffset;
			}
			break;
		case Field_Enumerate:
			{
				int nValue = *(int*)((char *)m_pBuffer + pField->m_nTemplateOffset);
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
				int nValue = *(int*)((char *)m_pBuffer + pField->m_nTemplateOffset);
				return _itoa(nValue,szNumber,10);
			};
			break;
		case Field_Float:
			{
				float fValue = *(float*)((char *)m_pBuffer + pField->m_nTemplateOffset);
				_stprintf(szNumber,"%.4f",fValue);
				return szNumber;
			}
			break;
		case Field_IdField:
			{
				int nValue = *(int*)((char *)m_pBuffer + pField->m_nTemplateOffset);
				if(pField->m_pFieldId && pField->m_pFieldId->pLeafPath)
				{
					cLeafNode * pSourceNode = (cLeafNode *)g_NodeStreamImp.GetRoot()->GetNodeByPath(pField->m_pFieldId->pLeafPath);
					if(pSourceNode)
					{
						UINT nRow = pSourceNode->FindRow((UINT)pField->m_pFieldId->nValField,nValue); 
						if(nRow != -1)
							return pSourceNode->GetString(nRow,pField->m_pFieldId->nDispField); 
					}
				}
			}
			break;
		}
	}
	return NULL;
}
int cLeafRow_Imp::GetInteger(UINT col)const 
{
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if(pField && (pField->m_nTemplateOffset>=0) )
	{
		switch(pField->m_eType)
		{
		case Field_String:
			return atoi((char *)m_pBuffer + pField->m_nTemplateOffset);
			break;
		case Field_Enumerate:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Boolean:
		case Field_Percent:
		case Field_IdField:
			return *(int*)((char *)m_pBuffer + pField->m_nTemplateOffset);
			break;
		case Field_Float:
			return (int)(*(float*)((char *)m_pBuffer + pField->m_nTemplateOffset));
		}
	}
	return 0;
}
float cLeafRow_Imp::GetFloat(UINT col)	const 									// 取得指定位置的值
{
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if(pField && (pField->m_nTemplateOffset>=0) )
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
			break;
		case Field_Float:
			return (*(float*)((char *)m_pBuffer + pField->m_nTemplateOffset));
		}
	}
	return 0.0f;
}

void cLeafRow_Imp::SetString(UINT col,const TCHAR * string)
{
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if(pField && (pField->m_nTemplateOffset>=0) )
	{
		switch(pField->m_eType)
		{
		case Field_String:
			{
				int nLen = min(lstrlen(string),pField->m_nLength-1);
				lstrcpyn((char *)m_pBuffer + pField->m_nTemplateOffset,string,nLen+1);
				*((char *)m_pBuffer + pField->m_nTemplateOffset + nLen) = 0;
			}
			break;
		case Field_Enumerate:
			{
				int n = 0;
				if(pField->m_ppStrings)
				{
					for(n=0;n<(pField->m_nMax-pField->m_nMin);n++)
					{
						if(pField->m_ppStrings[n])
						{
							if(!_stricmp(pField->m_ppStrings[n],string))
								break;
						}
					}
				}
				if(n==(pField->m_nMax-pField->m_nMin))
					n = 0;
				*(int*)((char *)m_pBuffer + pField->m_nTemplateOffset) = n;
			}
			break;
		case Field_Boolean:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Percent:
			*(int*)((char *)m_pBuffer + pField->m_nTemplateOffset) = _ttoi(string);
			break;

		case Field_Float:
			*(float*)((char *)m_pBuffer + pField->m_nTemplateOffset) = (float)_tstof(string);
			break;
		case Field_IdField:
			{
				int nValue = *(int*)((char *)m_pBuffer + pField->m_nTemplateOffset);
				if(pField->m_pFieldId && pField->m_pFieldId->pLeafPath)
				{
					cLeafNode * pSourceNode = (cLeafNode *)g_NodeStreamImp.GetRoot()->GetNodeByPath(pField->m_pFieldId->pLeafPath);
					if(pSourceNode)
					{
						UINT nRow = pSourceNode->FindRow(pField->m_pFieldId->nDispField,string); 
						if(nRow != -1)
							*(int*)((char *)m_pBuffer + pField->m_nTemplateOffset) = pSourceNode->GetInteger(nRow,(UINT)pField->m_pFieldId->nValField);
					}
				}
			}
			break;
		}
	}
}
void cLeafRow_Imp::SetInteger(UINT col,int nValue)
{
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if(pField && (pField->m_nTemplateOffset>=0) )
	{
		switch(pField->m_eType)
		{
		case Field_String:
			lstrcpy((char *)m_pBuffer + pField->m_nTemplateOffset,_itoa(nValue,szNumber,10));
			break;
		case Field_Enumerate:
		case Field_Boolean:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Percent:
		case Field_IdField:
			*(int*)((char *)m_pBuffer + pField->m_nTemplateOffset) = nValue;
			break;
		case Field_Float:
			*(float*)((char *)m_pBuffer + pField->m_nTemplateOffset) = (float)nValue;
			break;
		}
	}
}
void cLeafRow_Imp::SetFloat(UINT col,float fValue)
{
	const LeafField * pField = m_pNodeDesc->GetField(col);
	if(pField && (pField->m_nTemplateOffset>=0) )
	{
		switch(pField->m_eType)
		{
		case Field_String:
		case Field_Enumerate:
		case Field_Boolean:
		case Field_Integer:
		case Field_Dynamic:
		case Field_Percent:
		case Field_IdField:
			break;
		case Field_Float:
			*(float*)((char *)m_pBuffer + pField->m_nTemplateOffset) = fValue;
			break;
		}
	}
}

const TCHAR * cLeafRow_Imp::GetString(const TCHAR * lpszField)const 
{
	UINT col = m_pNodeDesc->FindField(lpszField);
	if(col != (UINT)-1)
		return GetString(col);
	return NULL;
}
int cLeafRow_Imp::GetInteger(const TCHAR * lpszField)const 
{
	UINT col = m_pNodeDesc->FindField(lpszField);
	if(col != (UINT)-1)
		return GetInteger(col);
	return 0;
}
float cLeafRow_Imp::GetFloat(const TCHAR * lpszField)const 
{
	UINT col = m_pNodeDesc->FindField(lpszField);
	if(col != (UINT)-1)
		return GetFloat(col);
	return 0.0f;
}
void cLeafRow_Imp::SetString(const TCHAR * lpszField,const TCHAR * string)
{
	UINT col = m_pNodeDesc->FindField(lpszField);
	if(col != (UINT)-1)
		return SetString(col,string);
}
void cLeafRow_Imp::SetInteger(const TCHAR * lpszField,int nValue)
{
	UINT col = m_pNodeDesc->FindField(lpszField);
	if(col != (UINT)-1)
		return SetInteger(col,nValue);
}
void cLeafRow_Imp::SetFloat(const TCHAR * lpszField,float fValue)
{
	UINT col = m_pNodeDesc->FindField(lpszField);
	if(col != (UINT)-1)
		return SetFloat(col,fValue);
}
void cLeafRow_Imp::BuildRelation(const cLeafNode * pLimiteNode,const cLeafNode * pEffectNode,const cLeafNode * pBuffEffect)
{
	// 只针对数据模板类
	if(m_pNodeDesc->GetType() == Node_UnitTemplate)
	{
		int nID = GetInteger((UINT)0); 
		
		// 限制
		if(pLimiteNode)
		{
			ResultRows rLimite = pLimiteNode->Query(cQuery(nID),TRUE);
			m_nLimitFields = rLimite.nCount;
			if(rLimite.nCount>0)
			{
				m_ppLimitFields = new LimitField*[rLimite.nCount];
				for(UINT i=0;i<rLimite.nCount;i++)
					m_ppLimitFields[i] = (LimitField*)pLimiteNode->GetRow( rLimite.nFirst + i)->GetBuffer();
			}
		}
		// 影响
		if(pEffectNode)
		{
			ResultRows rEffect = pEffectNode->Query(cQuery(nID),TRUE);
			m_nEffectValues = rEffect.nCount;
			if(rEffect.nCount>0)
			{
				m_ppEffectValues = new EffectValue*[rEffect.nCount];
				for(UINT i=0;i<rEffect.nCount;i++)
					m_ppEffectValues[i] = (EffectValue*)pEffectNode->Result(&rEffect,i)->GetBuffer();
			}
		}
		// BUFF影响
		if (pBuffEffect)
		{
			ResultRows rEffect = pBuffEffect->Query(cQuery(nID),TRUE);
			m_nBuffEffects = rEffect.nCount;
			if(rEffect.nCount>0)
			{
				m_ppBuffEffect = new BuffEffectValue*[rEffect.nCount];
				for(UINT i=0;i<rEffect.nCount;i++)
					m_ppBuffEffect[i] = (BuffEffectValue*)pBuffEffect->Result(&rEffect,i)->GetBuffer();
			}
		}
	}
}


void cAttribLeafNode::GetBase()
{
	// 查找自己的基类
	const UnitDef * pUnitDef = ((cUnitTreeNode *)m_pParent)->GetUnitDef();
	if(pUnitDef && pUnitDef->m_nBaseClass)
	{
		cLeafNode * pNode = g_NodeStreamImp.GetUnitManager(); 
		UINT nRow = pNode->FindRow((UINT)1,m_pParent->GetName());
		if(nRow != -1)
		{
			int nBaseClass = pNode->GetInteger(nRow,m_pParent->GetName());
			nRow = pNode->FindRow((UINT)0,pUnitDef->m_nBaseClass);
			if(nRow != -1)
			{
				const cLeafRow * pRow = pNode->GetRow(nRow); 
				const TCHAR * lpszBaseName = pRow->GetString(UNIT_ALIAS_COL); // 基类的名字
				m_pBaseClassDesc = g_NodeStreamImp.GetUnitAttrib(lpszBaseName); 
			}
			else
			{
				m_pBaseClassDesc = NULL;
			}
		}
	}
}

cAttribLeafNode::cAttribLeafNode(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent)
	:cLeafNode_Imp(pDesc,lpszName,lpszAlias,pParent)
	,m_pTargetDesc(NULL)
	,m_pBaseClassDesc(NULL)
{
	GetBase();
	m_pTargetDesc = new cTemplateLeafNodeDesc(g_NodeTable[Node_UnitTemplate].pNodeDesc,this,m_pBaseClassDesc);
}

cAttribLeafNode::~cAttribLeafNode()
{
	delete m_pTargetDesc;
	m_pTargetDesc = NULL;
}
UINT cAttribLeafNode::GetRows()const
{
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;  
	return m_nRows + nBaseRows;
}
const TCHAR * cAttribLeafNode::GetString(UINT row,UINT col) const			// 取得指定位置的值
{
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	if(row<nBaseRows)
		return m_pBaseClassDesc->GetString(row,col);
	return cLeafNode_Imp::GetString(row-nBaseRows,col);
}
int cAttribLeafNode::GetInteger(UINT row,UINT col) const						// 取得指定位置的值
{
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	if(row<nBaseRows)
		return m_pBaseClassDesc->GetInteger(row,col);
	return cLeafNode_Imp::GetInteger(row-nBaseRows,col);
}
float cAttribLeafNode::GetFloat(UINT row,UINT col) const						// 取得指定位置的值
{
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	if(row<nBaseRows)
		return m_pBaseClassDesc->GetFloat(row,col);
	return cLeafNode_Imp::GetFloat(row-nBaseRows,col);
}
void cAttribLeafNode::SetString(UINT row,UINT col,const TCHAR * string)	// 设置指定位置的值
{
	// 继承类是无法编辑基类的。
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	if(row>=nBaseRows)
		cLeafNode_Imp::SetString(row-nBaseRows,col,string);
	else	// 对于字段最终值公式部分的调整20080625
		m_pBaseClassDesc->SetString(row,col,string);
}
void cAttribLeafNode::SetInteger(UINT row,UINT col,int nValue)			// 设置指定位置的值
{
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	if(row>=nBaseRows)
		cLeafNode_Imp::SetInteger(row-nBaseRows,col,nValue);
	else	// 对于字段最终值公式部分的调整20080625
		m_pBaseClassDesc->SetInteger(row,col,nValue);

}
void cAttribLeafNode::SetFloat(UINT row,UINT col,float fValue)			// 设置指定位置的值
{
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	if(row>=nBaseRows)
		cLeafNode_Imp::SetFloat(row-nBaseRows,col,fValue);
}

void cAttribLeafNode::ImportEnum(HANDLE hXmlNode,UINT nRow)
{
	pug::xml_node * pNode = (pug::xml_node *)hXmlNode;
	std::string author = pNode->attribute(_T("ss:Author")).getvalue();
	author += _T(":");
	std::vector<std::string> vecAttribEnum;			// 枚举字符串
	if(pNode->moveto_child("ss:Data"))
	{
		for(UINT i=0;i<2;i++)
		{
			if(pNode->moveto_child(i))
			{
				pug::xml_node _comment = *pNode;
				while(_comment.moveto_child((unsigned int)0));
				_comment.moveto_parent();
				std::string _c = _comment.child_value();
				if(_c != author)
				{
					SplitString(_c,vecAttribEnum,"&#10;");
					size_t nNum = vecAttribEnum.size();
					TCHAR * lpszEnum = new TCHAR[ nNum * MAX_NAME];
					ZeroMemory(lpszEnum,nNum * MAX_NAME);
					for(UINT x=0;x<nNum;x++)
					{
						size_t len = vecAttribEnum[x].length();
						lstrcpyn(lpszEnum + x * MAX_NAME,vecAttribEnum[x].c_str(),MAX_NAME-1); 
					}
					m_pTargetDesc->SetEnumString(nRow,lpszEnum,(UINT)nNum);
					delete []lpszEnum;
				}
				pNode->moveto_parent();
			}
		}
		pNode->moveto_parent();
	}
	pNode->moveto_parent();
}

void cAttribLeafNode::ExportEnum(HANDLE hXmlNode,UINT nRow) const
{
	pug::xml_node * pCell = (pug::xml_node *)hXmlNode;
	pug::xml_node commn = pCell->append_child(pug::node_element,"Comment");
	commn = commn.append_child(pug::node_element,"ss:Data");

	UINT nNum = 0;
	const TCHAR * lpszEnum = m_pTargetDesc->GetEnumString(nRow,nNum);
	if(lpszEnum && lpszEnum[0] && nNum)
	{
		std::string strEnum;
		UINT x;
		for(x=0;x<nNum-1;x++)
		{
			strEnum += lpszEnum + x*MAX_NAME;
			strEnum += _T("&#10;");
		}	
		strEnum += lpszEnum + x*MAX_NAME;
		commn.set_child_value(pug::node_pcdata,strEnum.c_str()); 
	}
}
// 设置数据属性叶节点的枚举字符串
BOOL cAttribLeafNode::SetEnumString(UINT nRow,const TCHAR * lpszEnum,UINT nNum)
{
	if(m_pTargetDesc)
	{
		m_pTargetDesc->SetEnumString(nRow,lpszEnum,nNum); 
		return TRUE;
	}	
	return FALSE;
}
const TCHAR * cAttribLeafNode::GetEnumString(UINT nRow,UINT& nNum)const					// 取得数据属性叶节点的枚举字符串
{
	if(m_pTargetDesc)
	{
		UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
		if(nRow<(m_nRows+nBaseRows))
			return m_pTargetDesc->GetEnumString(nRow,nNum);
		return NULL;
	}
	return NULL;
}

void cAttribLeafNode::SetFieldID(UINT nRow,FieldId * pFieldId)	// 设置数据属性叶节点的属性域对应
{
	if(m_pTargetDesc)
	{
		m_pTargetDesc->SetFieldID(nRow,pFieldId); 
	}	
}
void cAttribLeafNode::GetFieldID(UINT nRow,FieldId * pFieldId) const	// 取得数据属性叶节点的属性域对应
{
	if(m_pTargetDesc)
	{
		UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
		if(nRow<(m_nRows+nBaseRows))
			m_pTargetDesc->GetFieldID(nRow,pFieldId);
	}
}
void cAttribLeafNode::SetRows(UINT nRow)				// 设置行数
{
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	nRow = max(nBaseRows,nRow);
	ValidRow(nRow,TRUE);
	m_nRows = nRow-nBaseRows;
}
UINT cAttribLeafNode::GetBaseRows() const
{
	return m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;  
}
UINT cAttribLeafNode::FindRow(UINT nField,int nValue) const
{
	if(m_pBaseClassDesc)
		return m_pBaseClassDesc->FindRow(nField,nValue);
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	return cLeafNode_Imp::FindRow(nField,nValue) + nBaseRows; 
}
UINT cAttribLeafNode::FindRow(const TCHAR * lpcstrField,int nValue) const
{
	if(m_pBaseClassDesc)
		return m_pBaseClassDesc->FindRow(lpcstrField,nValue);
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	return cLeafNode_Imp::FindRow(lpcstrField,nValue) + nBaseRows; 
}
UINT cAttribLeafNode::FindRow(UINT nField,const char * lpcstrValue) const
{
	if(m_pBaseClassDesc)
		return m_pBaseClassDesc->FindRow(nField,lpcstrValue);
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	return cLeafNode_Imp::FindRow(nField,lpcstrValue) + nBaseRows; 
}
UINT cAttribLeafNode::FindRow(const TCHAR * lpcstrField,const TCHAR * lpcstrValue) const
{
	if(m_pBaseClassDesc)
		return m_pBaseClassDesc->FindRow(lpcstrField,lpcstrValue);
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	return cLeafNode_Imp::FindRow(lpcstrField,lpcstrValue) + nBaseRows; 
}
void cAttribLeafNode::ValidRow(UINT nRow,BOOL bAllocMore)
{
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	if(nRow > (m_nRows + nBaseRows))
		cLeafNode_Imp::ValidRow(nRow - nBaseRows,bAllocMore);
}
void cAttribLeafNode::BuildIndex()							// 重新建立索引	
{
	if(m_pBaseClassDesc)
		m_pBaseClassDesc->BuildIndex();
	cLeafNode_Imp::BuildIndex();
}
ResultRows cAttribLeafNode::Query(cQuery q,BOOL bLastResult)const 					// 根据索引寻找行号
{
	if(m_pBaseClassDesc)
		return m_pBaseClassDesc->Query(q,bLastResult);
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	ResultRows r = cLeafNode_Imp::Query(q,bLastResult);
	for(UINT n = 0;n<r.nCount;n++)
		r.nFirst += nBaseRows;
	return r;
}
const cLeafRow * cAttribLeafNode::GetRow(UINT nRow)const 					// 根据行号取得行
{
	UINT nBaseRows = m_pBaseClassDesc?m_pBaseClassDesc->GetRows():0;
	if(nRow<nBaseRows)
		return m_pBaseClassDesc->GetRow(nRow);
	//这块有问题吗？20080625
	//return cLeafNode_Imp::GetRow(nRow);
	return cLeafNode_Imp::GetRow(nRow-nBaseRows);
}

void cAttribLeafNode::SetDependField(UINT nRow,std::string & strValue)
{
	m_mapDepend[nRow]=strValue;
	if(!strValue.empty())
	{
		UINT nValue = 0;
		std::set<UINT> &setConverse = m_mapConverseDepend[nRow];
		std::vector<std::string> vecRef;
		SplitString(strValue,vecRef,",");
		for(size_t i=0;i<vecRef.size();i++)
		{
			nValue = atoi(vecRef[i].c_str())-1;
			// 建立正向依赖关系
			setConverse.insert(nValue);

			// 反向依赖关系建立
			std::set<UINT> &setPositive = m_mapPositiveDepend[nValue];
			setPositive.insert(nRow);
		}
	}
}

////////////////////////////////////////////////////////////
// 如果A依赖B，那么 IfHasConverseDepend(A) = true
////////////////////////////////////////////////////////////
bool cAttribLeafNode::IfHasConverseDepend(UINT nField)
{
	return m_mapConverseDepend.find(nField) != m_mapConverseDepend.end();
}

////////////////////////////////////////////////////////////
// 如果A依赖B，那么 IfHasPositiveDepend(B) = true
////////////////////////////////////////////////////////////
bool cAttribLeafNode::IfHasPositiveDepend(UINT nField)
{
	if(m_mapPositiveDepend.find(nField) != m_mapPositiveDepend.end())
		return true;
	return false;
}
bool cAttribLeafNode::IfHasPositiveDepend(UINT nField1,UINT nField2)
{
	std::map<UINT,std::set<UINT> >::iterator i = m_mapPositiveDepend.find(nField1);
	if(i != m_mapPositiveDepend.end())
	{
		std::set<UINT>& s = i->second;
		if(s.find(nField2) != s.end())
			return true;
	}
	return false;
}

////////////////////////////////////////////////////////////
// 如果字段A有依赖项，那么取得所有A的依赖字段
////////////////////////////////////////////////////////////
std::set<UINT> &cAttribLeafNode::GetConverseDependFields(UINT nField)
{
	return m_mapConverseDepend[nField];
}

////////////////////////////////////////////////////////////
// 如果字段A被其他字段依赖，那么取得所有依赖A的字段
////////////////////////////////////////////////////////////
std::set<UINT> &cAttribLeafNode::GetPositiveDependFields(UINT nField)
{
	return m_mapPositiveDepend[nField];
}

struct FieldSorter
{
	cAttribLeafNode * m_pNode;
	FieldSorter(cAttribLeafNode * pNode)
	{
		m_pNode = pNode;
	}
	bool operator ()(UINT const &t1, UINT const &t2)
	{
		int nD1 = m_pNode->GetDependLevel(t1);
		int nD2 = m_pNode->GetDependLevel(t2);
		if(nD1 == 3 && nD2 == 3)
			return m_pNode->IfHasPositiveDepend(t1,t2);
		return nD1<nD2;
	}
};

int cAttribLeafNode::GetDependLevel(UINT nField)
{
	// 独立的字段
	if (!IfHasPositiveDepend(nField) && !IfHasConverseDepend(nField))
		return 0;

	// 别人依赖自己的但是自己不依赖别人的
	else if(IfHasPositiveDepend(nField) && !IfHasConverseDepend(nField))
		return 1;

	// 自己依赖别人的但是不被别人依赖
	else if(!IfHasPositiveDepend(nField) && IfHasConverseDepend(nField))
		return 4;

	return 3;
}

void cAttribLeafNode::BuildOrderLst()
{
	UINT nRows = GetRows();
	for (UINT i=0;i<nRows;i++)
		m_vecOrderField.push_back(i);
	std::sort(m_vecOrderField.begin(),m_vecOrderField.end(),FieldSorter(this));
}

const stState * cFsmNode_Imp::GetState(UINT nStateID)
{

	if(m_mapState.find(nStateID) != m_mapState.end())
	{
		return &m_mapState[nStateID];
	}
	return NULL;

}

void cFsmNode_Imp::AddState(UINT nSequence,const stState * pState)
{
	if(nSequence > MAX_STATE)
		return;
	if(pState)
	{
		if(m_nRow[nSequence-1] != 0 && m_nRow[nSequence-1] != pState->nID)
		{
			for(int i=(int)m_nStateCnt;i>(int)nSequence-1;i--)
			{
				m_nRow[i] = m_nRow[i-1];
			}
		}
		m_nRow[nSequence-1] = pState->nID;
		m_mapState[pState->nID] = *pState;
		m_nStateCnt++;
	}
}

void cFsmNode_Imp::DelState(UINT nStateID)
{
	if(m_mapState.find(nStateID) != m_mapState.end())
	{
		//调整顺序
		for(UINT i=0;i<m_nStateCnt;i++)
		{
			if(m_nRow[i] == nStateID)
			{
				//首先清除和该状态相关联的状态转换标志
				struct stState & state = m_mapState[nStateID];
				std::map<UINT,struct stState>::iterator beg = m_mapState.begin();
				for(;beg!=m_mapState.end();beg++)
				{
					beg->second.nRestoreTo &= ~(1<<nStateID);
				}
				//重新排列状态位置
				for(UINT j=i;j<m_nStateCnt-1;j++)
				{
					m_nRow[j] = m_nRow[j+1];
				}
				break;
			}
		}
		//然后删除该状态
		m_mapState.erase(nStateID);
		m_nStateCnt--;
	}
}

const stState * cFsmNode_Imp::GetFirstState()
{
	if(!m_nStateCnt)
		return NULL;
	m_nPos = 0;
	return &(m_mapState[ m_nRow[m_nPos++] ]);
}

const stState * cFsmNode_Imp::GetNextState(UINT nStateID)
{
	if(m_nPos < m_nStateCnt)
		return &(m_mapState[ m_nRow[m_nPos++] ]);
	return NULL;
}

UINT cFsmNode_Imp::SwitchState(const UINT nState)
{
	const struct stState *pState = this->GetCurrentState();
	if(pState)
	{
		if(pState->nRestoreTo & ~(1<<nState))
		{
			return nState;
		}
	}

	return m_nCurrentID;
}

cFsmLeafNode::cFsmLeafNode(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent)
	:cLeafNode_Imp(pDesc,lpszName,lpszAlias,pParent)
{
	m_pFsmNode = new cFsmNode_Imp;
}


cContactDataLeafNodeDesc::cContactDataLeafNodeDesc(cNodeDesc * pDesc,cLeafNode * pSource,cLeafNode * pBase)
	:cLeafNodeDesc_Imp(pDesc->GetType(),(TCHAR *)pDesc->GetDescName(),pDesc->IsFiled(),pDesc->IfExclusive(),NULL)   
	,m_pBaseDesc(pBase?(cContactDataLeafNodeDesc*)((cContactAttribLeafNode*)pBase)->GetTargetDesc():NULL)
	,m_pSourceNode(pSource)
	,m_pFieldNames(NULL)
{
	m_mapFieldEnum.clear();
	m_mapFieldID.clear();
}
cContactDataLeafNodeDesc::~cContactDataLeafNodeDesc()
{
	for(UINT n=0;n<m_nFields;n++)
		delete []m_pFields[n].m_ppStrings; 
	delete []m_pFieldNames;
	delete []m_pFields;
	m_pSourceNode = NULL;

	std::map<UINT,FieldEnum *>::iterator beg = m_mapFieldEnum.begin();
	for(;beg!=m_mapFieldEnum.end();beg++)
	{
		FieldEnum *pFieldEnum = beg->second;
		if(pFieldEnum)
		{
			delete []pFieldEnum->pString;
			delete pFieldEnum;
		}
	}
	m_mapFieldEnum.clear();

	std::map<UINT,FieldId *>::iterator begID = m_mapFieldID.begin();
	for(;begID!=m_mapFieldID.end();begID++)
	{
		FieldId *pFieldID = begID->second;
		if(pFieldID)
			delete pFieldID;
	}
	m_mapFieldID.clear();
}

void cContactDataLeafNodeDesc::SetEnumString(UINT nRow,const TCHAR * lpszEnum,UINT nNum)
{
	// 不能从继承类设置基类的枚举
	UINT nBaseFields = m_pBaseDesc?m_pBaseDesc->GetFieldCount():0;
	if(nRow < nBaseFields)
		return;

	nRow -= nBaseFields;

	UINT nOldNum = 0;
	FieldEnum *pEnum = NULL;
	if (lpszEnum)
	{
		FieldEnum * pFieldEnum = new FieldEnum;
		ZeroMemory(pFieldEnum,sizeof(FieldEnum));
		pFieldEnum->nNum = nNum;
		pFieldEnum->pString = new TCHAR[nNum * MAX_NAME];
		ZeroMemory(pFieldEnum->pString,sizeof(TCHAR)*nNum*MAX_NAME);
		for(UINT i=0;i<nNum;i++)
		{
			lstrcpy(pFieldEnum->pString + i * MAX_NAME,lpszEnum + i * MAX_NAME);
		}

		std::map<UINT,FieldEnum *>::const_iterator it = m_mapFieldEnum.find(nRow);
		if(it != m_mapFieldEnum.end())
		{
			if(it->second)
				if(it->second->pString)
					delete []it->second->pString;
				delete it->second;
		}
		m_mapFieldEnum[nRow] = pFieldEnum;
		pEnum = pFieldEnum;
	}


	if(m_pFields/* && (nRow<=m_nFields)*/ && pEnum)
	{
		// 不管原有列枚举值的是否变动，只需重新设置枚举信息即可
		if(nRow<m_nFields)
		{
			UINT nOldNum = m_pFields[nRow].m_nMax;
			if(nOldNum != nNum)
			{
				if(m_pFields[nRow].m_ppStrings)
				{
					delete []m_pFields[nRow].m_ppStrings;
					m_pFields[nRow].m_ppStrings = NULL;
				}
				if(nNum)
					m_pFields[nRow].m_ppStrings = new TCHAR * [nNum];
				m_pFields[nRow].m_nMax = nNum;
				m_pFields[nRow].m_nMin = 0;
			}
			if(lpszEnum && nNum)
			{
				if (m_pFields[nRow].m_ppStrings == NULL)
				{
					m_pFields[nRow].m_ppStrings = new TCHAR * [nNum];
				}
				for(int i=0;i<m_pFields[nRow].m_nMax-m_pFields[nRow].m_nMin;i++)
				{
					m_pFields[nRow].m_ppStrings[i] = pEnum->pString + i * MAX_NAME;
				}
			}
		}
		// 新枚举列的追加，先扩展m_pFields属性列内存，然后设置枚举信息
		else
		{
			LeafField * pLeafFields = new LeafField[nRow+1];
			ZeroMemory(pLeafFields,sizeof(LeafField)*(nRow+1));
			if(m_pFields)
			{
				CopyMemory(pLeafFields,m_pFields,sizeof(LeafField)*m_nFields);
				delete []m_pFields;
			}
			m_pFields = pLeafFields;
			m_nFields = nRow+1;
			if(nNum)
				m_pFields[nRow].m_ppStrings = new TCHAR * [nNum];
			m_pFields[nRow].m_nMax = nNum;
			// 设置枚举信息
			if(lpszEnum && nNum)
			{
				for(int i=0;i<m_pFields[nRow].m_nMax-m_pFields[nRow].m_nMin;i++)
				{
					m_pFields[nRow].m_ppStrings[i] = pEnum->pString + i * MAX_NAME;
				}
			}
		}
	}
}

const TCHAR * cContactDataLeafNodeDesc::GetEnumString(UINT nRow,UINT& nNum)const 
{
	UINT nBaseFields = m_pBaseDesc?m_pBaseDesc->GetFieldCount():0;
	if(nRow<nBaseFields)
	{
		return m_pBaseDesc->GetEnumString(nRow,nNum); 
	}
	else/* if((nRow-nBaseFields)<m_nFieldEnums)*/
	{
		nRow -= nBaseFields;
		std::map<UINT,FieldEnum *>::const_iterator it = m_mapFieldEnum.find(nRow);
		if(it != m_mapFieldEnum.end())
		{
			nNum = it->second->nNum;
			return it->second->pString;
		}		
	}
	nNum = 0;
	return NULL;
}

void cContactDataLeafNodeDesc::SetFieldID(UINT nRow,FieldId * pFieldId)			// 设置数据属性叶节点的属性域对应
{
	// 不能从继承类设置基类的枚举
	UINT nBaseFields = m_pBaseDesc?m_pBaseDesc->GetFieldCount():0;
	if(nRow < nBaseFields)
		return;
	
	nRow -= nBaseFields;
	FieldId * pId = NULL;
	if (pFieldId)
	{
		FieldId * pField = new FieldId;
		ZeroMemory(pField,sizeof(FieldId));

		if(pFieldId->pLeafPath)
		{
			pField->pLeafPath = new TCHAR[MAX_PATH];
			ZeroMemory(pField->pLeafPath,sizeof(TCHAR)*MAX_PATH);
			lstrcpy(pField->pLeafPath,pFieldId->pLeafPath);
			pField->nDispField = pFieldId->nDispField;
			pField->nValField = pFieldId->nValField;
		}
		std::map<UINT,FieldId *>::const_iterator it = m_mapFieldID.find(nRow);
		if(it != m_mapFieldID.end())
		{
			if(it->second)
				if(it->second->pLeafPath)
					delete it->second->pLeafPath;
				delete it->second;
		}

		m_mapFieldID[nRow] = pField;
		pId = pField;
	}

	if(m_pFields && pId)
	{
		if((nRow<m_nFields))
		{
			m_pFields[nRow].m_pFieldId = pId;
		}
		// 新枚举列的追加，先扩展m_pFields属性列内存，然后设置枚举信息
		else
		{
			LeafField * pLeafFields = new LeafField[nRow+1];
			ZeroMemory(pLeafFields,sizeof(LeafField)*(nRow+1));
			if(m_pFields)
			{
				CopyMemory(pLeafFields,m_pFields,sizeof(LeafField)*m_nFields);
				delete []m_pFields;
			}
			m_pFields = pLeafFields;
			m_nFields = nRow+1;

			// 设置ID映射信息
			m_pFields[nRow].m_pFieldId = pId;
		}
	}
}

void cContactDataLeafNodeDesc::GetFieldID(UINT nRow,FieldId * pFieldId)const 			// 取得数据属性叶节点的属性域对应
{
	UINT nBaseFields = m_pBaseDesc?m_pBaseDesc->GetFieldCount():0;
	if(nRow<nBaseFields)
	{
		m_pBaseDesc->GetFieldID(nRow,pFieldId); 
	}
	else /*if((nRow-nBaseFields)<m_nFieldIds) */
	{
		nRow -= nBaseFields;
		std::map<UINT,FieldId *>::const_iterator it = m_mapFieldID.find(nRow);
		if(it != m_mapFieldID.end())
		{
			if(it->second->pLeafPath && pFieldId->pLeafPath)
				_tcscpy(pFieldId->pLeafPath,it->second->pLeafPath);
			*pFieldId = *(it->second);
		}
		else
		{
			pFieldId->pLeafPath = NULL;
			pFieldId->nValField = 0;
			pFieldId->nDispField = 0;
		}
	}
}
void cContactDataLeafNodeDesc::Build()
{
	delete []m_pFieldNames;
	delete []m_pFields;
	m_nFields = 0;
	m_pFields = NULL;
	m_vecFields.clear();

	// 分配空间，暂时假设所有的行都会成为域
	UINT nRow = m_pSourceNode->GetRows();
	UINT nBaseRows = m_pSourceNode->GetBaseRows();
	if(nRow <= nBaseRows)
		return;

	// 派生类的长度,继承了基类的长度
	if(m_pBaseDesc)
	{
		m_nTempLength = m_pBaseDesc->m_nTempLength;
		m_nInstLength = m_pBaseDesc->m_nInstLength;
	}
	else
	{
		m_nTempLength = 0;
		m_nInstLength = 0;
	}

	nRow -= nBaseRows;
	m_pFields = new LeafField[nRow];	
	ZeroMemory(m_pFields,sizeof(LeafField) * nRow);
	m_pFieldNames = new TCHAR[nRow * MAX_NAME * 2];
	ZeroMemory(m_pFieldNames,sizeof(TCHAR) * nRow * MAX_NAME * 2);

	// 0"字段名" 1"基本数值" 2"需要保存" 3"类型" 4"最小值" 5"最大值" 6"长度" 7"说明"
	for(UINT n=0;n<nRow;n++)
	{
		UINT nCurRow = n+nBaseRows;
		
		m_pFields[m_nFields].m_lpszName = m_pFieldNames + n * MAX_NAME * 2;
		lstrcpy(m_pFields[m_nFields].m_lpszName,m_pSourceNode->GetString(nCurRow,(UINT)(ContactData_Name-1))); 
		
		m_pFields[m_nFields].m_lpszAlias = m_pFieldNames + n * MAX_NAME * 2 + MAX_NAME;
		lstrcpy(m_pFields[m_nFields].m_lpszAlias,m_pSourceNode->GetString(nCurRow,(UINT)(ContactData_Alias-1))); 
		if(!m_pFields[m_nFields].m_lpszAlias[0])
			_stprintf(m_pFields[m_nFields].m_lpszAlias,_T("Field%d"),n+1);

		m_pFields[m_nFields].m_eType = (FieldType)m_pSourceNode->GetInteger(nCurRow,ContactData_Type-1);
		m_pFields[m_nFields].m_nLength = m_pSourceNode->GetInteger(nCurRow,ContactData_Length-1);
		m_pFields[m_nFields].m_nMax = m_pSourceNode->GetInteger(nCurRow,ContactData_Max-1);
		m_pFields[m_nFields].m_nMin = m_pSourceNode->GetInteger(nCurRow,ContactData_Min-1);
		m_pFields[m_nFields].m_nKeyOrder = m_pSourceNode->GetInteger(nCurRow,ContactData_Index-1);


		if(m_pFields[m_nFields].m_nLimitField)
			m_pFields[m_pFields[m_nFields].m_nLimitField-1-nBaseRows].m_nLimitDest = nCurRow+1;

		m_pFields[m_nFields].m_nTemplateOffset = m_nTempLength;

		// 目前非字符串的域长度统统为4
		// todo:不允许长度为0 合法性校验。
		if(m_pFields[m_nFields].m_eType != Field_String)
		{
			m_pFields[m_nFields].m_nLength = 4;
		}

		// 枚举字符串最大最小值校验
		if(m_pFields[m_nFields].m_eType == Field_Enumerate)
		{
			std::map<UINT,FieldEnum *>::iterator it = m_mapFieldEnum.find(n);
			if(it!=m_mapFieldEnum.end())
			{
				m_pFields[m_nFields].m_nMin = 0;
				m_pFields[m_nFields].m_nMax = it->second->nNum;
				m_pSourceNode->SetInteger(nCurRow,ContactData_Min-1,0);
				m_pSourceNode->SetInteger(nCurRow,ContactData_Max-1,it->second->nNum);
				if(m_pFields[m_nFields].m_ppStrings)
					delete m_pFields[m_nFields].m_ppStrings;
				m_pFields[m_nFields].m_ppStrings = new TCHAR * [it->second->nNum];
				ZeroMemory(m_pFields[m_nFields].m_ppStrings,sizeof(TCHAR*)*(it->second->nNum));
				for(int i=0;i<m_pFields[m_nFields].m_nMax-m_pFields[m_nFields].m_nMin;i++)
				{
					m_pFields[m_nFields].m_ppStrings[i] = it->second->pString + i * MAX_NAME;
				}
			}
		}
		else if(m_pFields[m_nFields].m_eType == Field_IdField)
		{
			std::map<UINT,FieldId *>::const_iterator it = m_mapFieldID.find(n);
			if(it!=m_mapFieldID.end())
			{
				// 根据叶节点路径取得叶节点
				cLeafNode * pLeafNode = (cLeafNode *)g_NodeStreamImp.GetRoot()->GetNodeByPath(it->second->pLeafPath); 
				UINT nNum = pLeafNode->GetRows(); 
				
				m_pFields[m_nFields].m_nMin = 0;
				m_pFields[m_nFields].m_nMax = nNum;
				m_pSourceNode->SetInteger(nCurRow,ContactData_Min-1,0);
				m_pSourceNode->SetInteger(nCurRow,ContactData_Max-1,nNum);
				m_pFields[m_nFields].m_pFieldId = it->second;
			}
		}

		m_nTempLength += m_pFields[m_nFields].m_nLength;

		Field f = {m_pFields + m_nFields,m_nFields};
		m_vecFields.push_back(f);
		if(m_nIDField == -1 && lstrcmpi(m_pFields[m_nFields].m_lpszName,_T("ID")) == 0)
			m_nIDField = m_nFields;

		m_nFields ++;
		//}
	}
}

cContactAttribLeafNode::cContactAttribLeafNode(cNodeDesc * pDesc,TCHAR * lpszName,TCHAR * lpszAlias,cBaseNode * pParent)
	:cLeafNode_Imp(pDesc,lpszName,lpszAlias,pParent)
	,m_pTargetDesc(NULL)
{
	m_pTargetDesc = new cContactDataLeafNodeDesc(g_NodeTable[Node_ContactData].pNodeDesc,this,NULL);
}
cContactAttribLeafNode::~cContactAttribLeafNode()
{
	delete m_pTargetDesc;
	m_pTargetDesc = NULL;
}
// 设置数据属性叶节点的枚举字符串
BOOL cContactAttribLeafNode::SetEnumString(UINT nRow,const TCHAR * lpszEnum,UINT nNum)
{
	if(m_pTargetDesc)
	{
		m_pTargetDesc->SetEnumString(nRow,lpszEnum,nNum); 
		return TRUE;
	}	
	return FALSE;
}

const TCHAR * cContactAttribLeafNode::GetEnumString(UINT nRow,UINT& nNum)const 
{
	if(m_pTargetDesc)
	{
		return m_pTargetDesc->GetEnumString(nRow,nNum);
	}
	return NULL;
}

void cContactAttribLeafNode::SetFieldID(UINT nRow,FieldId * pFieldId)	// 设置数据属性叶节点的属性域对应
{
	if(m_pTargetDesc)
	{
		m_pTargetDesc->SetFieldID(nRow,pFieldId); 
	}	
}

void cContactAttribLeafNode::GetFieldID(UINT nRow,FieldId * pFieldId) const	// 取得数据属性叶节点的属性域对应
{
	if(m_pTargetDesc)
	{
		m_pTargetDesc->GetFieldID(nRow,pFieldId);
	}
}