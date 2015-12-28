//--------------------------------------------------------------------
//  Name:			overload.h
//  Copyright:		unknown
//	Author:			Lighter.W
//  Date:			9:38 2003-5-29
//  Description:	overload memory, stdio function
//--------------------------------------------------------------------

#ifndef _OVERLOAD_H__
#define _OVERLOAD_H__   

//----------------------------------------------------------------------------
//		Memory overload
//---------------------------------------------------------------------------
#ifdef  OVERLOAD

extern IMemManager * pMemManager;

//NEW_OFF
#undef new
#undef delete
#undef malloc
#undef calloc
#undef realloc
#undef free


#include <stdlib.h>           // Required for malloc() and free()

//
// C++ style memory allocation.
// 保证16位对齐
inline void* operator new( size_t size )    { return malloc( size ); }
inline void* operator new[]( size_t size )  { return malloc( size ); }
inline void* operator new( unsigned int size, const char *file, int line )
{
	return pMemManager->Malloc(file, line, size, MM_NEW );
}

// 在系统已经关闭后，还有可能 STL 要释放一些东西。
inline void operator delete( void* address)
{
	if(pMemManager)
	{
		if(!pMemManager->Free( address, MM_DELETE ))
			free( address );
	}
	else
		free( address );
}

inline void* operator new[]( unsigned int size, const char *file, int line )
{
	return pMemManager->Malloc(file, line, size, MM_NEW_ARRAY);
}

inline void operator delete[]( void* address )
{
	if(pMemManager)
	{
		if(!pMemManager->Free( address, MM_DELETE_ARRAY ))
			free( address );
	}
	else
		free( address );
}
inline void operator delete( void *address, const char *file, int line )   { free( address ); }
inline void operator delete[]( void *address, const char *file, int line ) { free( address ); }


//NEW_ON
#define new              new( __FILE__, __LINE__ )
#define delete           delete
#define malloc(sz)       pMemManager->Malloc(   __FILE__, __LINE__, sz,     MM_MALLOC       )
#define calloc(num, sz)  pMemManager->Malloc(   __FILE__, __LINE__, sz*num, MM_CALLOC       )
#define realloc(ptr, sz) pMemManager->Malloc(   __FILE__, __LINE__, sz,     MM_REALLOC, ptr )
#define free(ptr)        {if(!pMemManager->Free( ptr, MM_FREE )) free( ptr );}

#endif
#endif