using Accord.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Struct
{
    public class TransformStruct : BaseStruct
    {
        public Vector3 localPosition;
        public Vector3 localAngle;
        public Vector3 localScale;
        public TransformStruct()
        {
            localPosition = new Vector3(0.0f);
            localAngle = new Vector3(0.0f);
            localScale = new Vector3(1.0f);
        }
    }
}
