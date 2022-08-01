using Accord.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Mgr
{
    public static class SendMessageToCPP
    {
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr AddGameObectToScene(string name, IntPtr tfParent);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_Bool(string s, bool b);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_IntPtr_Bool(string s, IntPtr p, bool b);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_Int(string s, int i);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_IntPtr_Int(string s, IntPtr p, int i);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_Float(string s, float f);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_IntPtr_Float(string s, IntPtr p, float f);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_String(string s, string str);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_IntPtr_String(string s, IntPtr p, string str);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_Vec3(string s, Vector3 v);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_IntPtr_Vec3(string s, IntPtr p, Vector3 v);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_Vec4(string s, Vector4 v);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_IntPtr_Vec4(string s, IntPtr p, Vector4 v);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_Mat4(string s, Matrix4x4 m);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_IntPtr_Mat4(string s, IntPtr p, Matrix4x4 m);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_IntPtr_String_Float(string s, IntPtr p, char[] str, float f);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_IntPtr_String_Vec3(string s, IntPtr p, char[] str, Vector3 v);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_IntPtr_String_Vec4(string s, IntPtr p, char[] str, Vector4 v);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CsEvent_IntPtr_String_String_Int(string s, IntPtr p, char[] str, char[] str2, int i);
    }
}
