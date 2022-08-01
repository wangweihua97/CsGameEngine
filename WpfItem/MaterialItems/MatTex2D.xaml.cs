using System;
using System.Collections.Generic;
using System.IO;
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
    /// MatTex2D.xaml 的交互逻辑
    /// </summary>
    public partial class MatTex2D : UserControl, IShaderValue
    {
        Tex2DShaderValue shaderValue;
        bool isInit = false;
        public MatTex2D()
        {
            InitializeComponent();
        }

        public void InitData(string name, ShaderValue shaderValue)
        {
            this.shaderValue = shaderValue as Tex2DShaderValue;
            shaderName.Text = name;
            path.Text = this.shaderValue.path;
            Refresh();
            isInit = true;
        }

        public void Refresh()
        {
            var uri = FileMgr.GetLocalResUri(shaderValue.path);

            if(uri != null)
                img.Source = new BitmapImage(uri);
            Update();
        }

        void Update()
        {
            if (!isInit)
                return;
            MaterialMgr.UpdateTex2D(shaderName.Text, shaderValue);
        }

        private void path_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (string.IsNullOrEmpty(path.Text))
                return;
            FileInfo fileInfo = new FileInfo(path.Text);
            if(fileInfo.Exists)
            {
                shaderValue.path = path.Text;
                Refresh();
            }
            
            
        }
    }
}
