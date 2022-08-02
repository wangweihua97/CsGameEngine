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
using CsGameEngine.Edit;
using CsGameEngine.Edit.Mgr;

namespace CsGameEngine.WpfItem
{
    /// <summary>
    /// FileShow.xaml 的交互逻辑
    /// </summary>
    public partial class FileShow : UserControl
    {
        public FileData data;
        public FileShow()
        {
            InitializeComponent();
        }

        public void InitData(FileData data)
        {
            this.data = data;
            if(data.isFolder)
            {
                fileName.Text = data.globalDirectoryInfo.Name;
                img.Source =  new BitmapImage(new Uri("Icons/folder.png", UriKind.RelativeOrAbsolute));
            }
            else
            {
                fileName.Text = data.globalFileInfo.Name;
                img.Source = new BitmapImage(new Uri("file.png", UriKind.RelativeOrAbsolute));

                if(this.data.localPath.EndsWith("png")||
                   this.data.localPath.EndsWith("jpg")||
                   this.data.localPath.EndsWith("jpeg")||
                   this.data.localPath.EndsWith("tga")
                    )
                {
                    var uri = FileMgr.GetLocalResUri(this.data.localPath);

                    if (uri != null)
                        img.Source = new BitmapImage(uri);
                }
            }
        }

        private void img_Click(object sender, RoutedEventArgs e)
        {

        }

        private void img_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if(e.ChangedButton == MouseButton.Left && e.ClickCount == 2)
            {
                if (data.isFolder)
                {
                    FileMgr.Into(data.globalDirectoryInfo);
                }
                else
                {
                    FileMgr.TryOpenFile(data.localPath);
                }
            }
            
        }

        private void UserControl_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            string[] titles = new[] { "复制路径到剪切板" };
            RoutedEventHandler clickCallBacks = (cs, ce) =>
            {
                //Clipboard.SetData(DataFormats.Text, data.localPath);
                Clipboard.Clear();
                Clipboard.SetDataObject(data.localPath);
                MainWindow.Instance.CustomMenu.IsOpen = false;
            };
            PopupMgr.PopopMenu(titles,new[] { clickCallBacks }, e.GetPosition(MainWindow.Instance));
        }
    }
}
