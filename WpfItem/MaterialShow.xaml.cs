using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using Accord.Math;
using HandyControl.Controls;
using HandyControl.Data;
using CsGameEngine.Edit.Mat;
using CsGameEngine.Edit.Mgr;
using CsGameEngine.WpfItem.MaterialItems;

namespace CsGameEngine.WpfItem
{
    /// <summary>
    /// MaterialShow.xaml 的交互逻辑
    /// </summary>
    public partial class MaterialShow : UserControl
    {
        System.Windows.Window window;
        string localFilePath;
        BaseMaterial mat;
        MatColor curSelectMatColor;
        public MaterialShow()
        {
            InitializeComponent();
            foreach(var name in MaterialMgr.materialHelper.shaderNames)
            {
                select.Items.Add(name);
            }
            window = new System.Windows.Window();
            window.WindowStartupLocation = WindowStartupLocation.CenterOwner;
            window.Owner = MainWindow.Instance;
            window.Width = 600;
            window.Height = 600;
            window.Closed += UserControl_ContextMenuClosing;
            window.Content = this;
            window.Show();
        }

        public void SetSelectMatColor(MatColor matColor)
        {
            curSelectMatColor = matColor;
            ColorName.Text = curSelectMatColor.shaderName.Text;
        }

        public void InitData(BaseMaterial mat, string localFilePath)
        {
            this.mat = mat;
            this.localFilePath = localFilePath;
            select.SelectedIndex = MaterialMgr.materialHelper.GetIndexByShaderName(mat.shaderName);
            foreach(var kvp in mat.UniformValues)
            {
                switch (kvp.Value.type)
                {
                    case SHADER_TYPE.SHADER_TYPE_BOOL:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_INT:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_FLOAT:
                        AddFloatUniformValue(kvp.Key ,kvp.Value.value);
                        break;
                    case SHADER_TYPE.SHADER_TYPE_SAMPLER1D:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_SAMPLER2D:
                        AddSAMPLER2DUniformValue(kvp.Key, kvp.Value.value);
                        break;
                    case SHADER_TYPE.SHADER_TYPE_SAMPLER3D:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_SAMPLER4D:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_SAMPLERCUBE:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_VEC2:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_VEC3:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_VEC4:
                        AddColorUniformValue(kvp.Key, kvp.Value.value);
                        break;
                    case SHADER_TYPE.SHADER_TYPE_MAT2:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_MAT3:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_MAT4:
                        break;
                }
            }
        }

        void AddFloatUniformValue(string name ,ShaderValue shaderValue)
        {
            MatFloat matFloat = new MatFloat();
            matFloat.InitData(name, shaderValue);
            AddUserControl(matFloat);
        }

        void AddColorUniformValue(string name, ShaderValue shaderValue)
        {
            MatColor matColor = new MatColor();
            matColor.InitData(name, shaderValue);
            matColor.parent = this;
            AddUserControl(matColor);
        }

        void AddSAMPLER2DUniformValue(string name, ShaderValue shaderValue)
        {
            MatTex2D matTex2D = new MatTex2D();
            matTex2D.InitData(name, shaderValue);
            AddUserControl(matTex2D);
        }

        void AddUserControl<T>(T t) where T: UserControl
        {
            RowDefinition rowDefinition = new RowDefinition();
            rowDefinition.Height = new GridLength(150);
            list.RowDefinitions.Add(rowDefinition);
            list.Children.Add(t);
            Grid.SetColumn(t, 0);
            Grid.SetRow(t, list.RowDefinitions.Count - 1);
        }

        private void UserControl_ContextMenuClosing(object sender, System.EventArgs e)
        {
            MaterialMgr.materialHelper.SaveMaterial(mat ,localFilePath);
        }

        private void select_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            string selectName = select.SelectedItem.ToString();
            if (this.mat == null || this.mat.shaderName.Equals(selectName))
                return;
            list.Children.Clear();
            list.RowDefinitions.Clear();
            BaseMaterial newMat = MaterialMgr.materialHelper.materials[selectName];
            InitData(newMat, localFilePath);
        }

        private void colorPicker_SelectedColorChanged(object sender, FunctionEventArgs<System.Windows.Media.Color> e)
        {
            if (curSelectMatColor == null)
                return;
            Color c = colorPicker.SelectedBrush.Color;
            curSelectMatColor.shaderValue.v = new Vector4((float)c.R / 255, (float)c.G / 255, (float)c.B / 255, (float)c.A / 255);
            curSelectMatColor.Refresh();
        }
    }
}
