using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using CsGameEngine.Edit.Mat;
using CsGameEngine.Edit.Mgr;
using CsGameEngine.Mgr;

namespace CsGameEngine.Engine.Components
{
    public class AutoMatModelComponent : Component
    {
        public string ModelPath {
            get { return _modelPath; }
            set { 
                _modelPath = value;
                RenderThread.AddTask(() =>
                {
                    RefreshModelPath();

                });
            } }
        string _modelPath;
        
        public IntPtr _modelComponent;

        public AutoMatModelComponent()
        {

        }
        public AutoMatModelComponent(GameObject go):base(go)
        {
            _modelComponent = AddModelComponent(go._go);
        }
        void RefreshModelPath()
        {
            ModelComponent_LoadAutoMatModel(_modelComponent , _modelPath);
        }

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr AddModelComponent(IntPtr go);


        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ModelComponent_LoadAutoMatModel(IntPtr modelComponent ,string path);
    }
}
