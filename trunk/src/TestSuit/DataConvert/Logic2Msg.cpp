#ifndef _WIN32_WINNT            // 指定要求的最低平台是 Windows Vista。
#define _WIN32_WINNT 0x0600     // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include "support.h"
#include "dllLoader.h"

#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/dynamic_message.h"

#include "Logic2Msg.h"

#define OVERLOAD
#include "overload.h"

#define GetTheWorld_FN "GetTheWorld"

LogicEngin::LogicEngin()
	:m_pTheWorld(NULL)
	,m_pWorldRoot(NULL)
{
}
LogicEngin::~LogicEngin()
{
	if(m_pWorldRoot && m_pTheWorld)
	{
		m_pTheWorld->DestroyWorld(&m_pWorldRoot);
	}		
}
BOOL LogicEngin::Init(const TCHAR * lpszDLL,const char * lpszFile)
{
	if(!Load(lpszDLL))
		return FALSE;
	FnGetTheWorld * pFnGetTheWorld = (FnGetTheWorld *)GetProcAddress( m_hDll, GetTheWorld_FN);
	if(!pFnGetTheWorld)
	{
		_error("can not get entry function %s\n",GetTheWorld_FN);
		return FALSE;
	}
	m_pTheWorld = pFnGetTheWorld();

	_printf("now loading logic data,please wait...\n");
	m_pWorldRoot = m_pTheWorld->LoadFromFile(lpszFile,NULL);
	if(!m_pWorldRoot)
	{
		_error("can not load world define file %s\n",lpszFile);
		return FALSE;
	}
	return TRUE;
}



extern UnitField g_UnitFields[];

UnitField * FindUnitFields(int nUnit)
{
	UINT n=0;
	while(g_UnitFields[n].nUnitID>0)
	{
		if(g_UnitFields[n].nUnitID == nUnit)
			return &g_UnitFields[n];
		n++;
	}
	assert(0 && _T("单元对照表还没写"));
	return NULL;
}

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


	// 转换编码
const char * ToUtf8(const char * pBits,char * pDst)	
{
	wchar_t pUstr[256] = {0};
	int size = strlen(pBits);
	int uSize = ::MultiByteToWideChar(CP_ACP,0,pBits,size+1,NULL,0);
	size = ::MultiByteToWideChar(CP_ACP,0,pBits,size+1,pUstr,uSize);
	size = ::WideCharToMultiByte(CP_UTF8,0,pUstr,-1,NULL,0,NULL,NULL);
	size = ::WideCharToMultiByte(CP_UTF8,0,pUstr,-1,pDst,size,NULL,NULL);
	pDst[size+1] = 0;
	return pDst;
}

