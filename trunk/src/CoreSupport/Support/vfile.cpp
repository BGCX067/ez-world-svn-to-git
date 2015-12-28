//--------------------------------------------------------------------
//  Name:			File.h
//  Copyright:		unknown
//	Author:			Lighter.W
//  Date:			9:38 2003-5-29
//  Description:	文件存取类
// 	Classes:
// 		CSysFiles			- 全局文件存取函数,在 System 初始化时被初始化
// 		VFile:CSysFiles		- 只读文件装载器
// 		VFileSearch			- 用给定的模板搜索文件
// 	Functions:
// 		f_StripName			- 从路径名中去掉文件名
// 		f_Name				- 从路径名中取得文件名
// 		f_Extension			- 从文件名中取得扩展名
// 		f_StripExtension	- 从文件名中取得扩展名
//  TODO: 
// 		- 把压缩文件的链表管理加入 CSysFiles 类中
// 		- 文件查找采取 2 分法
// 		- 制定新的文件格式允许新压缩算法,减低压缩率保证时间
//--------------------------------------------------------------------

#include <io.h>
#include <direct.h>
#include <assert.h>
#include <shlwapi.h>

#include "support.h"
#include "vfile.h"
#include "output.h"

#include "strcode.h"
#include "ZipArchive.h"
#define OVERLOAD
#include "overload.h"

CSysFiles * Singleton<CSysFiles>::ms_Singleton = NULL;

int CVFile::m_nFileOpend = 0;

CVFile::CVFile()
	:m_nSize(0)
	,m_fp(NULL)
	,m_pPak(NULL)
	,m_wIndex(0)
	,m_pBuf(NULL)
	,m_bCached(FALSE)
{
	m_szFname[0] = 0;
}
CVFile::~CVFile()
{
	if(m_fp)
	{
		fclose(m_fp);
		m_nFileOpend --;
	}

	if(m_pPak && m_wIndex != ZIP_FILE_INDEX_NOT_FOUND)
	{
		::EnterCriticalSection((LPCRITICAL_SECTION)CSysFiles::getSingletonPtr()->GetCS());
		m_pPak->CloseFile();
		m_nFileOpend --;
		assert(m_nFileOpend == 0);
		::LeaveCriticalSection((LPCRITICAL_SECTION)CSysFiles::getSingletonPtr()->GetCS());
	}
	if(m_pBuf)
		delete []m_pBuf;
}
void CVFile::Release()
{
	if(!m_bCached)
		delete this;
}
BOOL CVFile::Open(const TCHAR * filename, BOOL bRead,BOOL bCache)
{	
	_tcscpy(m_szFname, filename);
	f_UnixToDosName(m_szFname);

	BOOL bOpend = FALSE;
	// 先在压缩包文件列表中寻找
	m_pPak = CSysFiles::getSingletonPtr()->FindInPack(m_szFname, m_szFname, m_wIndex, m_nSize);
	if (m_pPak)
	{
		::EnterCriticalSection((LPCRITICAL_SECTION)CSysFiles::getSingletonPtr()->GetCS());
		if(m_pPak->OpenFile(m_wIndex))
		{
			assert(m_nFileOpend == 0 && "VFileSystem can only open ONE file in one time.please close last file before open new one.");
			m_nFileOpend ++;
			bOpend = TRUE;
		}
		::LeaveCriticalSection((LPCRITICAL_SECTION)CSysFiles::getSingletonPtr()->GetCS());
	}

	// 压缩包中没有，再搜索实际路径
	if(!bOpend && CSysFiles::getSingletonPtr()->FindInPath(m_szFname, m_szFname, m_nSize))
	{
		::SetLastError(0);
		if(m_fp=_tfopen(m_szFname,_T("rb"))) 
		{
			m_nFileOpend ++;
			bOpend = TRUE;
		}
	}
	
	if(bOpend && bRead)
	{
		m_pBuf = new BYTE[m_nSize+1];
		ZeroMemory(m_pBuf,m_nSize+1);
		Read(m_pBuf);
	}
	m_bCached = bCache;
	return bOpend;
}
size_t CVFile::Read(void * buffer)
{
	size_t len = 0;
	if( m_fp || m_pPak)
	{
		if(m_fp)
			len = fread(buffer, m_nSize, 1, m_fp);

		else if(m_pPak)
		{
			// decompress data
			len = m_pPak->ReadFile(buffer, (DWORD)m_nSize);
			if ( len != (DWORD)m_nSize)
			{
				assert(0);
			}
		}
	}
	return len;
}
/*---------------------------------------------------------------------------
	CSysFiles Class Static Function
---------------------------------------------------------------------------*/

