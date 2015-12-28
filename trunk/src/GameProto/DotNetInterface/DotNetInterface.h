// DotNetInterface.h

#pragma once

using namespace System;

namespace DotNetInterface {

	public enum class FunctionCallCode
	{
		fc_success = 0,	// �������óɹ�
		fc_not_found,	// ������û�ҵ�
		fc_params_error,// ���������������
		fc_continue,
	};

	public interface class FunctionCall
	{
	public:
		virtual FunctionCallCode Call(int nCallID,System::String^ strFunction,array<Byte>^ inParams,array<Byte>^ outParams);
	};	

	private ref class HostFunctionCall : public FunctionCall
	{
		iFunctionCall * m_pHostFunctionCall;
		char * m_pInParams;
		char * m_pOutParams;
	public:
		HostFunctionCall(iFunctionCall * pHostFunctionCall);
		~HostFunctionCall();
		virtual FunctionCallCode Call(int nCallID,System::String^ strFunction,array<Byte>^ inParams,array<Byte>^ outParams);
	};

	public interface class LogicFunctionCall: public FunctionCall
	{
	public:
		virtual void SetHostFuncCall(FunctionCall^ funcCall);
	};
}
