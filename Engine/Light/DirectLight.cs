using Accord.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Engine
{
    public struct DirectLight
    {
        public Vector3 Direction;
        public Vector3 Color;
        public float Intensity;
        public bool CastShadows;

    }
}
