//--------------------------------------------------------------------
//  Name:			Support.h
//  Copyright:		unknown
//	Author:			Lighter.W
//  Date:			28-05-2003 17:32
//  Description:	Library Shared this only header File
//--------------------------------------------------------------------
//! 这个文件是系统支持库的头文件。
/*! 
	提供了LOG文件，虚拟文件系统，内存管理，变量表系统的纯虚接口。
*/

#ifndef _SUPPORT_H
#define _SUPPORT_H

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <Windows.h>
#include <tchar.h>
#include <sstream>

#ifdef UNICODE
	typedef std::wstringstream t_logstream;
#else
	typedef std::stringstream t_logstream;
#endif // !UNICODE


#ifdef SUPPORT_EXPORTS
	#define SUPPORT_DLL_EXPORT   extern "C" __declspec( dllexport )
	#define SUPPORT_DLL_CLASS	  __declspec( dllexport )
#else
	#define SUPPORT_DLL_CLASS	  __declspec( dllimport )
	#define SUPPORT_DLL_EXPORT   extern "C" __declspec( dllimport )
#endif


//----------------------------------------------------------------------------
//		super class for LOG 
//---------------------------------------------------------------------------
//! LOG 的纯虚接口。
/*!
	LOG 的纯虚接口类。
*/

enum LogType
{
	LT_FATAL,
	LT_ERROR,
	LT_WARN,
	LT_INFO,
	LT_DEBUG,
	LT_MAX,
};

class ILogInstance
{
public:
	virtual bool IsEnabledFor(LogType eType)const=0;
	virtual void Write(LogType eType,const TCHAR *str,const char* file, int line)=0;
	virtual void WriteF(LogType eType,const char* file, int line,const TCHAR * _Format, ...)=0;
};

class ILog
{
public:
	virtual ILogInstance * GetInstance(const TCHAR * lpszName)=0;
	virtual ILogInstance * GetRoot()const=0;
};

// 流模式log
#define LOG(logger, logEvent, logLevel)					\
    {													\
        if(logger && (logger)->IsEnabledFor(logLevel)) {\
            t_logstream _buf;							\
            _buf << logEvent;							\
            (logger)->Write(logLevel,					\
                _buf.str().c_str(), __FILE__, __LINE__);\
        }												\
    }
#define LOG_FATAL(logger,logEvent)	LOG(logger,logEvent,LT_FATAL)
#define	LOG_ERROR(logger,logEvent)	LOG(logger,logEvent,LT_ERROR)
#define	LOG_WARN(logger,logEvent)	LOG(logger,logEvent,LT_WARN)
#define	LOG_INFO(logger,logEvent)	LOG(logger,logEvent,LT_INFO)
#define	LOG_DEBUG(logger,logEvent)	LOG(logger,logEvent,LT_DEBUG)

// printf 模式 log
#define LOGF(logger,logLevel,pszFormat, ...) \
    {													\
        if(logger && (logger)->IsEnabledFor(logLevel)) {\
            (logger)->WriteF(logLevel,					\
             __FILE__, __LINE__,pszFormat, __VA_ARGS__);\
        }												\
    }
#define LOGF_FATAL(logger,pszFormat, ...)	LOGF(logger,LT_FATAL,pszFormat,__VA_ARGS__)
#define	LOGF_ERROR(logger,pszFormat, ...)	LOGF(logger,LT_ERROR,pszFormat,__VA_ARGS__)
#define	LOGF_WARN(logger,pszFormat, ...)	LOGF(logger,LT_WARN,pszFormat,__VA_ARGS__)
#define	LOGF_INFO(logger,pszFormat, ...)	LOGF(logger,LT_INFO,pszFormat,__VA_ARGS__)
#define	LOGF_DEBUG(logger,pszFormat, ...)	LOGF(logger,LT_DEBUG,pszFormat,__VA_ARGS__)


