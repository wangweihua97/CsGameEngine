
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using YamlDotNet.RepresentationModel;

namespace CsGameEngine.Mgr
{
    public static class YamlHelper
    {
        public static void Deserialize<T>(T t, YamlMappingNode node)
        {
            Type type = t.GetType();

            FieldInfo[] fieldInfos = type.GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
            foreach (FieldInfo f in fieldInfos)
            {
                if (f.FieldType == typeof(int))
                {
                    f.SetValue(t, Int32.Parse((string)node[f.Name]));
                }
                else if (f.FieldType == typeof(float))
                {
                    float value = float.Parse((string)node[f.Name]);
                    f.SetValue(t, value);
                }
                else if (f.FieldType == typeof(string))
                {
                    f.SetValue(t, (string)node[f.Name]);
                }
                else if (node.Children.ContainsKey(f.Name))
                {
                    if (f.FieldType.IsClass)
                    {
                        MethodInfo curMethod = typeof(YamlHelper).GetMethod("Deserialize", new Type[] { f.FieldType, typeof(YamlMappingNode) });
                        curMethod.Invoke(null, new object[] { f.GetValue(t), (YamlMappingNode)node[f.Name] });
                    }
                    else
                    {
                        object r = (object)f.GetValue(t);
                        MethodInfo curMethod = typeof(YamlHelper).GetMethod("Deserialize");
                        curMethod = curMethod.MakeGenericMethod(new Type[] { typeof(Object)});
                        curMethod.Invoke(null, new object[] { r ,(YamlMappingNode)node[f.Name]});
                        f.SetValue(t, Convert.ChangeType( r, f.FieldType));
                    }

                }
                else
                {
                    Console.WriteLine("不存在类型" + f.Name);
                }
            }
        }
    }
}
