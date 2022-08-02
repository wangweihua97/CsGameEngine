using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;
using System.Windows.Input;
using CsGameEngine.Engine;
using CsGameEngine.Mgr;
using CsGameEngine.WpfItem;
using PropertyGrid = CsGameEngine.WpfItem.PropertyGrid;

namespace CsGameEngine.Edit.Mgr
{
    static class PropertyShowMgr
    {
        public static GameObject curGameObject;
        public static void SetGO(GameObject go)
        {
            curGameObject = go;
            MainWindow.Instance.PropertyTreeView.Items.Clear();
            AddTransformShow();
            foreach(var component in curGameObject.components)
            {
                string name = component.GetType().Name.Split('.').Last();
                AddComponentShow(name, component);
            }
        }

        public static void CreatCostumMenu(object sender, MouseButtonEventArgs e)
        {

            string[] titles = new[] { "添加一个Component" };
            RoutedEventHandler clickCallBacks = (cs, ce) =>
            {
                PopupMgr.PopupInputWindow("输入要添加Component的类名", AddComponentCallBack, null);
            };
            PopupMgr.PopopMenu(titles, new[] { clickCallBacks }, e.GetPosition(MainWindow.Instance));
        }

        static void AddTransformShow()
        {
            //PropertyTreeView
            TreeViewItem treeViewItem = new TreeViewItem();
            treeViewItem.Header = "Transform";
            PropertyGrid propertyGrid = new PropertyGrid(curGameObject.transform);
            treeViewItem.Items.Add(propertyGrid);
            MainWindow.Instance.PropertyTreeView.Items.Add(treeViewItem);
        }

        static void AddComponentShow(string name ,Component component)
        {
            //PropertyTreeView
            TreeViewItem treeViewItem = new TreeViewItem();
            treeViewItem.Header = name;
            PropertyGrid propertyGrid = new PropertyGrid(component);
            CustomPropertyGrid customPropertyGrid = component as CustomPropertyGrid;
            if (customPropertyGrid != null)
                customPropertyGrid.CustomPropertyGrid(propertyGrid);
            treeViewItem.Items.Add(propertyGrid);
            MainWindow.Instance.PropertyTreeView.Items.Add(treeViewItem);
        }
        public static void Refresh()
        {

        }

        static void AddComponentCallBack(string input)
        {

            Type type = ComponentMgr.GetType(input);
            if (type == null)
                return;
            RenderThread.AddTask(() =>
            {
                var component = curGameObject.AddComponent(type);
                EditorThread.AddTask(() =>
                {
                    AddComponentShow(input, component);
                });
            });
                
            
        }
    }
}
