#ifndef _VW3DUTIL_
#define _VW3DUTIL_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma warning(disable: 4127)

	extern "C"
	{
		bool IsGBKCharacter(unsigned character);
		bool IsValidString(LPCTSTR pstring);
#if _MSC_VER <1400
		unsigned int strnlen(const char *str,unsigned int len);
#endif
		const char * _stdcall LoadFileToMemory(const TCHAR *pfilename,DWORD *pfilelength=NULL,DWORD offset=0);
		char * _stdcall LoadFileToMemoryW(const TCHAR *pfilename,DWORD offset=0,DWORD *pfilelength=NULL);
		int _stdcall UnloadFileFromMemory(const void  *pmem);
		char *_stdcall CreateShareMemory(const TCHAR *szName,HANDLE &hMapFile,DWORD memorysize=65536);
		char *_stdcall OpenShareMemory(const TCHAR *szName);
		int _stdcall UnloadShareMemory(const void  *pmem,HANDLE hfilemap);
		int _stdcall UnloadMemory(const void  *pmem);
		const char *LoadResourceToMemory(LPCTSTR lpType,LPCTSTR lpName,DWORD *pfilelength=NULL,HMODULE hModule=NULL,WORD wLanguage=MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));

	}
#endif
