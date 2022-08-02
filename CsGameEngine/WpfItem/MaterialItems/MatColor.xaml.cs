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
    /// MatColor.xaml 的交互逻辑
    /// </summary>
    public partial class MatColor : UserControl , IShaderValue
    {
        public MaterialShow parent;
        public Vec4ShaderValue shaderValue;
        bool isInit = false;
        public MatColor()
        {
            InitializeComponent();
        }

        public void InitData(string name, ShaderValue shaderValue)
        {
            this.shaderValue = shaderValue as Vec4ShaderValue;
            shaderName.Text = name;
            Refresh();
            isInit = true;
        }

        public void Refresh()
        {
            R.Text = shaderValue.v.X.ToString();
            G.Text = shaderValue.v.Y.ToString();
            B.Text = shaderValue.v.Z.ToString();
            A.Text = shaderValue.v.W.ToString();
            RefreshColor();
        }

        public void RefreshColor()
        {
            Color cl = Color.FromScRgb(shaderValue.v.W ,shaderValue.v.X, shaderValue.v.Y, shaderValue.v.Z);
            color.Background = new SolidColorBrush(Color.FromArgb(cl.A, cl.R, cl.G, cl.B));
            Update();
        }

        void Update()
        {
            if (!isInit)
                return;
            MaterialMgr.UpdateVec4(shaderName.Text, shaderValue.v);
        }

        private void R_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (shaderValue == null)
                return;
            if (R.Text.StartsWith("0.") && R.Text.Length >= 3 || R.Text.Equals("1") || R.Text.Equals("0"))
                shaderValue.v.X = float.Parse(R.Text);
            RefreshColor();
        }

        private void G_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (shaderValue == null)
                return;
            if (G.Text.StartsWith("0.") && G.Text.Length >= 3 || G.Text.Equals("1") || G.Text.Equals("0"))
                shaderValue.v.Y = float.Parse(G.Text);
            RefreshColor();
        }

        private void B_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (shaderValue == null)
                return;
            if (B.Text.StartsWith("0.") && B.Text.Length >= 3 || B.Text.Equals("1") || B.Text.Equals("0"))
                shaderValue.v.Z = float.Parse(B.Text);
            RefreshColor();
        }

        private void A_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (shaderValue == null)
                return;
            if (A.Text.StartsWith("0.") && A.Text.Length >= 3 || A.Text.Equals("1") || A.Text.Equals("0"))
                shaderValue.v.W = float.Parse(A.Text);
            RefreshColor();
        }

        private void color_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            parent.SetSelectMatColor(this);
        }
    }
}