void SetFieldValue(google::protobuf::Message * pMessage,const char * lpszField,const void * value,int nIndex)
{
	char szUtf8[256] = {0};
	std::vector<std::string> vecFields;
	SplitString(lpszField,vecFields,".");
	UINT nFields = vecFields.size();
	const google::protobuf::FieldDescriptor * df;
	for(UINT n=0;n<nFields;n++)
	{
		df = pMessage->GetDescriptor()->FindFieldByName(vecFields[n]);
		if(n == 0 && !df->is_repeated() && nIndex>0)
			return;

		if(df->is_required())
		{
			switch(df->cpp_type())
			{
			case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
				pMessage = pMessage->GetReflection()->MutableMessage(pMessage,df);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
				pMessage->GetReflection()->SetInt32(pMessage,df,*(google::protobuf::int32*)value);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
				pMessage->GetReflection()->SetInt64(pMessage,df,*(google::protobuf::int64*)value);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
				pMessage->GetReflection()->SetUInt32(pMessage,df,*(google::protobuf::uint32*)value);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
				pMessage->GetReflection()->SetUInt64(pMessage,df,*(google::protobuf::uint64*)value);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
				pMessage->GetReflection()->SetDouble(pMessage,df,(double)*(float*)value);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
				pMessage->GetReflection()->SetFloat(pMessage,df,*(float*)value);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
				pMessage->GetReflection()->SetBool(pMessage,df,*(BOOL*)value == TRUE);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
				{
					const google::protobuf::EnumDescriptor * ped = df->enum_type();
					const google::protobuf::EnumValueDescriptor * pvd = ped->FindValueByNumber(*(int*)value);
					assert(pvd);
					pMessage->GetReflection()->SetEnum(pMessage,df, pvd);
				}
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
				pMessage->GetReflection()->SetString(pMessage,df,ToUtf8((const char *)value, szUtf8));	
				break;
			}
		}
		else if(df->is_repeated())
		{
			switch(df->cpp_type())
			{
			case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
				{
					int nSize = pMessage->GetReflection()->FieldSize(*pMessage,df);
					if(nIndex < nSize)
						pMessage = pMessage->GetReflection()->MutableRepeatedMessage(pMessage,df,nIndex);
					else
						pMessage = pMessage->GetReflection()->AddMessage(pMessage,df);
				}
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
				pMessage->GetReflection()->AddInt32(pMessage,df,*(google::protobuf::int32*)value);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
				pMessage->GetReflection()->AddInt64(pMessage,df,*(google::protobuf::int64*)value);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
				pMessage->GetReflection()->AddUInt32(pMessage,df,*(google::protobuf::uint32*)value);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
				pMessage->GetReflection()->AddUInt64(pMessage,df,*(google::protobuf::uint64*)value);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
				pMessage->GetReflection()->AddDouble(pMessage,df,(double)*(float*)value);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
				pMessage->GetReflection()->AddFloat(pMessage,df,*(float*)value);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
				pMessage->GetReflection()->AddBool(pMessage,df,*(BOOL*)value == TRUE);
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
				{
					const google::protobuf::EnumDescriptor * ped = df->enum_type();
					const google::protobuf::EnumValueDescriptor * pvd = ped->FindValueByNumber(*(int*)value);
					assert(pvd);
					pMessage->GetReflection()->SetEnum(pMessage,df, pvd);
				}
				break;
			case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
				pMessage->GetReflection()->AddString(pMessage,df,ToUtf8((const char *)value, szUtf8));	
				break;
			}
		}
		else
		{
			assert(0 && L"不支持optional");
		}
	}
	


	//const google::protobuf::FieldDescriptor * df = pNewTemplate->GetDescriptor()->FindFieldByName("item_info");
}

const char * GetFieldName(cWorld * pWorld,int nReleationID,int nDestUnit,int nField)
{
	int nUnitID = -1;
	cLeafNode * p = pWorld->GetRelation();
	for(UINT n=0;n<p->GetRows();n++)
	{
		const Relation * r = (const Relation *)p->GetRow(n)->GetBuffer();
		if(r->nID == nReleationID)
		{
			nUnitID = (nDestUnit==0)?r->nHandlerUnitID:r->nDestUnitID;
			break;
		}
	}
	if(nUnitID > 0)
	{
		UnitField * pUnitField = FindUnitFields(nUnitID);
		if(pUnitField && nField<pUnitField->nMaxFields)
		{
			const char * fname = pUnitField->lpszFields[nField];
			assert(fname && _T("使用了未声明的域名"));
			return fname;
		}
	}
	assert(0 && _T("关系不正确"));
	return NULL;
}

