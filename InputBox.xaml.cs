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
using CsGameEngine.Edit.Mgr;

namespace CsGameEngine
{
    /// <summary>
    /// InputBox.xaml 的交互逻辑
    /// </summary>
    public partial class InputBox : UserControl
    {
        PopupMgr.SureCallBack sureCallBack;
        PopupMgr.CancelCallBack cancelCallBack;
        Window window;
        bool isSure = false;
        public InputBox()
        {
            InitializeComponent();
            window = new Window();
            window.WindowStartupLocation = WindowStartupLocation.CenterOwner;
            window.Owner = MainWindow.Instance;
            window.Width = this.Width;
            window.Height = this.Height + 50;
            window.Content = this;
            window.Show();
        }

        public void Init(string title, PopupMgr.SureCallBack sureCallBack, PopupMgr.CancelCallBack cancelCallBack)
        {
            titleName.Text = title;
            this.sureCallBack = sureCallBack;
            this.cancelCallBack = cancelCallBack;

        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            isSure = true;
            sureCallBack.Invoke(textBox.Text);
            window.Close();
        }

        private void UserControl_ContextMenuClosing(object sender, ContextMenuEventArgs e)
        {
            if (!isSure && cancelCallBack != null)
                cancelCallBack.Invoke();
        }
    }
}