//----------------------------------------------------------------------------
//		super class for Memory 
//---------------------------------------------------------------------------
//! 内存管理的纯虚接口。
/*!
	内存管理的纯虚接口类。一般程序不会直接调用到这个接口，而是通过包含 overload.h 来重载 new 和 delete 的定义，
	达到使用这个内存管理器来管理内存的目的。
*/
typedef TCHAR ALLOC_TYPE;
const ALLOC_TYPE MM_UNKNOWN        = 0;  // Declared as TCHARacters to minimize memory footprint, 
const ALLOC_TYPE MM_NEW            = 1;  //   TCHAR = 1 byte
const ALLOC_TYPE MM_NEW_ARRAY      = 2;  //   enum types = int = 32 bits = 8 bytes on standard machines
const ALLOC_TYPE MM_MALLOC         = 3;
const ALLOC_TYPE MM_CALLOC         = 4;
const ALLOC_TYPE MM_REALLOC        = 5;
const ALLOC_TYPE MM_DELETE         = 6;
const ALLOC_TYPE MM_DELETE_ARRAY   = 7;
const ALLOC_TYPE MM_FREE           = 8;
class IMemManager 
{
public:
	virtual void * Malloc( const char *file, int line, size_t size, ALLOC_TYPE type, void *address = NULL )=0;
	virtual bool Free( void *address, ALLOC_TYPE type )=0;
	virtual bool CheckPtr(void *mem)=0;
	virtual void MemoryInfo(size_t &nPoosUsed, size_t &nAppUsed)=0;
	virtual void DumpAllocs(const TCHAR * file)=0;
};

//----------------------------------------------------------------------------
//		super class for VFS
//---------------------------------------------------------------------------
//! 虚拟文件系统的句柄。
/*!
	虚拟文件系统的句柄类型定义。
*/
class IVFile
{
public:
	virtual int GetSize()const = 0;
	virtual const TCHAR * GetPath()const = 0;
	virtual size_t Read(void * pBuffer) = 0;
	virtual const BYTE * GetBuf()const = 0;
	virtual void Release() = 0;
};

class IFileFindCB
{
public:
	virtual void AddFile( const TCHAR *filename ) = 0;
};

//! 虚拟文件系统的纯虚接口。
/*!
	虚拟文件系统的纯虚接口类。在程序中必须用这个接口来代替标准的文件打开，关闭，读取得功能。
	以便数据的打包发行。
*/
class IVirtualFile
{
public:
	//! 打开一个文件
	/*!
	@param filename 需要打开的文件名。
	@param exception 如果在打开文件中发生了错误，是否抛出异常。
	@return HVFILE 返回值 打开的文件的句柄
	*/
	virtual IVFile * Open( const WCHAR *filename, BOOL bRead, BOOL bCache )=0;
	virtual IVFile * Open( const CHAR *filename, BOOL bRead, BOOL bCache )=0;
	/**
	 * \brief 文件是否存在
	 *
	 * Describe:在不需要打开文件的情况下检查文件是否存在
	 * \param *filename 需要检查的文件名
	 * \return 存在返回TRUE 否则返回FALSE
	 */
	virtual BOOL Exist( const WCHAR *filename )=0;
	virtual BOOL Exist( const CHAR *filename )=0;

	virtual int FindMatchFiles(const WCHAR * lpszPattern, IFileFindCB* pCallBack) = 0;
	virtual int FindMatchFiles(const CHAR * lpszPattern, IFileFindCB* pCallBack) = 0;

	virtual const WCHAR * AbsolutePath( const WCHAR *filename,WCHAR * lpszPath ) = 0;
	virtual const CHAR * AbsolutePath( const CHAR *filename,CHAR * lpszPath ) = 0;

	virtual const WCHAR * RelativePath( const WCHAR *filename ) = 0;
	virtual const CHAR * RelativePath( const CHAR *filename ) = 0;
};

