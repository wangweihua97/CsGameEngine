using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using CsGameEngine.WpfItem;
using CsGameEngine.WpfItem.PropertyItem;

namespace CsGameEngine.Engine.Components
{
    public class AnimationComponent : Component, CustomPropertyGrid
    {
        public List<string> anims;
        public  float PlaySpeed
        {
            get { return _playSpeed; }
            set { _playSpeed = value; }
        }
        private float _playSpeed;
        private ModelComponent modelComponent;

        public AnimationComponent()
        {

        }
        public AnimationComponent(GameObject go) : base(go)
        {
            anims = new List<string>();
            modelComponent = gameObject.GetComponent<ModelComponent>();
            int count = ModelComponent_GetAnimationCount(modelComponent._modelComponent);
            for(int i = 0; i< count;i++)
            {
                IntPtr sp = ModelComponent_GetAnimationName(modelComponent._modelComponent, i);
                string name = Marshal.PtrToStringAnsi(sp);
                anims.Add(name);
            }
        }
        public void CustomPropertyGrid(PropertyGrid propertyGrid)
        {
            PropertyEnum propertyEnum = new PropertyEnum();
            propertyEnum.selectionChanged = AnimationChange;
            propertyEnum.name.Text = "动画";
            propertyEnum.AddItems(anims);

            RowDefinition rowDefinition = new RowDefinition();
            rowDefinition.Height = new System.Windows.GridLength(40);
            propertyGrid.grid.RowDefinitions.Add(rowDefinition);
            propertyGrid.grid.Children.Add(propertyEnum);

            Grid.SetRow(propertyEnum, propertyGrid.grid.RowDefinitions.Count - 1);
        }

        void AnimationChange(string name)
        {
            ModelComponent_PlayAnimation(modelComponent._modelComponent, name.ToCharArray());
        }

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ModelComponent_SetAnimationSpeed(IntPtr modelComponent, float speed);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int ModelComponent_GetAnimationCount(IntPtr modelComponent);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ModelComponent_GetAnimationName(IntPtr modelComponent, int index);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ModelComponent_PlayAnimation(IntPtr modelComponent, char[] name);
    }
}
