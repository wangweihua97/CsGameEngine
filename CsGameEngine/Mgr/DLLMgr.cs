using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Mgr
{
    public static class DLLMgr
    {
        public unsafe static byte[] GetBytes<T>(T t)where T : unmanaged
        {
            int size = Marshal.SizeOf(t);
            IntPtr p = (IntPtr)(&t);
            byte[] r = new byte[size];
            byte* memBytePtr = (byte*)p.ToPointer();
            for(int i =0; i< size;i++)
            {
                r[i] = memBytePtr[i];
            }
            return r;
        }

        public unsafe static int GetSizeOfObject<T>(T t)
        {
            int size = 0;
            Type type = t.GetType();
            FieldInfo[] fieldInfos = type.GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
            foreach (FieldInfo f in fieldInfos)
            {
                MethodInfo mi = typeof(DLLMgr).GetMethod("SizeOf").MakeGenericMethod(new Type[] { f.FieldType });
                object r = mi.Invoke(null, null);
                size += (int)r;
            }
            return size;
        }

        unsafe static int SizeOf<T>() where T : unmanaged
        {
            return sizeof(T);
        }

        public static Object BytesToStruct(Byte[] bytes, Type strcutType)
        {
            Int32 size = Marshal.SizeOf(strcutType);
            IntPtr buffer = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.Copy(bytes, 0, buffer, size);
                return Marshal.PtrToStructure(buffer, strcutType);
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }
    }
}
