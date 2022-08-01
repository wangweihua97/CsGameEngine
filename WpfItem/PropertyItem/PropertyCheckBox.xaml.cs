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
    /// PropertyCheckBox.xaml 的交互逻辑
    /// </summary>
    public partial class PropertyCheckBox : UserControl
    {
        Type type;
        string fieldName;
        PropertyGrid parent;
        public PropertyCheckBox()
        {
            InitializeComponent();
        }

        public PropertyCheckBox(Type type, string fieldName , PropertyGrid parent)
        {
            this.type = type;
            this.fieldName = fieldName;
            this.parent = parent;
            InitializeComponent();
            name.Text = fieldName;
        }

        private void checkBox_Checked(object sender, RoutedEventArgs e)
        {
            parent.CheckBoxChange(type, fieldName, true);
        }

        private void checkBox_Unchecked(object sender, RoutedEventArgs e)
        {
            parent.CheckBoxChange(type, fieldName, false);
        }
    }
}