class CVfsReader
{
	IVFile * m_pFile;
public:
	CVfsReader(IVirtualFile * pVFile,const WCHAR *filename,BOOL bRead )
		:m_pFile(NULL)
	{
		if(pVFile)
			m_pFile = pVFile->Open(filename,bRead,FALSE);
	}
	CVfsReader(IVirtualFile * pVFile,const CHAR *filename,BOOL bRead )
		:m_pFile(NULL)
	{
		if(pVFile)
			m_pFile = pVFile->Open(filename,bRead,FALSE);
	}
	CVfsReader(IVirtualFile * pVFile,const WCHAR *filename,BYTE *& pBuf,size_t & size)
		:m_pFile(NULL)
	{
		if(pVFile)
		{
			m_pFile = pVFile->Open(filename,FALSE,FALSE);
			if(m_pFile)
			{
				size = m_pFile->GetSize();
				pBuf = new BYTE[size + 1];
				ZeroMemory(pBuf,sizeof(BYTE)*(size + 1));
				if(m_pFile->Read(pBuf) != 1)
				{
					delete []pBuf;
					pBuf = NULL; 
				}
			}			
		}
	}
	CVfsReader(IVirtualFile * pVFile,const CHAR *filename,BYTE *& pBuf,size_t & size)
		:m_pFile(NULL)
	{
		if(pVFile)
		{
			m_pFile = pVFile->Open(filename,FALSE,FALSE);
			size = m_pFile->GetSize();
			pBuf = new BYTE[size + 1];
			ZeroMemory(pBuf,sizeof(BYTE)*(size + 1));
			if(!m_pFile->Read(pBuf)!=size)
			{
				delete []pBuf;
				pBuf = NULL; 
			}
		}
	}
	~CVfsReader()
	{
		if(m_pFile)
			m_pFile->Release();
	}
	IVFile * GetFile()const{return m_pFile;}
};


//----------------------------------------------------------------------------
//		super class for Efficiency
//---------------------------------------------------------------------------
/** \ingroup Interface
* \brief 效率评估设备,用来统计代码执行所用的时间
*
* Describe:使用cpu的循环周期数，来统计代码执行所使用的cpu循环周期。用来作为程序优化的评估工具
*/

class IEffcRecord
{
public:
	virtual void Begin() = 0;
	virtual void End() = 0;
};

class IEfficiency
{
public:
	/**
	* \brief 开启/关闭效率评估
	*/
	virtual void Enable(bool bEnable) = 0;

	/**
	* \brief 半闭效率评估
	*/
	virtual bool IsEnable()const = 0;

	/**
	* \brief 更新所有效率评估设备的节点
	*
	* Describe:每桢累加.
	*/
	virtual void update() = 0;

	/**
	* \brief 输出当前桢的效率
	*
	* Describe:设备在这里记录效率到文件
	* \param bCurrentFrame 是否只输出当前桢的效率. 否则输出从开始运行到现在的效率统计
	*/
	virtual void DumpFrame() = 0;
	virtual void DumpAll() = 0;

	virtual IEffcRecord * GetRecord(const TCHAR * Name)=0;
};

class ISystem
{
public:
	virtual ILog			* GetSysLog() = 0;
	virtual IMemManager		* GetSysMem() = 0;
	virtual IVirtualFile	* GetSysVfs() = 0;
	virtual IEfficiency		* GetSysEfficiency() = 0;
};

class CLogAble
{
protected:
	ILogInstance * m_pLog;
public:
	CLogAble(const TCHAR * lpszName,ILog * pLog)
		:m_pLog(pLog->GetInstance(lpszName))
	{
	}
};

//--------------------------------------------------------------------
//	extern Public Var
//--------------------------------------------------------------------

SUPPORT_DLL_EXPORT ISystem * SysInit(LPCTSTR lpszLogFile, LPCTSTR lpszBasePath,LPCTSTR lpszExtraPath,LPCTSTR lpszEffcFile);
SUPPORT_DLL_EXPORT void SysShut(ISystem * pSystem);

typedef ISystem * (FnSysInit)(LPCTSTR lpszLogFile, LPCTSTR lpszBasePath,LPCTSTR lpszExtraPath,LPCTSTR lpszEffcFile);
typedef void (FnSysShut)(ISystem * pSystem);

#define GetSysInit_FN "SysInit"
#define GetSysShut_FN "SysShut"

#endif