// 初始化函数,取得当前路径
CSysFiles::CSysFiles(const TCHAR * lpszMainPath,const TCHAR * lpszExtraPath)
	:CLogAble(_T("System.SysFiles"),CLog::getSingletonPtr())
{
	// 所有數據打到一個包內.
	FormatPath(lpszMainPath,main_path);
	LoadPacks(main_path);

	// 附加一個包用作更新用
	TCHAR extra_path[MAXFILEPATH] = {0};		// 額外工作目录.
	FormatPath(lpszExtraPath,extra_path);
	LoadPacks(extra_path);

	// 統計裝載的包的數量
	std::list<Pack>::iterator i = m_lstPackets.begin();
	int nPackedFiles = 0;
	while(i != m_lstPackets.end())
	{
		LOG_INFO(m_pLog,i->sName.c_str());
		LOG_INFO(m_pLog,_T("\t")<<i->pPak->GetCount()<<_T(" file(s) in pack"));
		i ++; 
	}
	LOG_INFO(m_pLog,m_lstPackets.size()<<_T(" packed files"));
}

void CSysFiles::FormatPath(const TCHAR * lpszPath,TCHAR * lpszDst)
{
	if(!PathIsRelative(lpszPath))
	{
		// 绝对路径直接拷贝
		_tcsncpy(lpszDst, lpszPath, MAXFILEPATH);
	}
	else
	{
		// 相对路径转化成绝对路径保存
		TCHAR szWorkPath[255],szDst[255];
		::GetCurrentDirectory(255,szWorkPath);
		_tcscat(szWorkPath,_T("/"));
		_tcscat(szWorkPath,lpszPath);
		f_UnixToDosName(szWorkPath);
		PathCanonicalize(szDst,szWorkPath);
		_tcsncpy(lpszDst, szDst, MAXFILEPATH);
	}
}

// 装载压缩文件
void CSysFiles::LoadPacks(const TCHAR * lpszPath)
{
	TCHAR fname[512] = {0};					// FIXME: static allocation

	// 从搜索器中取得的名字和路径
	_tcscpy(fname, lpszPath);
	// 末尾的斜杠换成 .pk3
	fname[_tcslen(fname)-1] = 0;
	_tcscat(fname,_T(".pk3"));

	if(PathFileExists(fname) && !PathIsDirectory(fname))
	{
		// 打开压缩包文件，把压缩包文件信息加入链表
		CZipArchive * OnePacket;
		OnePacket = new CZipArchive;
		if (!OnePacket->Open(fname,CZipArchive::zipOpenReadOnly))
		{
			delete OnePacket;
			return;
		}
		OnePacket->EnableFindFast();

		// 加入到链表
		Pack packet = {fname,OnePacket};
		m_lstPackets.push_back(packet);
	}
}

// 关闭全局文件存取系统
CSysFiles::~CSysFiles()
{
	FileCacheMap::iterator it = m_mapFileCache.begin();
	while(it != m_mapFileCache.end())
	{
		delete it->second;
		it ++;
	}
	
	std::list<Pack>::iterator i = m_lstPackets.begin();
	while(i != m_lstPackets.end())
	{
		delete i->pPak;
		i ++; 
	}
}

// 在压缩包内寻找文件，返回包含这个文件的压缩包名
CZipArchive * CSysFiles::FindInPack(const TCHAR *name, TCHAR * path, WORD& index, int& size)
{
	CZipArchive * zip = NULL;
	size = -1;
	index = ZIP_FILE_INDEX_NOT_FOUND; 
	
	// 寻找文件名相同的文件
	std::list<Pack>::iterator i = m_lstPackets.begin();
	while(i != m_lstPackets.end())
	{
		index = i->pPak->FindFile(name);
		if(index != ZIP_FILE_INDEX_NOT_FOUND)
		{
			zip = i->pPak;
			_tcscpy(path,i->sName.c_str());

			CZipFileHeader * header = zip->GetFileInfo(index);
			if(header)
				size = header->m_uUncomprSize;

			break;
		}
		i ++;
	}
	return zip;
}

