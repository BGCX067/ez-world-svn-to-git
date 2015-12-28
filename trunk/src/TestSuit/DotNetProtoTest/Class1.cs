using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DotNetInterface;

namespace DotNetProtoTest
{
    public class DotNetFunctionCall : LogicFunctionCall
    {
        protected FunctionCall m_hostCaller;

        public DotNetFunctionCall() { }
        ~DotNetFunctionCall() { }
        public FunctionCallCode Call(int nCallID, String strFunction, byte[] inParams, byte[] outParams)
        {
            //Console.WriteLine("call id = {0:D} begin:", nCallID);
            byte[] byInArray = new byte[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
            byte[] byOutArray = new byte[0];
            FunctionCallCode eCode;
            while ((eCode = m_hostCaller.Call(nCallID, "test", byInArray, byOutArray)) != FunctionCallCode.fc_success)
            {
            }
            //Console.WriteLine("call id = {0:D} end:", nCallID);
            return FunctionCallCode.fc_success;
        }
        public void SetHostFuncCall(FunctionCall funcCall)
        {
            m_hostCaller = funcCall;
        }
    }
}
