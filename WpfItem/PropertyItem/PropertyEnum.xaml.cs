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
    /// PropertyEnum.xaml 的交互逻辑
    /// </summary>
    public partial class PropertyEnum : UserControl
    {
        public delegate void SelectionChanged(string name);
        public SelectionChanged selectionChanged;
        public PropertyEnum()
        {
            InitializeComponent();
        }

        public void AddItems(List<string> items)
        {
            foreach(var item in items)
            {
                select.Items.Add(item);
            }
        }

        private void select_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            string selectName = select.SelectedItem.ToString();
            if (selectionChanged != null)
                selectionChanged(selectName);
        }
    }
}
