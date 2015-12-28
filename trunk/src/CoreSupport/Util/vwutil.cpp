#include "StdAfx.H"
#include <assert.h>
#include <math.h>
#include <windows.h>
#include <stdio.h>
//#include "vwassert.h"
#include "vwutil.h"


/***************************/
//分区                      低位	高位     
//----------------------------------------------
//●GBK/1:GB2312非汉字符号: A1~A9 || A1~FE
//●GBK/2:GB2312汉字      : B0~F7 || A1~FE
//●GBK/3:扩充汉字        : 81~A0 || 40~FE
//●GBK/4:扩充汉字        : AA~FE || 40~A0
//●GBK/5:扩充非汉字      : A8~A9 || 40~A0
//● 用户自定义区 (1)	  :	AA~AF || A1~FE
//● 用户自定义区 (2)     : F8~FE || A1~FE
//● 用户自定义区 (3)     : A1~A7 || 40~A0
bool IsGBKCharacter(unsigned character)
{//这个判决不包含用户自定义区
	BYTE hb,lb;
	hb=((character&0xff00)>>8);
	lb=(character&0xff);
	if(lb<=0xa9&&lb>=0xa1&&hb>=0xa1&&hb<=0xfe)
		return true;
	if(lb>=0xb0&&lb<=0xf7&&hb>=0xa1&&hb<=0xfe)
		return true;
	if(lb<=0xa0&&lb>=0x81&&hb>=0x40&&hb<=0xfe)
		return true;
	if(lb<=0xfe&&lb>=0xaa&&hb>=0x40&&hb<=0xa0)
		return true;
	return (lb<=0xa9&&lb>=0xa8&&hb>=0x40&&hb<=0xa0);
}

#if _MSC_VER <1400
unsigned int strnlen(const char *str,unsigned int len)
{
#if 0
	_asm{
		mov ecx,len;
		mov di,str;
		xor al,al;
		cld;
		repnz scasb;
		mov eax,len;
		sub eax,ecx;
	};
#else
	unsigned int i;
	for(i=0;i<len;i++)
	{
		if(!str[i])break;
	}
	return i;
#endif
}
#endif
const char * _stdcall LoadFileToMemory(const TCHAR *pfilename,DWORD *pfilelength,DWORD offset)
{
	void *hfilein=INVALID_HANDLE_VALUE,*hmap=NULL;
	char *psource=NULL;
	__try{

		hfilein=::CreateFile(pfilename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hfilein==INVALID_HANDLE_VALUE){
			assert(0);
			//SetError("Can't open file for read!");
			return 0;
		}
		DWORD filelength;
		filelength=GetFileSize(hfilein,NULL);
		if(!filelength)
			return NULL;
		if(pfilelength)
		{
			*pfilelength=filelength;
		}
		hmap=CreateFileMapping(hfilein,NULL,PAGE_READONLY,0,filelength,0);
		assert(hmap);
		if(!hmap)return 0;
		psource=(char *)MapViewOfFile(hmap,FILE_MAP_READ,0,offset,filelength);
		assert(psource);
		return psource;
	}
	__finally{
		if(hfilein!=(void *)(-1)){
			CloseHandle(hfilein);
		}
		if(hmap)CloseHandle(hmap);
	}
}

