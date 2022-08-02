using Accord.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CsGameEngine.Mgr;

namespace CsGameEngine.Engine.Components
{
    public class RotateComponent : Component
    {
        public Vector3 RotateSpeed
        {
            get { return _rotateSpeed; }
            set
            {
                _rotateSpeed = value;
            }
        }
        private Vector3 _rotateSpeed;

        public RotateComponent()
        {

        }
        public RotateComponent(GameObject go):base(go)
        {

        }
        public override void Update()
        {
            base.Update();
            gameObject.transform.localAngle += _rotateSpeed * Time.DeltaTime;
        }
    }
}
