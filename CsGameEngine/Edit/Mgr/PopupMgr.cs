using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace CsGameEngine.Edit.Mgr
{
    public static class PopupMgr
    {
        public delegate void SureCallBack(string inputText);
        public delegate void CancelCallBack();
        public static void PopupInputWindow(string title, SureCallBack sureCallBack, CancelCallBack cancelCallBack)
        {
            InputBox inputBox = new InputBox();
            inputBox.Init(title, sureCallBack, cancelCallBack);
        }

        public static void PopopMenu(string[] titles, System.Windows.RoutedEventHandler[] clickCallBacks , Point p)
        {
            ClearCostumMenu();
            for (int i = titles.Length -1; i >= 0;i--)
            {
                MenuItem item = new MenuItem();
                item.Header = titles[i];
                item.Click += clickCallBacks[i];
                MainWindow.Instance.CustomMenu.Items.Insert(0, item);
            }
            Canvas.SetTop(MainWindow.Instance.CustomMenu, p.Y);
            Canvas.SetLeft(MainWindow.Instance.CustomMenu, p.X);
            MainWindow.Instance.CustomMenu.IsOpen = true;
        }

        public static void ClearCostumMenu()
        {
            for (int i = 0; i < MainWindow.Instance.CustomMenu.Items.Count; i++)
            {
                MainWindow.Instance.CustomMenu.Items.RemoveAt(0);
            }
        }
    }
}
