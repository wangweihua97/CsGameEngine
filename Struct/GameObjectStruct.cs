using Accord.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Struct
{
    public class GameObjectStruct : BaseStruct
    {
        public string name;
        public TransformStruct transform;
        public List<ComponentStruct> components;
        public GameObjectStruct()
        {
            name = "";
            transform = new TransformStruct();
            components = new List<ComponentStruct>();
        }
    }
}
