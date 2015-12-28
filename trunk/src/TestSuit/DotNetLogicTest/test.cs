using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DotNetInterface;

namespace DotNetLogicTest
{
    public class DotNetFunctionCall : LogicFunctionCall
    {
        protected FunctionCall m_hostCaller;

        public DotNetFunctionCall() {}
        ~DotNetFunctionCall() {}
        public FunctionCallCode Call(int nCallID,String strFunction, byte[] inParams, byte[] outParams) 
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

namespace testDelegate
{
    public class testDelegate
    {
        delegate void MsgHandler(string strMsg);
        delegate double DoubleHandler(double D_Parm);

        void OnMsg(string strMsg)
        {
            Console.WriteLine("Theresultis:\t{0}",strMsg);
        }

        public static double MathMethod(double parm1)
        {
            return Math.Pow(parm1,2);
        }

        public static void Main()
        {
            testDelegate t = new testDelegate();
            MsgHandler f = new MsgHandler(t.OnMsg);

            DoubleHandler[]DoubleMethod={
                new DoubleHandler(Math.Sin),
                new DoubleHandler(Math.Log10),
                new DoubleHandler(Math.Sqrt)
            };
            foreach(DoubleHandler handle in DoubleMethod)
            {
                f(MathMethod(handle(.5)).ToString()); //调用MsgHandler的对象f来显示计算结果
            }
        }
    }
} 



