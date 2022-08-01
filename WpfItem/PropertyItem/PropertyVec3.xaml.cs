using Accord.Math;
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

namespace CsGameEngine.WpfItem.PropertyItem
{
    /// <summary>
    /// PropertyVec3.xaml 的交互逻辑
    /// </summary>
    public partial class PropertyVec3 : UserControl
    {
        Type type;
        string fieldName;
        PropertyGrid parent;
        public PropertyVec3()
        {
            InitializeComponent();
        }

        public PropertyVec3(Type type, string fieldName, PropertyGrid parent ,Vector3 value)
        {
            this.type = type;
            this.fieldName = fieldName;
            this.parent = parent;
            InitializeComponent();
            name.Text = fieldName;
            X.Text = value.X.ToString();
            Y.Text = value.Y.ToString();
            Z.Text = value.Z.ToString();
        }

        private void X_LostFocus(object sender, RoutedEventArgs e)
        {
            Vector3 newValue = new Vector3(float.Parse(X.Text), float.Parse(Y.Text), float.Parse(Z.Text));
            parent.Vec3Change(type, fieldName, newValue);
        }
    }
}
