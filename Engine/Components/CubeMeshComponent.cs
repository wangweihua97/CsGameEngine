using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using CsGameEngine.Edit.Mat;
using CsGameEngine.Edit.Mgr;
using CsGameEngine.Mgr;

namespace CsGameEngine.Engine.Components
{
    public class CubeMeshComponent : Component
    {
        public IntPtr _cubeMeshComponent;
        public string MatPath
        {
            get { return _matPath; }
            set
            {
                if (mat != null)
                {
                    MaterialMgr.UnRegister(_matPath, mat);
                }
                _matPath = value;
                RenderThread.AddTask(() =>
                {
                    RefreshMaterial();

                });
            }
        }
        string _matPath;

        private Material mat;
        public CubeMeshComponent()
        {

        }
        public CubeMeshComponent(GameObject go) : base(go)
        {
            _cubeMeshComponent = AddCubeMeshComponent(gameObject._go);
        }

        void RefreshMaterial()
        {
            
            BaseMaterial baseMaterial = MaterialMgr.materialHelper.GetMaterial(_matPath);
            mat = new Material(baseMaterial);
            MaterialMgr.Register(_matPath, mat);
            CubeMeshComponent_SetMeshMat(_cubeMeshComponent , mat._mat);
        }

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr AddCubeMeshComponent(IntPtr go);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void CubeMeshComponent_SetMeshMat(IntPtr cube, IntPtr mat);
    }
}