// 在实际路径查找文件，返回包含这个文件的路径。
BOOL CSysFiles::FindInPath(const TCHAR *fname, TCHAR * path, int& size)	
{
	TCHAR szFile[MAX_PATH] = {0};
	_tcscpy_s(szFile,MAX_PATH,main_path);
	_tcscat_s(szFile,MAX_PATH,fname);
	
	WIN32_FIND_DATA wfd;
	HANDLE hFind = ::FindFirstFile(szFile, &wfd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do 
		{
			if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				size = wfd.nFileSizeLow;
				_tcscpy_s(path,MAX_PATH,szFile);
				return TRUE;
			}

		} while (::FindNextFile(hFind, &wfd));
		FindClose(hFind);
	}
	return FALSE;
}


IVFile * CSysFiles::Open( const WCHAR *filename, BOOL bRead, BOOL bCache)
{
	// 空文件名直接返回
	if(!filename)
		return NULL;
	// 最后一个字符是非法字符，就不用找了
	size_t len = _tcslen(filename);
	if( (!len) || (filename[len-1] == _T('/')) || (filename[len-1] == _T('\\')))
		return NULL;

	// 先在 cache里找
	FileCacheMap::iterator it = m_mapFileCache.find(filename);
	if(it != m_mapFileCache.end())
		return it->second;
	
	
	CVFile * myfile = new CVFile;
	if(myfile->Open(filename,bRead||bCache,bCache))
	{
		if(bCache)
		{
			std::pair<t_string, CVFile *> p(filename,myfile);
			m_mapFileCache.insert(p);
		}
		return myfile;
	}
	else
	{
		DWORD dwError = GetLastError();
		if(dwError != 0)
		{
			LOG_ERROR(m_pLog,_T("CSysFiles::Open()" )<<filename<<_T(" failed.ErrorCode = ")<<dwError);

			LPVOID lpMsgBuf;
			if (FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dwError,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL ))
			{
				LOG_ERROR(m_pLog,_T("CSysFiles::Open()" )<<_T(" ,Message = ")<<lpMsgBuf);
			}
			// Free the buffer.
			LocalFree( lpMsgBuf );
		}
		else
			LOG_ERROR(m_pLog,_T("CSysFiles::Open()" )<<filename<<_T(" failed"));
	}

	delete myfile;
	return NULL;
}

BOOL CSysFiles::Exist( const WCHAR *filename )
{
	TCHAR path[MAXFILEPATH]={0};
	int size = 0;
	WORD wIndex = -1;

	if(FindInPack(filename,path, wIndex, size))
		return TRUE;
	if(FindInPath(filename, path, size))
		return TRUE;
	return FALSE;
}

const WCHAR * CSysFiles::AbsolutePath( const WCHAR *filename,WCHAR * lpszPath )
{
	lpszPath[0] = 0;
	_tcscpy_s(lpszPath,MAX_PATH,main_path);
	_tcscat_s(lpszPath,MAX_PATH,filename);
	return lpszPath;
}
const WCHAR * CSysFiles::RelativePath( const WCHAR *filename )
{
	TCHAR szWorkPath[MAX_PATH] = {0};
	_tcscpy_s(szWorkPath,MAX_PATH,filename);
	_tcslwr(szWorkPath);

	const TCHAR * pDest = _tcsstr(szWorkPath, main_path);
	if(pDest && (pDest == szWorkPath))
		return filename + _tcslen(main_path);
	return NULL;
}

