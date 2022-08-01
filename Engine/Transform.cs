using Accord.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Engine
{
    public class Transform : Component
    {
        public IntPtr _tf = IntPtr.Zero;
        public Vector3 localPosition { get { return _localPosition; }
            set {
                _localPosition = value;
                if(_tf != IntPtr.Zero)
                    SetLocalPositon(_tf, _localPosition);
            } }
        private Vector3 _localPosition;
        public Vector3 localAngle {
            get { return _localAngle; }
            set
            {
                _localAngle = value;
                if (_tf != IntPtr.Zero)
                    SetLocalRotation(_tf, _localAngle);
            }
        }
        private Vector3 _localAngle;
        public Vector3 localScale {
            get { return _localScale; }
            set
            {
                _localScale = value;
                if (_tf != IntPtr.Zero)
                    SetLocalScale(_tf, _localScale);
            }
        }
        private Vector3 _localScale;
        public Transform parent;
        public Transform()
        {
            localPosition = new Vector3(0.0f);
            localAngle = new Vector3(0.0f);
            localScale = new Vector3(1.0f);
        }

        public Transform(GameObject gameObject, Transform parent):base(gameObject)
        {
            this.parent = parent;
            localPosition = new Vector3(0.0f);
            localAngle = new Vector3(0.0f);
            localScale = new Vector3(1.0f);
        }

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetLocalPositon(IntPtr tf, Vector3 pos);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetLocalRotation(IntPtr tf, Vector3 pos);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetLocalScale(IntPtr tf, Vector3 pos);
    }
}
