using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Struct
{
    public class ComponentStruct : BaseStruct
    {
        public uint componentId;
        public List<object> args;
        public ComponentStruct(string componentName)
        {
            componentId = ComponentsMap.Maps[componentName];
        }
    }

    public static class ComponentsMap
    {
        public static Dictionary<string, uint> Maps = new Dictionary<string, uint>() {
            { "2", 1 },
        };
    }
}
