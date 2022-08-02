using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Edit.Mgr
{
    static class ComponentMgr
    {

        //static Assembly ComponentAssembly;
        public static void Init()
        {
            //ComponentAssembly = Assembly.Load("Assembly-CSharp");
        }

        public static Type GetType(string className)
        {
            return System.Type.GetType("CsGameEngine.Engine.Components."+ className); ;
        }
    }
}
