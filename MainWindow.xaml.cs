using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
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
using CsGameEngine.Engine;
using CsGameEngine.Mgr;
using CsGameEngine.WpfItem;

namespace CsGameEngine
{
    
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public static MainWindow Instance;
        public MainWindow()
        {
            InitializeComponent();
            Instance = this;
            Init();
            RunEditorThread();
        }

        async void Init()
        {
            FileMgr.Init();
            MaterialMgr.Init();
            ComponentMgr.Init();
            CustomMenu.Closed += CloseMene;
        }

        async Task RunEditorThread()
        {
            while(true)
            {
                await Task.Delay(10);
                EditorThread.ExcuteTask();
            }
        }

        private void OnLoaded(object sender, RoutedEventArgs args)
        {
            var glHost = new OpenGlHost(BorderHost);
            glHost.Error += GlHostOnError;
            BorderHost.Child = glHost;
        }

        private void GlHostOnError(string message)
        {
            MessageBox.Show(this, message, "OpenGL Thread Error");
            Close();
        }

        private void Window_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            string message = "Event:" + e.RoutedEvent + "  " + "Text:" + e.Text;
           
        }

        private void KeyDownFun(object sender, KeyEventArgs e)
        {
            InputMgr.KeyDownFun(sender, e);
        }

        private void KeyUpFun(object sender, KeyEventArgs e)
        {
            InputMgr.KeyUpFun(sender, e);
        }

        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void CreatScene(object sender, RoutedEventArgs e)
        {
            SceneMgr.CreatNewScene();
        }

        private void CreatMat(object sender, RoutedEventArgs e)
        {
            MaterialMgr.CreatMaterialFile();
        }

        public void ShowFileList(List<FileData> datas)
        {
            int gridWidth = (int)FileScorller.ActualWidth;
            if (gridWidth <= 0)
                gridWidth = 1360;
            int colNum = gridWidth / (FileData.FileItemWidth + 20);
            int rowNum = (int)Math.Ceiling((float)datas.Count / colNum);
            fileGrid.Children.Clear();
            fileGrid.ColumnDefinitions.Clear();
            fileGrid.RowDefinitions.Clear();

            for (int i = 0; i< colNum;i++)
            {
                ColumnDefinition columnDefinition = new ColumnDefinition();
                columnDefinition.Width = new GridLength(FileData.FileItemWidth + 20);
                fileGrid.ColumnDefinitions.Add(columnDefinition);
            }
                
            for (int j = 0; j < rowNum; j++)
            {
                RowDefinition rowDefinition = new RowDefinition();
                rowDefinition.Height = new GridLength(FileData.FileItemHeight + 20);
                fileGrid.RowDefinitions.Add(rowDefinition);
            }

            int row = 0;
            int col = 0;
            foreach(var data in datas)
            {
                FileShow f = new FileShow();
                fileGrid.Children.Add(f);
                f.InitData(data);
                Grid.SetColumn(f, col);
                Grid.SetRow(f, row);
                
                col++;
                if (col >= colNum)
                {
                    row++;
                    col = 0;
                }
            }


        }

        private void ScrollViewer_SizeChanged(object sender, SizeChangedEventArgs e)
        {

        }

        private void fileLeft_Click(object sender, RoutedEventArgs e)
        {
            FileMgr.Back();
        }

        public void CloseMene(object sender, RoutedEventArgs e)
        {
            PopupMgr.ClearCostumMenu();
        }

        private void gosCanvas_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            GameObjectMgr.CreatCostumMenu(sender, e);
        }

        private void PropertyTreeView_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            PropertyShowMgr.CreatCostumMenu(sender, e);
        }

        private void BorderHost_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            RenderThread.AddTask(()=>
            {
                InputMgr.LeftMouseDown();
            });
            
        }

        private void BorderHost_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            RenderThread.AddTask(() =>
            {
                InputMgr.LeftMouseUp();
            });
        }

        private void SkyBoxPath_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void SkyBoxButton_Click(object sender, RoutedEventArgs e)
        {
            SceneSetting.LoadCubemap(SkyBoxPath.Text);
        }

        private void BloomThreshold_TextChanged(object sender, TextChangedEventArgs e)
        {
            float threshold;
            if(float.TryParse(BloomThreshold.Text, out threshold))
            {
                SceneSetting.SetBloomThreshold(threshold);
            }
        }

        private void BloomIntensity_TextChanged(object sender, TextChangedEventArgs e)
        {
            float intensity;
            if (float.TryParse(BloomIntensity.Text, out intensity))
            {
                SceneSetting.SetBloomIntensity(intensity);
            }
        }

        private void UseBloom_Checked(object sender, RoutedEventArgs e)
        {
            SceneSetting.SetUseBloom(true);
        }

        private void UseBloom_Unchecked(object sender, RoutedEventArgs e)
        {
            SceneSetting.SetUseBloom(false);
        }
    }
}