BOOL ImportTemplate(cWorld	* pWorld,google::protobuf::Message * pTemplateRoot,const std::string & field,Logic::Unit_IDs eUnit,const char * lpszFields[],int nLevelCol)
{
	char szUtf8[256] = {0};
	const google::protobuf::FieldDescriptor * descField = pTemplateRoot->GetDescriptor()->FindFieldByName(field);
	const cUnit * pUnit = pWorld->GetUnit(eUnit);
	if(pUnit && descField)
	{
		const cLeafNode * pTemplate = pUnit->GetLeaf(Node_UnitTemplate); 
		const cLeafNode * pLimite = pUnit->GetLeaf(Node_UnitLimite); 
		const cLeafNode * pEffect = pUnit->GetLeaf(Node_UnitEffect); 
		const cLeafNode * pBuff	  = pUnit->GetLeaf(Node_BuffEffect); 

		// 主模板
		const cLeafNodeDesc * pDesc = (const cLeafNodeDesc *)pTemplate->GetDesc();
		google::protobuf::Message * pNewTemplate = NULL;
		for(UINT n=0;n<pTemplate->GetRows();n++)
		{
			const cLeafRow * pRow = pTemplate->GetRow(n);
			int nLevel = pRow->GetInteger(nLevelCol);	// 可以写成函数指针形式
			if(nLevel == 0)
				pNewTemplate = pTemplateRoot->GetReflection()->AddMessage(pTemplateRoot,descField);
			for(UINT m=0;m<pDesc->GetFieldCount();m++)
			{
				const LeafField * f = pDesc->GetField(m);
				if(f->m_nTemplateOffset >= 0 && lpszFields[m])
				{
					const char * lpszName = f->m_lpszAlias;
					const char * pValue = (const char *)pRow->GetBuffer() + f->m_nTemplateOffset;
					SetFieldValue(pNewTemplate,lpszFields[m],pValue,nLevel);
				}
			}

			// 主记录才有limit
			int nID = pRow->GetInteger("ID");
			if(pLimite)
			{
				const google::protobuf::FieldDescriptor * descLimitField = pNewTemplate->GetDescriptor()->FindFieldByName("limits");
				if(nID>=0 && descLimitField)
				{
					ResultRows r = pLimite->Query(cQuery(nID),TRUE);
					for(UINT x=0;x<r.nCount;x++)
					{
						google::protobuf::Message * pNewLimit = NULL;
						if(nLevel == 0)
							pNewLimit = pNewTemplate->GetReflection()->AddMessage(pNewTemplate,descLimitField);
						else
							pNewLimit = pNewTemplate->GetReflection()->MutableRepeatedMessage(pNewTemplate,descLimitField,x);

						const cLeafRow * rl = pLimite->GetRow(r.nFirst+x);
						const LimitField * l = (const LimitField *)rl->GetBuffer();

						if(l->eDestType == LimitType_Prop)
						{
							if(nLevel == 0)
							{
								pNewLimit->GetReflection()->SetString(pNewLimit,pNewLimit->GetDescriptor()->FindFieldByName("field"),ToUtf8(GetFieldName(pWorld,l->nReleationID,(int)l->eDest,l->nDestField), szUtf8));// 取得目标域名
								pNewLimit->GetReflection()->SetEnum(pNewLimit,pNewLimit->GetDescriptor()->FindFieldByName("op"),pNewLimit->GetDescriptor()->FindFieldByName("op")->enum_type()->FindValueByNumber(l->eOp));
								pNewLimit->GetReflection()->SetEnum(pNewLimit,pNewLimit->GetDescriptor()->FindFieldByName("value_type"),pNewLimit->GetDescriptor()->FindFieldByName("value_type")->enum_type()->FindValueByNumber(l->eValueType));
							}
							pNewLimit->GetReflection()->AddInt32(pNewLimit,pNewLimit->GetDescriptor()->FindFieldByName("value"),l->nValue);
						}
						else
						{
							//暂时不处理自定义属性
						}
					}
				}
			}
			if(pEffect)
			{
				const google::protobuf::FieldDescriptor * descEffectField = pNewTemplate->GetDescriptor()->FindFieldByName("effects");
				if(nID>=0 && descEffectField)
				{
					ResultRows r = pEffect->Query(cQuery(nID),TRUE);
					for(UINT x=0;x<r.nCount;x++)
					{
						google::protobuf::Message * pNewEffect = NULL;
						if(nLevel == 0)
							pNewEffect = pNewTemplate->GetReflection()->AddMessage(pNewTemplate,descEffectField);
						else
							pNewEffect = pNewTemplate->GetReflection()->MutableRepeatedMessage(pNewTemplate,descEffectField,x);

						const cLeafRow * re = pEffect->GetRow(r.nFirst+x);
						const EffectValue * e = (const EffectValue *)re->GetBuffer();

						if(nLevel == 0)
						{
							pNewEffect->GetReflection()->SetString(pNewEffect,pNewEffect->GetDescriptor()->FindFieldByName("field"),ToUtf8(GetFieldName(pWorld,e->nRelationID,(int)e->eDest,e->nOperField), szUtf8));// 取得目标域名
							pNewEffect->GetReflection()->SetEnum(pNewEffect,pNewEffect->GetDescriptor()->FindFieldByName("op"),pNewEffect->GetDescriptor()->FindFieldByName("op")->enum_type()->FindValueByNumber(e->eOp));
							pNewEffect->GetReflection()->SetEnum(pNewEffect,pNewEffect->GetDescriptor()->FindFieldByName("type"),pNewEffect->GetDescriptor()->FindFieldByName("type")->enum_type()->FindValueByNumber(0));
						}
						pNewEffect->GetReflection()->AddFloat(pNewEffect,pNewEffect->GetDescriptor()->FindFieldByName("value"),e->nValue);
					}
				}
			}
			if(pBuff)
			{

			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL ImportContact(cWorld	* pWorld,google::protobuf::Message * pTemplateRoot,Logic::Contact_IDs eContact,const char * lpszIndices[],const char * lpszFields[],UINT nIdxLevel)
{
	char szUtf8[256] = {0};
	const cLeafNode * pTemplate = pWorld->GetContactTemplate(eContact);
	if(pTemplate)
	{
		const cLeafNodeDesc * pDesc = (const cLeafNodeDesc *)pTemplate->GetDesc();
		google::protobuf::Message * pNewTemplate[4] = {NULL,NULL,NULL,NULL};
		int nLastValue[4] = {-1,-1,-1,-1};
		for(UINT n=0;n<pTemplate->GetRows();n++)
		{
			const cLeafRow * pRow = pTemplate->GetRow(n);
			for(UINT m=0;m<nIdxLevel;m++)
			{
				const LeafField * f = pDesc->GetField(m);
				const char * lpszName = f->m_lpszAlias;
				const char * pValue = (const char *)pRow->GetBuffer() + f->m_nTemplateOffset;

				int nNewValue = *(int*)pValue;
				if(nNewValue != nLastValue[m])
				{
					// 加入新的Message
					google::protobuf::Message * pParent = (m==0)?pTemplateRoot:pNewTemplate[m-1];
					const google::protobuf::FieldDescriptor * descField = pParent->GetDescriptor()->FindFieldByName(lpszIndices[m]);

					pNewTemplate[m] = pParent->GetReflection()->AddMessage(pParent,descField);
					nLastValue[m] = nNewValue;
				}
			}
			const google::protobuf::FieldDescriptor * descField = pNewTemplate[nIdxLevel-1]->GetDescriptor()->FindFieldByName(lpszIndices[nIdxLevel]);
			google::protobuf::Message * pNewMessage = pNewTemplate[nIdxLevel-1]->GetReflection()->AddMessage(pNewTemplate[nIdxLevel-1],descField);
			for(UINT m=nIdxLevel;m<pDesc->GetFieldCount();m++)
			{
				const LeafField * f = pDesc->GetField(m);
				const char * lpszName = f->m_lpszAlias;
				const char * value = (const char *)pRow->GetBuffer() + f->m_nTemplateOffset;

				const google::protobuf::FieldDescriptor * df = pNewMessage->GetDescriptor()->FindFieldByName(lpszFields[m-nIdxLevel]);
				switch(df->cpp_type())
				{
				case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
					assert(0 && _T("这里不能是消息类型"));
					break;
				case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
					pNewMessage->GetReflection()->SetInt32(pNewMessage,df,*(google::protobuf::int32*)value);
					break;
				case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
					pNewMessage->GetReflection()->SetInt64(pNewMessage,df,*(google::protobuf::int64*)value);
					break;
				case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
					pNewMessage->GetReflection()->SetUInt32(pNewMessage,df,*(google::protobuf::uint32*)value);
					break;
				case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
					pNewMessage->GetReflection()->SetUInt64(pNewMessage,df,*(google::protobuf::uint64*)value);
					break;
				case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
					pNewMessage->GetReflection()->SetDouble(pNewMessage,df,(double)*(float*)value);
					break;
				case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
					pNewMessage->GetReflection()->SetFloat(pNewMessage,df,*(float*)value);
					break;
				case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
					pNewMessage->GetReflection()->SetBool(pNewMessage,df,*(BOOL*)value == TRUE);
					break;
				case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
					{
						const google::protobuf::EnumDescriptor * ped = df->enum_type();
						const google::protobuf::EnumValueDescriptor * pvd = ped->FindValueByNumber(*(int*)value);
						assert(pvd);
						pNewMessage->GetReflection()->SetEnum(pNewMessage,df, pvd);
					}
					break;
				case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
					pNewMessage->GetReflection()->SetString(pNewMessage,df,ToUtf8((const char *)value, szUtf8));	
					break;
				}				
			}
		}
		return TRUE;
	}
	return FALSE;
}