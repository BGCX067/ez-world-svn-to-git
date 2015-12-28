// 这是主 DLL 文件。

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>			// Windows 头文件:
#include "gcroot.h"
#include "ModuleSupport.h"

#include "stdafx.h"
#include "LogicInterface.h"
#include "support.h"
#include "DotNetInterface.h"

using namespace DotNetInterface;
using namespace System::Runtime::InteropServices;
using namespace System::Reflection; 
using namespace System::IO;

class CDotNetInterface : public iLogicInterface
{
	gcroot<HostFunctionCall^> m_HostFunctionCall;
	gcroot<LogicFunctionCall^> m_LogicFunctionCall;
public:
	CDotNetInterface(const LogicInterfaceConfig * pConfig)
	{
	}
	~CDotNetInterface(){}

	int Call(int nCallID,const char * lpszFunction,const char * pInParams,size_t nInParams,char * pOutParams,size_t * nOutParams)
	{
		String^ strFunction = gcnew String(lpszFunction);
		array< Byte >^ bytepInParams = gcnew array< Byte >(nInParams);
		array< Byte >^ bytepOutParams = gcnew array< Byte >(0);
		Marshal::Copy((IntPtr)(void*)pInParams,bytepInParams, 0, nInParams);
		DotNetInterface::FunctionCallCode eRet = m_LogicFunctionCall->Call(nCallID,strFunction,bytepInParams,bytepOutParams);
		if(eRet == DotNetInterface::FunctionCallCode::fc_success)
		{
			*nOutParams = bytepOutParams->Length;
			if(bytepOutParams->Length>0)
				Marshal::Copy(bytepOutParams,0,(IntPtr)(void*)pOutParams,bytepOutParams->Length);
		}
		return (int)eRet;
	}
	BOOL Initialize(const char * lpszDll,iFunctionCall * pHostFunc)
	{
		String^ strDll = gcnew String(lpszDll);
		Assembly^ module = Assembly::LoadFrom(strDll); 
		array<Type^>^ types = module->GetTypes();
		Type^ type = module->GetType("DotNetLogicTest.DotNetFunctionCall");
		m_LogicFunctionCall = (LogicFunctionCall^)System::Activator::CreateInstance(type); 
		if((LogicFunctionCall^)m_LogicFunctionCall)
		{
			m_HostFunctionCall = gcnew HostFunctionCall(pHostFunc);
			m_LogicFunctionCall->SetHostFuncCall(m_HostFunctionCall);
			return TRUE;
		}
		return FALSE;
	}
};


namespace DotNetInterface {
	HostFunctionCall::HostFunctionCall(iFunctionCall * pHostFunctionCall)
		:m_pHostFunctionCall(pHostFunctionCall)
		,m_pInParams(NULL)
	{
		m_pInParams = new char[65535];
		m_pOutParams = new char[65535];
	}
	HostFunctionCall::~HostFunctionCall()
	{
		delete []m_pInParams;
		delete []m_pOutParams;
	}
	FunctionCallCode HostFunctionCall::Call(int nCallID,System::String^ strFunction,array<Byte>^ inParams,array<Byte>^ outParams)
	{
		using namespace Runtime::InteropServices;
		const char* lpszFunction = (const char*)(Marshal::StringToHGlobalAnsi(strFunction)).ToPointer();
		
		Marshal::Copy(inParams,0,(IntPtr)(void*)m_pInParams,inParams->Length);
		size_t nOutParams = 0;
		int nRet = m_pHostFunctionCall->Call(nCallID,lpszFunction,m_pInParams,(size_t)inParams->Length,m_pOutParams,&nOutParams);
		if(nRet == (int)DotNetInterface::FunctionCallCode::fc_success)
		{		
			Array::Resize(outParams,nOutParams);
			if(nOutParams>0)
				Marshal::Copy((IntPtr)(void*)m_pOutParams,outParams, 0, nOutParams);
		}
		Marshal::FreeHGlobal(IntPtr((void*)lpszFunction));
		return (DotNetInterface::FunctionCallCode)nRet;
	}
}


#if defined( DOTNET_EXPORTS )
#	define DOTNET_EXPORTS_API	extern "C" __declspec( dllexport )
#	define DOTNET_EXPORTS_CLASS	__declspec( dllexport )
#else
#	define DOTNET_EXPORTS_API	extern "C" __declspec( dllimport )
#	define DOTNET_EXPORTS_CLASS	__declspec( dllexport )
#endif


DOTNET_EXPORTS_API iModule * CreateModule(const iInitParam * param, ISystem * pSystem)
{
	static CModuleSupport g_ms(pSystem,_T("DotNetInterface"));
	return new CDotNetInterface((const LogicInterfaceConfig*)param);
}

DOTNET_EXPORTS_API void DestroyModule(iModule *& pModule)
{
	delete pModule;
	pModule = NULL;
}