char * _stdcall LoadFileToMemoryW(const TCHAR *pfilename,DWORD offset,DWORD *pfilelength)
{
	void *hfilein=(void *)(-1),*hmap=NULL;
	char *psource=NULL;
	__try{

		hfilein=::CreateFile(pfilename,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hfilein==(void *)(-1)){
			assert(0);
			//SetError("Can't open file for read!");
			return 0;
		}
		assert(pfilelength);
		//if(!pfilelength){
		//SetError("Invalid parameter!");
		//	return 0;
		//}
		DWORD filelength;
		if(pfilelength)
		{
			*pfilelength=GetFileSize(hfilein,NULL);
			if(!*pfilelength)*pfilelength=65536;
		}else{
			filelength=GetFileSize(hfilein,NULL);
			if(!filelength)filelength=65536;
		}
		hmap=CreateFileMapping(hfilein,NULL,PAGE_READWRITE,0,pfilelength?offset+*pfilelength:filelength,0);
		assert(hmap);
		if(!hmap)return 0;
		psource=(char *)MapViewOfFile(hmap,FILE_MAP_WRITE,0,offset,pfilelength?*pfilelength:filelength);
		assert(psource);
		return psource;
	}
	__finally{
		if(hfilein!=(void *)(-1)){
			CloseHandle(hfilein);
		}
		if(hmap)CloseHandle(hmap);
	}
}


int _stdcall UnloadFileFromMemory(const void  *pmem)
{
	if(!pmem)return -1;
	UnmapViewOfFile(pmem);
	return 0;
}

char *_stdcall CreateShareMemory(const TCHAR *szName,HANDLE &hMapFile,DWORD memorysize)
{
	char *pBuf;

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // max. object size 
		memorysize,              // buffer size  
		szName);                 // name of mapping object

	if (hMapFile == NULL) 
	{
		assert(0);
		return NULL;
	}
	pBuf = (char *) MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,                   
		0,                   
		memorysize);           

	if (pBuf == NULL) 
	{
		assert(0);
		return NULL;
	}

	return pBuf;
}

int _stdcall UnloadShareMemory(const void  *pmem,HANDLE hfilemap)
{
	if(!pmem)return -1;
	UnmapViewOfFile(pmem);
	CloseHandle(hfilemap);
	return 0;
}

char  *_stdcall OpenShareMemory(const TCHAR *szName)
{
	HANDLE hMapFile;
	char *pBuf;

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		szName);               // of mapping object 

	if (hMapFile == NULL) 
	{ 
		//printf("Could not open file mapping object (%d).\n", 
		//	GetLastError());
		return NULL;
	} 

	pBuf = (char *)MapViewOfFile(hMapFile,    // handle to mapping object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,                    
		0,                    
		0);                   

	if (pBuf == NULL) 
	{ 
		assert(0);
		return NULL;
	}

	CloseHandle(hMapFile);
	return pBuf;
}

int _stdcall UnloadMemory(const void  *pmem)
{
	if(!pmem)return -1;
	UnmapViewOfFile(pmem);
	return 0;
}


const char *LoadResourceToMemory(LPCTSTR lptype,LPCTSTR lpname,DWORD *pfilelength,HMODULE hmodule,WORD wlanguage)
{
	HRSRC hrsrc;
	HGLOBAL hglobal;
	hrsrc=FindResourceEx(GetModuleHandle(NULL),lptype,lpname,wlanguage);
	if(!hrsrc)
		return NULL;
	hglobal=LoadResource(hmodule,hrsrc);
	if(!hglobal)
		return NULL;
	if(pfilelength)
		*pfilelength=SizeofResource(hmodule,hrsrc);
	return (const char *)LockResource(hglobal);
}

bool IsValidString(LPCTSTR pstring)
{
	//	GetACP();
	//	NLSVERSIONINFO nlsvi;
	//	GetNLSVersion(COMPARE_STRING,LOCALE_SYSTEM_DEFAULT,&nlsvi);
	//	return IsNLSDefinedString(COMPARE_STRING,0,&nlsvi,pstring,-1);


	int i;
	//bool bIsChinese=false;
	for(i=0;pstring[i];i++)
	{
		if(pstring[i]<0)
		{
			if(pstring[i+1])
			{
				if(!IsGBKCharacter(*(WORD *)(pstring+i)))
					return false;
				i++;
			}else return false;

		}else if(pstring[i]<=0x7f)
		{
			if(!isgraph(pstring[i])&&pstring[i]!=0xd&&pstring[i]!=0xa&&pstring[i]!=' ')
				return false;
		};
	}
	return true;
}

