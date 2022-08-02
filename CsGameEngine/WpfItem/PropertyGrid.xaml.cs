using Accord.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
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
using CsGameEngine.Engine;
using CsGameEngine.Mgr;
using CsGameEngine.WpfItem.PropertyItem;

namespace CsGameEngine.WpfItem
{
    public interface CustomPropertyGrid
    {
        void CustomPropertyGrid(PropertyGrid propertyGrid);
    }
    /// <summary>
    /// PropertyGrid.xaml 的交互逻辑
    /// </summary>
    public partial class PropertyGrid : UserControl
    {
        enum PropertyEditType
        {
            CheckBox,
            Vec3,
            Input
        }

        public Component component;
        public PropertyGrid()
        {
            InitializeComponent();
        }

        public PropertyGrid(Component component)
        {
            InitializeComponent();
            this.component = component;
            InitComponent();
        }

        void InitComponent()
        {
            List<(Type, string)> propertyList = new List<(Type, string)>();
            PropertyInfo[] properties = component.GetType().GetProperties(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public);
            foreach(var p in properties)
            {
                propertyList.Add((p.PropertyType ,p.Name));
            }
            InitShow(propertyList);
        }

        void InitShow(List<(Type, string)> propertyList)
        {
            ColumnDefinition c = new ColumnDefinition();
            c.Width = new GridLength(320);
            grid.ColumnDefinitions.Add(c);
            foreach (var property in propertyList)
            {
                PropertyEditType propertyEditType;
                if (property.Item1 == typeof(int))
                    propertyEditType = PropertyEditType.Input;
                else if (property.Item1 == typeof(float))
                    propertyEditType = PropertyEditType.Input;
                else if (property.Item1 == typeof(string))
                    propertyEditType = PropertyEditType.Input;
                else if (property.Item1 == typeof(bool))
                    propertyEditType = PropertyEditType.CheckBox;
                else if (property.Item1 == typeof(Vector3))
                    propertyEditType = PropertyEditType.Vec3;
                else if(property.Item1 == typeof(Dictionary<string, string>))
                {
                    AddDictionary(property.Item2);
                    continue;
                }
                else
                    continue;

                AddPropertyItem(grid ,propertyEditType , property.Item1 , property.Item2);
            }
        }

        void AddDictionary(string name)
        {
            TreeViewItem treeViewItem = new TreeViewItem();
            treeViewItem.Header = name;
            
            
            Grid subGrid = new Grid();
            var dic = (Dictionary<string, string>)component.GetType().GetProperty(name).GetValue(component);
            foreach(var kvp in dic)
            {
                AddPropertyItem(subGrid, PropertyEditType.Input, typeof(string), name, kvp.Key);
            }
            

            RowDefinition rowDefinition = new RowDefinition();
            rowDefinition.Height = new GridLength(GetHeight(PropertyEditType.Input)*(dic.Count + 1));
            grid.RowDefinitions.Add(rowDefinition);

            treeViewItem.Items.Add(subGrid);
            grid.Children.Add(treeViewItem);
            Grid.SetRow(treeViewItem, grid.RowDefinitions.Count - 1);
        }

        void AddPropertyItem(Grid parent ,PropertyEditType propertyEditType, Type type, string name, string dicName = "" )
        {
            RowDefinition rowDefinition = new RowDefinition();
            rowDefinition.Height = new GridLength(GetHeight(propertyEditType));
            parent.RowDefinitions.Add(rowDefinition);
            UserControl userControl;
            switch (propertyEditType)
            {
                case PropertyEditType.CheckBox:
                    PropertyCheckBox propertyCheckBox = new PropertyCheckBox(type ,name ,this);
                    userControl = propertyCheckBox;
                    break;
                case PropertyEditType.Vec3:
                    PropertyVec3 vec3 = new PropertyVec3(type, name, this , (Vector3)component.GetType().GetProperty(name).GetValue(component));
                    userControl = vec3;
                    break;
                default:
                    PropertyInput propertyInput = new PropertyInput(type, name, this , dicName);
                    userControl = propertyInput;
                    break;
            }
            parent.Children.Add(userControl);
            Grid.SetRow(userControl, parent.RowDefinitions.Count - 1);
            //rowDefinition.Height = new GridLength(userControl.ActualHeight);
            //this.Height = grid.ActualHeight; 
        }

        public void InputChange(Type type, string fieldName, string newValue , string dicName = "")
        {
            RenderThread.AddTask(() =>
            {
                if (type == typeof(int))
                    component.GetType().GetProperty(fieldName).SetValue(component, Int32.Parse(newValue));
                else if (type == typeof(float))
                    component.GetType().GetProperty(fieldName).SetValue(component, float.Parse(newValue));
                else if (type == typeof(string) && string.IsNullOrEmpty(dicName))
                    component.GetType().GetProperty(fieldName).SetValue(component, newValue);
                else if (type == typeof(string) && !string.IsNullOrEmpty(dicName))
                    component.OnPropertyContainerChange(fieldName, dicName, newValue);
            });
        }

        public void CheckBoxChange(Type type ,string fieldName ,bool newValue)
        {
            RenderThread.AddTask(() =>
            {
                component.GetType().GetProperty(fieldName).SetValue(component, newValue);
            });
        }

        public void Vec3Change(Type type, string fieldName, Vector3 newValue)
        {
            RenderThread.AddTask(() =>
            {
                component.GetType().GetProperty(fieldName).SetValue(component, newValue);
            });
        }

        int GetHeight(PropertyEditType propertyEditType)
        {
            switch (propertyEditType)
            {
                case PropertyEditType.CheckBox:
                    return 40;
                    break;
                case PropertyEditType.Vec3:
                    return 60;
                    break;
                case PropertyEditType.Input:
                    return 40;
                    break;
            }
            return 0;
        }
    }
}
