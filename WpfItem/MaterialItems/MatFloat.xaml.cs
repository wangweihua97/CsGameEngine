using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using CsGameEngine.Edit.Mat;
using CsGameEngine.Edit.Mgr;

namespace CsGameEngine.WpfItem.MaterialItems
{
    /// <summary>
    /// MatFloat.xaml 的交互逻辑
    /// </summary>
    public partial class MatFloat : UserControl, IShaderValue
    {
        FloatShaderValue shaderValue;
        bool isInit = false;
        public MatFloat()
        {
            InitializeComponent();
        }

        public void InitData(string name, ShaderValue shaderValue)
        {
            this.shaderValue = shaderValue as FloatShaderValue;
            shaderName.Text = name;
            Refresh();
            isInit = true;
        }

        public void Refresh()
        {
            sliderValue.Value = shaderValue.f;
            value.Text = shaderValue.f.ToString();
        }

        private void sliderValue_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            shaderValue.f = (float)e.NewValue;
            value.Text = shaderValue.f.ToString();
            Update();
        }

        void Update()
        {
            if (!isInit)
                return;
            MaterialMgr.UpdateFloat(shaderName.Text, shaderValue.f);
        }
    }
}
