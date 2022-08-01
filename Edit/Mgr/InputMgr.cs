using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace CsGameEngine.Edit.Mgr
{
    public static class InputMgr
    {
		enum KeyCode
		{
			W = 0,
			A = 1,
			S = 2,
			D = 3,
			Mouse1 = 4,
			Mouse2 = 5,
			Space = 6,
			Shift = 7,
			R = 8,
		};

        public static void KeyDownFun(object sender, KeyEventArgs e)
        {
            if (e.Key.Equals(Key.W))
            {
                CheckKey((int)KeyCode.W, true);
            }
            if (e.Key.Equals(Key.A))
            {
                CheckKey((int)KeyCode.A, true);
            }
            if (e.Key.Equals(Key.S))
            {
                CheckKey((int)KeyCode.S, true);
            }
            if (e.Key.Equals(Key.D))
            {
                CheckKey((int)KeyCode.D, true);
            }
            if (e.Key.Equals(Key.R))
            {
                CheckKey((int)KeyCode.R, true);
            }
            if (e.Key.Equals(Key.Space))
            {
                CheckKey((int)KeyCode.Space, true);
            }
        }

        public static void KeyUpFun(object sender, KeyEventArgs e)
        {
            if (e.Key.Equals(Key.W))
            {
                CheckKey((int)KeyCode.W, false);
            }
            if (e.Key.Equals(Key.A))
            {
                CheckKey((int)KeyCode.A, false);
            }
            if (e.Key.Equals(Key.S))
            {
                CheckKey((int)KeyCode.S, false);
            }
            if (e.Key.Equals(Key.D))
            {
                CheckKey((int)KeyCode.D, false);
            }
            if (e.Key.Equals(Key.R))
            {
                CheckKey((int)KeyCode.R, false);
            }
            if (e.Key.Equals(Key.Space))
            {
                CheckKey((int)KeyCode.Space, false);
            }
        }

        public static void  LeftMouseDown()
        {
            CheckKey((int)KeyCode.Mouse1, true);
        }

        public static void LeftMouseUp()
        {
            CheckKey((int)KeyCode.Mouse1, false);
        }

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool CheckKey(int key, bool isDown);
    }
}
