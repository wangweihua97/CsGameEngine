using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Engine
{
    public class Component
    {
        delegate void CsCallBack();

        protected IntPtr _component;
        public GameObject gameObject;
        CsCallBack update;
        CsCallBack lateUpdate;
        CsCallBack render;
        CsCallBack onDestory;

        public Component()
        {
        }
        public Component(GameObject go)
        {
            gameObject = go;
            _component = AddCsComponent(go._go);
            update = Update;
            lateUpdate = LateUpdate;
            render = Render;
            onDestory = OnDestory;
            CsComponent_RegisterCallBack(_component, update, lateUpdate, render , onDestory);
        }

        public virtual void OnPropertyContainerChange(string propertyName ,object keyOrIndex ,object newValue)
        {

        }

        public virtual void Update()
        {

        }
        public virtual void LateUpdate()
        {

        }
        public virtual void Render()
        {

        }

        public virtual void OnDestory()
        {

        }

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr AddCsComponent(IntPtr go);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void CsComponent_RegisterCallBack(IntPtr component , CsCallBack update, CsCallBack lateUpdate ,CsCallBack render , CsCallBack onDestory);
    }
}
