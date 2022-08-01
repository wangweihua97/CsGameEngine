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
    /// PropertyInput.xaml 的交互逻辑
    /// </summary>
    public partial class PropertyInput : UserControl
    {
        Type type;
        string fieldName;
        string dicName;
        PropertyGrid parent;
        public PropertyInput()
        {
            InitializeComponent();
        }

        public PropertyInput(Type type, string fieldName, PropertyGrid parent ,string dicName = "")
        {
            this.type = type;
            this.fieldName = fieldName;
            this.dicName = dicName;
            this.parent = parent;
            InitializeComponent();
            if(string.IsNullOrEmpty(dicName))
                name.Text = fieldName;
            else
                name.Text = dicName;
        }

        private void input_LostFocus(object sender, RoutedEventArgs e)
        {
            parent.InputChange(type , fieldName, input.Text , dicName);
        }
    }
}
