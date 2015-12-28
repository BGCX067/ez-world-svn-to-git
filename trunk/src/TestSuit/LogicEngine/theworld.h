#pragma once

#include <list>
#include <map>
#include <string>
#include <set>
#include "MemoryPool.h"
#include "blockallocator.h"
#include "InstNode.h"

#ifdef UNICODE
typedef std::wstring t_string;
#else
typedef std::string	t_string;
#endif // !UNICODE

class cInstNode_Imp;
using namespace MemPool;
class cWorldImp : public cWorld
{
#ifdef DEFINE_BLOCK_ALLOCATED_MAP
	enum {eBlockSize=10240};
	typedef block_allocated_map<int,cInstNode*,eBlockSize>::iterator IterBlockMap;
	typedef block_allocated_map<int,cInstNode*,eBlockSize>::const_iterator Const_IterBlockMap;
#endif
	cBaseNode *					m_pWorldRoot;		// 根节点
	cUnit **					m_ppUnits;			// 单元列表
	UINT						m_nUnits;
	std::map<int,cUnit*>		m_mapUnitIds;		// 以单元ID索引的单元映射
	std::map<t_string,cUnit*>	m_mapUnitNames;		// 以单元名索引的单元映射
#ifndef DEFINE_BLOCK_ALLOCATED_MAP
	std::map<int,cInstNode*>	m_mapInstNodes;		// 实例ID映射
#else
	block_allocated_map<int,cInstNode*,eBlockSize> m_mapInstNodes;		// 实例ID映射
#endif
	std::map<int,int>			m_mapInstMaxId;		// 每种类型的实例

	std::map<std::string,BOOL>	m_mapIsLoad;		// 避免重复被加载
	cRemain * m_pRemain;
	HMODULE						m_hFormulaMG;		// 公式库句柄
	cFormulaManager *			m_pFormulaManager;	// 公式管理者
	std::string					m_sScript;
	std::map<std::string,int>	m_mapUnitPools;		// 单元内存分配
	pFncGetInstsRelate			m_pRelateListen;	// 关系监听
	CMemoryPool *				m_pMemPool;			// 内存管理器
	DWORD						m_dwTime;			// 当前时间
	cXList<cInstNode_Imp>		m_lstInstPool;		// 实例池

	unsigned					m_lastmemdump;

	void BuildUnits();	
	cBaseNode * LoadFromXmlNode(HANDLE hXmlNode,cBaseNode * pParent);		// 从Xml节点装载
	cBaseNode * LoadFromBinFile(FILE * fp,cBaseNode * pParent);				// 从2进制文件装载
	void SaveToXmlNode(HANDLE hXmlNode,cBaseNode * pNode);					// 保存到Xml节点
	void SaveToBinFile(FILE * fp,cBaseNode * pNode);						// 保存到2进制文件
	void EnumLeafNode(cLeafNode ** ppNode,int& nCount, cBaseNode * pNode);
	cBaseNode * FindFirstNode(cBaseNode * pNode,Node_Type eType);

	void LoadInstFromXmlNode(HANDLE hXmlNode);
	void SaveInstToXmlNode(HANDLE hXmlNode,cInstNode * pNode);

	void SaveToXmlStringTable(HANDLE hXmlNode,cBaseNode * pNode);			// 保存到Xml节点
	void LoadToXmlStringTable(HANDLE hXmlNode);								// 从Xml节点读取
	BOOL HasChinese(const TCHAR * lpszString);
	BOOL Exclude(const TCHAR * lpszPath,int nField);

public:
	cWorldImp();
	~cWorldImp();

	virtual cBaseNode * LoadFromFile(const TCHAR * lpszFile,cBaseNode * pParent);	// 从文件装载
	virtual BOOL SaveToFile(const TCHAR * lpszFile,cBaseNode * pNode,BOOL bBin = FALSE);				// 保存到文件
	virtual BOOL LoadInstInfoFromFile(const TCHAR * lpszFile);						// 从文件加载实例
	virtual BOOL SaveInstInfoToFile(const TCHAR * lpszFile);						// 保存实例到文件
	virtual cBaseNode * CreateWorld();
	virtual void DestroyWorld(cBaseNode ** ppRoot);
	virtual void Update(DWORD dwCurTime);											// 更新
	
	virtual void RefreshUnitTree();
	virtual void RefreshTemplate(cBaseNode * pAttrib);
	virtual void RefreshContactTree();
	virtual void RefreshContact(cBaseNode * pAttrib);

