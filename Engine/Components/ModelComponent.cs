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
    public class ModelComponent : Component
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
        public bool HaveAnimation { get { return _haveAnimation; }
            set { 
                if(_haveAnimation == false)
                {
                    _haveAnimation = true;
                    RenderThread.AddTask(() =>
                    {
                        ModelComponent_InitAnimation(_modelComponent);
                    });
                }
            } }
        private bool _haveAnimation;
        public Dictionary<string, string> Name2Paths { get; set; }
        public IntPtr _modelComponent;
        public Dictionary<string, Material> name2Mat;
        private Dictionary<string, List<int>> name2Index;

        public ModelComponent()
        {

        }
        public ModelComponent(GameObject go):base(go)
        {
            _modelComponent = AddModelComponent(go._go);
            Name2Paths = new Dictionary<string, string>();
            name2Mat = new Dictionary<string, Material>();
            name2Index = new Dictionary<string, List<int>>();
        }

        public override void OnPropertyContainerChange(string propertyName, object keyOrIndex , object newValue)
        {
            base.OnPropertyContainerChange(propertyName, keyOrIndex , newValue);
            if (propertyName.Equals("Name2Paths"))
                RefreshMats((string)keyOrIndex , (string)newValue);
        }

        void RefreshModelPath()
        {
            ModelComponent_LoadModel(_modelComponent , _modelPath);
            int meshCount = ModelComponent_GetMeshsCount(_modelComponent);
            List<string> meshsName = new List<string>();
            for(int i = 0 ;i < meshCount;i++)
            {
                IntPtr sp = ModelComponent_GetMeshName(_modelComponent, i);
                string name = Marshal.PtrToStringAnsi(sp);
                if(meshsName.Contains(name))
                {
                    name2Index[name].Add(i);
                }
                else
                {
                    meshsName.Add(name);
                    Name2Paths.Add(name, "");
                    name2Index.Add(name, new List<int>() { i});
                }
            };
            EditorThread.AddTask(() =>
            {
                PropertyShowMgr.SetGO(gameObject);
            });
                
        }

        void RefreshMats(string key , string newValue)
        {
            string oldPath = Name2Paths[key];
            if (string.IsNullOrEmpty(newValue) || oldPath.Equals(newValue))
                return;
            if (!string.IsNullOrEmpty(oldPath))
            {
                MaterialMgr.UnRegister(oldPath, name2Mat[key]);
            }
            Name2Paths[key] = newValue;
            FileInfo fileInfo = new FileInfo(newValue);
            if (!fileInfo.Exists)
                return;
            BaseMaterial baseMaterial = MaterialMgr.materialHelper.GetMaterial(newValue);
            Material mat = new Material(baseMaterial);
            MaterialMgr.Register(newValue, mat);
            name2Mat[key] =  mat;
            for(int i = 0;i < name2Index[key].Count;i++)
            {
                ModelComponent_SetMeshMat(_modelComponent, name2Index[key][i], mat._mat);
            }
        }

        public override void OnDestory()
        {
            base.OnDestory();
            foreach(var kvp in Name2Paths)
            {
                if (string.IsNullOrEmpty(kvp.Value))
                    continue;
                MaterialMgr.UnRegister(kvp.Value, name2Mat[kvp.Key]);
            }
        }

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr AddModelComponent(IntPtr go);


        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ModelComponent_LoadModel(IntPtr modelComponent ,string path);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int ModelComponent_GetMeshsCount(IntPtr modelComponent);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ModelComponent_GetMeshName(IntPtr modelComponent, int index);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ModelComponent_SetMeshMat(IntPtr modelComponent, int index, IntPtr mat);
        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ModelComponent_InitAnimation(IntPtr modelComponent);
    }
}
