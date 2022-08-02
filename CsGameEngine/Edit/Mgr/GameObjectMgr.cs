using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using CsGameEngine.Engine;
using CsGameEngine.Mgr;

namespace CsGameEngine.Edit.Mgr
{
    static class GameObjectMgr
    {
        private static Dictionary<TreeViewItem , GameObject> _T2G = new Dictionary<TreeViewItem , GameObject>();
        private static Dictionary<GameObject, TreeViewItem > _G2T = new Dictionary<GameObject, TreeViewItem >();
        public static TreeViewItem  curSelect;
        public static TreeViewItem  curDrag;
        public static void CreatCostumMenu(object sender, MouseButtonEventArgs e)
        {

            string[] titles = new[] { "创建一个GameObject" };
            RoutedEventHandler clickCallBacks = (cs, ce) =>
            {
                PopupMgr.PopupInputWindow("输入要创建的GameObject名字", CreatGameObjectCallBack, null);
            };
            PopupMgr.PopopMenu(titles, new[] { clickCallBacks }, e.GetPosition(MainWindow.Instance));
        }

        static void CreatGameObjectCallBack(string inputText)
        {
            if (Scene.Instance == null)
                return;
            RenderThread.AddTask(() =>
            {
                GameObject go = Scene.Instance.AddGameObject(inputText);
                EditorThread.AddTask(() =>
                {
                    AddGameObject(go);
                });
            });
        }

        static TreeViewItem parent;
        static void CreatGameObjectInParentCallBack(string inputText)
        {
            if (Scene.Instance == null)
                return;
            RenderThread.AddTask(() =>
            {
                GameObject go = Scene.Instance.AddGameObject(inputText, _T2G[parent].transform);
                EditorThread.AddTask(() =>
                {
                    AddGameObject(go);
                });
            });
        }

        public static void RemoveGameObject(GameObject go)
        {
            var menuItem = _G2T[go];
            if (go.transform.parent == Scene.Instance.Root)
                MainWindow.Instance.gosTree.Items.Remove(menuItem);
            else
            {
                _G2T[go.transform.parent.gameObject].Items.Remove(menuItem);
            }

            _T2G.Remove(menuItem);
            _G2T.Remove(go);
            RenderThread.AddTask(() =>
            {
                GameObject.Destory(go);
            });
        }

        static void AddGameObject(GameObject go)
        {
            TreeViewItem  menuItem = new TreeViewItem ();
            menuItem.Header = go.name;
            menuItem.AllowDrop = true;
            menuItem.Selected += BeClick;
            menuItem.MouseRightButtonDown += OnMouseRightButtonDown;
            if (go.transform.parent == Scene.Instance.Root)
            {
                
                MainWindow.Instance.gosTree.Items.Add(menuItem);
            }
            else
            {
                _G2T[go.transform.parent.gameObject].Items.Add(menuItem);
            }
            Add(menuItem ,go);
        }

        static void BeClick(object sender, RoutedEventArgs e)
        {            
            var source = (TreeViewItem )e.Source;
            if (source == curSelect)
                return;
            source.Background = new SolidColorBrush(Color.FromScRgb(1, 0, 0.4f, 0.6f));
            if (curSelect  != null)
                curSelect.Background = new SolidColorBrush(Color.FromScRgb(1,1,1,1));
            curSelect = source;
            PropertyShowMgr.SetGO(_T2G[curSelect]);
        }

        static void BeSelect(object sender, MouseButtonEventArgs e)
        {
            ;
        }

        static void Drop(object sender, DragEventArgs e)
        {
            ;
        }

        static void OnMouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                DataObject data = new DataObject();
                curDrag = (TreeViewItem )e.Source;
                DragDrop.DoDragDrop((DependencyObject)e.Source, data, DragDropEffects.Move);
            }
        }

        static void OnMouseRightButtonDown(object sender, RoutedEventArgs e)
        {
            parent = (TreeViewItem )e.Source;
            e.Handled = true;
            string[] titles = new[] { "在此物体下创建一个GameObject" ,"删除"};
            RoutedEventHandler clickCallBacks = (cs, ce) =>
            {
                PopupMgr.PopupInputWindow("输入要创建的GameObject名字", CreatGameObjectInParentCallBack, null);
            };

            RoutedEventHandler deleteGo = (cs, ce) =>
            {
                RemoveGameObject(_T2G[parent]);
            };
            PopupMgr.PopopMenu(titles, new[] { clickCallBacks , deleteGo }, parent.PointToScreen(new Point(100,50)));
        }


        static void Add(TreeViewItem  menuItem , GameObject go)
        {
            _T2G.Add(menuItem, go);
            _G2T.Add(go, menuItem);
        }

        static GameObject GetGameObject(TreeViewItem  menuItem)
        {
            return _T2G[menuItem];
        }


    }
}
