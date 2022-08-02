using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Struct
{
    public class SceneStruct
    {
        public uint MaxFieldIdIndex;
        public List<TransformStruct> transformStructs;
        public List<GameObjectStruct> gameObjectStructs;
        public List<ComponentStruct> componentStructs;
        public Dictionary<int, float> test;
        public string filePath;

        public SceneStruct()
        {
            MaxFieldIdIndex = 0;
            transformStructs = new List<TransformStruct>();
            gameObjectStructs = new List<GameObjectStruct>();
            componentStructs = new List<ComponentStruct>();
            test = new Dictionary<int, float>();
        }
    }
}
