using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using CsGameEngine.Struct;
using YamlDotNet.Serialization;

namespace CsGameEngine.Engine
{
    public class GameObject
    {
        public IntPtr _go;
        public string name;
        public Transform transform;
        public List<Component> components;

        public GameObject(string name)
        {
            this.name = name;
            transform = new Transform();
            components = new List<Component>();
            transform.gameObject = this;
            transform.parent = Scene.Instance.Root;
        }

        public GameObject(string name ,Transform parent)
        {
            this.name = name;
            transform = new Transform();
            components = new List<Component>();
            transform.gameObject = this;
            transform.parent = parent;
        }
        public void Awake()
        {
            transform._tf = GetTransform(_go);
        }

        public T AddComponent<T>() where T : Component
        {
           return (T)AddComponent(typeof(T));
        }

        public T GetComponent<T>() where T : Component
        {
            foreach(var component in components)
            {
                if (component.GetType().Equals(typeof(T)))
                    return component as T;
            }
            return default;
        }

        public Component AddComponent(Type type)
        {
            ConstructorInfo constructorInfo = type.GetConstructor(
                BindingFlags.Instance | BindingFlags.Public, null,
                CallingConventions.HasThis, new Type[] { typeof(GameObject) }, null) ;
            object component = constructorInfo.Invoke(new[] { this});
            components.Add((Component)component);
            return (Component)component;
        }

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr AddGameObectToScene(string name, IntPtr tfParent);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr GetTransform(IntPtr go);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DestoryGameObject(IntPtr go);

        public static void Destory(GameObject go)
        {
            DestoryGameObject(go._go);
        }
    }
}
