using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Engine
{
    public static class DLLObject
    {
        private static Dictionary<IntPtr, object> objects = new Dictionary<IntPtr, object>();

        public static T Get<T>(IntPtr intPtr) where T : class
        {
            return objects[intPtr] as T;
        }

        public static void Set<T>(IntPtr intPtr ,T vlaue) where T : class
        {
            objects[intPtr] = vlaue;
        }

        public static void Clear()
        {
            objects.Clear();
        }
    }
}
