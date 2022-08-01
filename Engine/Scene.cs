using Accord.Math;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using CsGameEngine.Mgr;

namespace CsGameEngine.Engine
{
    public class Scene
    {
        public static Scene Instance;
        public IntPtr _scene;
        public DirectLight directLight;
        public List<GameObject> gos;
        public Transform Root;
        public string skyBoxPath;

        public Scene()
        {
            gos = new List<GameObject>();
            Instance = this;
            Root = new Transform();
        }

        public void InitScene()
        {
            _scene = CreatNewScene();
            Root._tf = GetSceneRootTransform();
            SetMainLight(directLight);
        }

        public GameObject AddGameObject(string name)
        {
            GameObject go = new GameObject(name);
            gos.Add(go);
            RenderThread.AddTask(() =>
            {
                IntPtr goP = AddGameObectToScene(name, Root._tf);
                go._go = goP;
                go.Awake();

            });
                
            return go;
        }

        public GameObject AddGameObject(string name ,Transform parent)
        {
            GameObject go = new GameObject(name , parent);
            gos.Add(go);
            RenderThread.AddTask(() =>
            {
                IntPtr goP = AddGameObectToScene(name, parent._tf);
                go._go = goP;
            });
            return go;
        }

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr AddGameObectToScene(string name, IntPtr parent);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr CreatNewScene();

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr GetSceneRootTransform();

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern DirectLight GetMainLight();

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SetMainLight(DirectLight data);
    }
}
