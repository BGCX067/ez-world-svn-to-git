//--------------------------------------------------------------------
//  Name:			File.h
//  Copyright:		unknown
//	Author:			Lighter.W
//  Date:			9:38 2003-5-29
//  Description:	文件存取类
// 	Classes:
// 		VFileEnv			- 虚拟文件系统的环境设置，维护工作路径，压缩文件管理
//		CSysFiles			- 文件系统接口的实现类
// 	Functions:
// 		f_StripName			- 从路径名中去掉文件名
// 		f_Name				- 从路径名中取得文件名
// 		f_Extension			- 从文件名中取得扩展名
// 		f_StripExtension	- 从文件名中取得扩展名
//  TODO: 
// 		- 把压缩文件的链表管理加入 VFileEnv 类中
// 		- 文件查找采取 2 分法
// 		- 制定新的文件格式允许新压缩算法,减低压缩率保证时间
//--------------------------------------------------------------------
#ifndef _LVFILE_INCLUDE
#define _LVFILE_INCLUDE

#include <list>
#include <map>
#include "Singleton.h"
#include "CSLock.h"
#include "tstring.h"

#define MAXFILEPATH		512			// maximum length for complete path
#define MAXFILENAME		64			// maximum length for single path component

/*---------------------------------------------------------------------------
	Functions
---------------------------------------------------------------------------*/
TCHAR * f_Name(TCHAR *path);				// 从路径名中取得文件名
void f_StripName(TCHAR *path);				// 从路径名中去掉文件名
TCHAR * f_Extension(TCHAR *name);			// 从文件名中取得扩展名
void f_StripExtension (TCHAR *path);		// 从文件名中去掉扩展名
inline void f_DosToUnixName(TCHAR *name)	
{
	for(int i=0; ; i++)
	{
		if(name[i]==_T('\0')) break;
		if(name[i]==_T('\\')) name[i]=_T('/');
	}
}

inline void f_UnixToDosName(TCHAR *name)
{
	for(int i=0; ; i++)
	{
		if(name[i]==_T('\0')) break;
		if(name[i]==_T('/')) name[i]=_T('\\');
	}
}


/*---------------------------------------------------------------------------
	类
---------------------------------------------------------------------------*/

class CZipArchive;
class CVFile : public IVFile
{
	TCHAR		m_szFname[MAXFILEPATH];	// 文件名
	int			m_nSize;				// 文件大小
	FILE *		m_fp;					// 文件指针
	CZipArchive *	m_pPak;				// 文件所在的压缩包指针
	WORD		m_wIndex;				// 在包内的索引
	BYTE *		m_pBuf;					// 文件数据内容
	BOOL		m_bCached;				// 被缓存的文件,不能直接释放掉.
	static int	m_nFileOpend;
public:
	CVFile();
	~CVFile();
	BOOL Open(const TCHAR *filename,BOOL bRead,BOOL bCache);

	int GetSize()const{return m_nSize;}
	const TCHAR * GetPath()const{return m_szFname;}
	size_t Read(void * pBuffer);
	const BYTE * GetBuf()const{return m_pBuf;}
	void Release();
};

//
//	全局文件存取类,维护程序数据文件在实际磁盘的搜索路径
//	路径下所有 .PK3 文件的链表,其中包含了 ,pk3 文件内的
//	的文件信息
//
// TODO: 增加写模式，文本模式，增加 vfile 句柄的校验（添加标志符）
class CSysFiles : public Singleton<CSysFiles>, public CLogAble, public IVirtualFile
{
	// .pk3 文件链表
	typedef struct Pack_s {
		t_string sName;					// pk3文件名
		CZipArchive * pPak;				// 压缩包指针
	}Pack;

	std::list<Pack>	m_lstPackets;
	TCHAR main_path[MAXFILEPATH];		// 主工作目录.

	CriticalSection m_CS;

	typedef std::map<t_string, CVFile *> FileCacheMap;
	FileCacheMap m_mapFileCache;


	void LoadPacks(const TCHAR * lpszPath);								// 搜索指定查找路径下所有的.pk3文件,形成链表,保存 .pk3 包内的文件名
	void FormatPath(const TCHAR * lpszPath,TCHAR * lpszDst);	

public:
	CSysFiles(const TCHAR * lpszMainPath,const TCHAR * lpszExtraPath);	// 初始化,取得当前工作目录
	~CSysFiles(void);					// 关闭

	BOOL FindInPath(const TCHAR *name,			// 在实际路径查找文件，返回包含这个文件的路径。
							TCHAR * path,		// 最后找到的文件名
							int& size);			// 找到的文件大小	
	CZipArchive * FindInPack(const TCHAR *name,	// 在压缩包内寻找文件，返回包含这个文件的压缩包指针
							TCHAR * path,		// 最后找到的文件名 
							WORD& index,		// 在压缩包中的索引
							int& size);			// 文件大小
	CRITICAL_SECTION * GetCS(){return &m_CS.GetCS();}

	IVFile * Open( const WCHAR *filename, BOOL bRead, BOOL bCache );
	IVFile * Open( const CHAR *filename, BOOL bRead, BOOL bCache );
	BOOL Exist( const WCHAR *filename );
	BOOL Exist( const CHAR *filename );
	int FindMatchFiles(const WCHAR * lpszPattern, IFileFindCB* pCallBack);
	int FindMatchFiles(const CHAR * lpszPattern, IFileFindCB* pCallBack);
	const WCHAR * AbsolutePath( const WCHAR *filename,WCHAR * lpszPath );
	const CHAR * AbsolutePath( const CHAR *filename,CHAR * lpszPath );
	const WCHAR * RelativePath( const WCHAR *filename );
	const CHAR * RelativePath( const CHAR *filename );
};

#endif // _FILE_INCLUDE