	virtual cLeafNode * GetUnitTemplate(int nID);
	virtual cLeafNode * GetUnitTemplate(const TCHAR * lpszUnit);
	virtual cLeafNode * GetUnitAttrib(int nID);
	virtual cLeafNode * GetUnitAttrib(const TCHAR * lpszUnit);
	virtual cLeafNode * GetUnitBuffer(int nID);
	virtual cLeafNode * GetUnitBuffer(const TCHAR * lpszUnit);
	virtual cLeafNode * GetContactTemplate(int nID);
	virtual cLeafNode * GetContactTemplate(const TCHAR * lpszUnit);
	virtual cLeafNode * GetContactAttrib(int nID);
	virtual cLeafNode * GetContactAttrib(const TCHAR * lpszUnit);
	virtual cLeafNode * GetUnitManager();
	virtual cLeafNode * GetRelation();
	virtual cBaseNode * GetRoot(){return m_pWorldRoot;}
	virtual cLeafNode * GetContactManager();
	virtual void * GetFormulaCenter(){return m_pFormulaManager;}

	virtual int EnumLeafNode(cLeafNode ** ppNode);

	virtual UINT GetUnits() const{return m_nUnits;}					// 取得当前世界定义中单元的数量
	virtual const cUnit * GetUnit(UINT n) const;					// 取得当前世界定义中单元
	virtual const cUnit * GetUnit(int nID) const;					// 用单元ID取得单元
	virtual const cUnit * GetUnit(const TCHAR * lpszUnit) const;	// 用单元ID取得单元
	virtual BOOL IfDeriveFrom(const cUnit * pSrc,const cUnit * pDst)const;		// 判断一个单元是否继承自另外一个单元(可能是间接继承)
	
	virtual cInstNode * CreateInstance(								// 创建类的实例节点
				const cUnit * pUnit,		// 单元指针							
				int nTemplateID,			// 数据模板ID
				int nInstanceID =-1 );		// 实例ID
	virtual cInstNode * GetInstance(int nID)const;			// 用实例ID取得实例指针
	virtual void DestroyInstance(int nID);	
	virtual int GetNextInstHandle(int nID)const;					// 取得实例句柄。hHandle == NULL为第一个
	virtual void DestroyAllInstance();								// 删除所有的实例指针

	virtual BOOL ExportSrcHeader(const TCHAR * lpszPath);					// 输出C++头文件
	virtual BOOL BuildFormulaSrc(const TCHAR * lpszPath);					// 输出单元中的字段公式源文件
	virtual BOOL BuildLuaFrame(const TCHAR * lpszPath);						// 输出LUA脚本框架

	virtual const void * GetTemplateBuf(int nUnitID,UINT nCol,int nValue)const;	// 取得指定单元ID,指定行的缓存

	virtual void SetRemain(cRemain * pRemain){m_pRemain = pRemain;}			// 设置时效回调
	virtual cRemain * GetRemain()const{return m_pRemain;}					// 取得时效回调

	virtual cBaseNode * ImportFromExcel(cBaseNode * pParent,const TCHAR * lpszPath,const TCHAR * lpszNode,const TCHAR * lpszAlias,const TCHAR * lpszType);
	virtual BOOL ExportToExcel(cBaseNode * pNode,const TCHAR * lpszPath);
	virtual void RegistRelateListener(pFncGetInstsRelate pListener){m_pRelateListen = pListener;}

	virtual BOOL ExportStringTable(const TCHAR * lpszFile);
	virtual BOOL ImportStringTable(const TCHAR * lpszFile);

	pFncGetInstsRelate GetInstsRelateListener(){return m_pRelateListen;}
	void CreateGlobalMemPool();
	void DestroyGlobalMemPool();
	void * GetMemory(std::size_t size,int &nId);
	void FreeMemory(std::size_t size,int nId);
	DWORD	GetTime(){return m_dwTime;}
	BOOL ReadConfig(char * lpszConfig);

private:
	BOOL LoadLib();
};


enum DefineString
{
	DSEnum,
	DSFID,
	DSTable,
	DSTree,
	DS_Max,
};

class CWorldDefine
{
	std::map<std::string,std::vector<std::string>> m_mapStrings[DS_Max];
	DefineString GetStringType(std::string &Define);
public:
	CWorldDefine();
	TCHAR * Str(DefineString eDs,const TCHAR * lpszName,UINT nIndex);
};