int CSysFiles::FindMatchFiles(const WCHAR * lpszPattern, IFileFindCB* pCallBack)
{
	if (!pCallBack)
		return 0;

	int nCount = 0;
	bool found = false;
	
	//先在压缩包中找，如果没有再到文件系统中找
	std::list<Pack>::iterator it = m_lstPackets.begin();
	while(it != m_lstPackets.end())
	{
		TCHAR lowerName[MAX_PATH];
		CZipIndexesArray indices;
		it->pPak->FindMatches(lpszPattern, indices);
		for (UINT i = 0; i < indices.GetCount(); i++)
		{
			CZipFileHeader *file = it->pPak->GetFileInfo(indices[i]);
			memset(lowerName, 0, sizeof(lowerName));
			_tcscpy(lowerName, file->GetFileName());
			_tcslwr(lowerName);
			t_string fileName(lowerName);
			std::replace(fileName.begin(), fileName.end(), _T('/'), _T('\\'));
			pCallBack->AddFile(fileName.c_str());
			found = true;
			nCount ++;
		}
		it ++;
	}

	//在文件系统中找，但有以下约束:
    //1: 只支持路径＋通配符的方式，如c:\temp\*.*，不支持路径中间带通配符，如c:\temp*\abc\*.txt
	//2: 不支持子目录中查找
	if (!found)
	{
		WIN32_FIND_DATA wfd;
		HANDLE hFind = ::FindFirstFile(lpszPattern, &wfd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do 
			{
				if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
					TCHAR fullName[MAX_PATH];
					memset(fullName, 0, sizeof(fullName));
					_tcscpy(fullName, main_path);
					_tcscat(fullName, wfd.cFileName);
					_tcslwr(fullName);
					t_string s = t_string(fullName);
					std::replace(s.begin(), s.end(), _T('/'), _T('\\'));
					pCallBack->AddFile(s.c_str());
					nCount ++;
				}

			} while (::FindNextFile(hFind, &wfd));
			FindClose(hFind);
		}
	}
	return nCount;
}

IVFile * CSysFiles::Open( const CHAR *filename, BOOL bRead, BOOL bCache )
{
	return Open(CMbsToWcs(filename).Get(),bRead,bCache);
}
BOOL CSysFiles::Exist( const CHAR *filename )
{
	return Exist(CMbsToWcs(filename).Get());
}
int CSysFiles::FindMatchFiles(const CHAR * lpszPattern, IFileFindCB* pCallBack)
{
	return FindMatchFiles(CMbsToWcs(lpszPattern).Get(),pCallBack);
}
const CHAR * CSysFiles::AbsolutePath( const CHAR *filename,CHAR * lpszPath )
{
	WCHAR szPath[MAX_PATH] = {0};
	AbsolutePath(CMbsToWcs(filename).Get(),szPath);
	strcpy_s(lpszPath,MAX_PATH,CWcsToMbs(szPath).Get());
	return lpszPath;
}
const CHAR * CSysFiles::RelativePath( const CHAR *filename )
{
	CHAR szWorkPath[MAX_PATH] = {0};
	strcpy_s(szWorkPath,MAX_PATH,filename);
	_strlwr(szWorkPath);

	const CHAR * pMainPath = CWcsToMbs(main_path).Get();
	
	const CHAR * pDest = strstr(szWorkPath, pMainPath);
	if(pDest && (pDest == szWorkPath))
		return filename + strlen(pMainPath);
	return NULL;
}


// CAUTION: string must be well formated ('\0' at the end)
void f_StripExtension (TCHAR *path){

	size_t length = _tcslen(path)-1;	// ?
	while (length > 0 && path[length] != _T('.')){
		length--;
		if (path[length] == _T('/'))
			return;		// no extension
	}
	if (length)
		path[length] = 0;
}

void f_StripName(TCHAR *path){
	size_t length;
	length = _tcslen(path)-1;	// ?
	while (length > 0 && path[length] != _T('/') && path[length] != _T('\\')){
		length--;
	}
	if (length)
		path[length+1] = 0;
	else 
		path[0] = 0;
}

TCHAR * f_Name(TCHAR *path){
	size_t length, l;
	l = length = _tcslen(path)-1;
	while (length > 0 && path[length] != _T('\\') && path[length] != _T('/')){
		length--;
	}
	return &path[length+1];
}

TCHAR * f_Extension(TCHAR *name){
	size_t length, l;
	l = length = _tcslen(name)-1;	// ?
	while (length > 0 && name[length] != _T('.')){
		length--;
		if (name[length] == '/')
			return &name[l];		// no extension
	}
	if(length==0) return &name[l];
	return &name[length];
}
