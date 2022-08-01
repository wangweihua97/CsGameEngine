using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Mgr
{
    static class Time
    {
        public static float DeltaTime
        {
            get { return _deltaTime; }
        }
        static float _deltaTime;

        public static void SetDeltaTime(float deltaTime)
        {
            _deltaTime = deltaTime;
        }
    }
}
