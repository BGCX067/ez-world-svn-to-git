// Common.CPP

// Author: Dr. Carlo Pescio
// Eptacom Consulting
// Via Bernardo Forte 2-3
// 17100 Savona - ITALY
// Fax +39-19-854761
// email pescio@programmers.net


#include <windows.h>
#include "../StackTrace.h"
#include "../CrashOutput.h"

// Necessary to load dialog resource
HINSTANCE dllInstance = 0 ;

//extern StackTraceInitializer stackTraceInitializer;


BOOL APIENTRY DllMain( HANDLE hModule, DWORD fdwReason, LPVOID lpvReserved )
  {
	  /*
  if( fdwReason == DLL_PROCESS_ATTACH )
    {
    dllInstance = (HINSTANCE)hModule ;
    InitializeCriticalSection( stackTraceInitializer.GetStackTraceSync() ) ;
    }
  else if( fdwReason == DLL_PROCESS_DETACH )
    {
    DeleteCriticalSection( stackTraceInitializer.GetStackTraceSync() ) ;
    }
	*/
  return( TRUE ) ;
  }


void InitStackTraceLibrary(const IStackTraceCallback * pCallBack)
{
	StackTraceInitializer::getSingletonPtr()->SetStackTraceCallBack(pCallBack);
}

const IStackTraceCallback* GetStackTraceConfig() 
{
	return StackTraceInitializer::getSingletonPtr()->GetStackTraceConfig();
}
