﻿#pragma checksum "..\..\..\..\WpfItem\MaterialShow.xaml" "{8829d00f-11b8-4213-878b-770e8597ac16}" "15CFE0DE302E6DB8997F811F43E5636681F70C62555261EB9485FF3218D826EC"
//------------------------------------------------------------------------------
// <auto-generated>
//     此代码由工具生成。
//     运行时版本:4.0.30319.42000
//
//     对此文件的更改可能会导致不正确的行为，并且如果
//     重新生成代码，这些更改将会丢失。
// </auto-generated>
//------------------------------------------------------------------------------

using HandyControl.Controls;
using HandyControl.Data;
using HandyControl.Expression.Media;
using HandyControl.Expression.Shapes;
using HandyControl.Interactivity;
using HandyControl.Media.Animation;
using HandyControl.Media.Effects;
using HandyControl.Properties.Langs;
using HandyControl.Themes;
using HandyControl.Tools;
using HandyControl.Tools.Converter;
using HandyControl.Tools.Extension;
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Shell;


namespace CsGameEngine.WpfItem {
    
    
    /// <summary>
    /// MaterialShow
    /// </summary>
    public partial class MaterialShow : System.Windows.Controls.UserControl, System.Windows.Markup.IComponentConnector {
        
        
        #line 11 "..\..\..\..\WpfItem\MaterialShow.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.ComboBox select;
        
        #line default
        #line hidden
        
        
        #line 14 "..\..\..\..\WpfItem\MaterialShow.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Grid list;
        
        #line default
        #line hidden
        
        
        #line 16 "..\..\..\..\WpfItem\MaterialShow.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBlock ColorName;
        
        #line default
        #line hidden
        
        
        #line 18 "..\..\..\..\WpfItem\MaterialShow.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal HandyControl.Controls.ColorPicker colorPicker;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/CsGameEngine;component/wpfitem/materialshow.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\..\WpfItem\MaterialShow.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1800:DoNotCastUnnecessarily")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            this.select = ((System.Windows.Controls.ComboBox)(target));
            
            #line 11 "..\..\..\..\WpfItem\MaterialShow.xaml"
            this.select.SelectionChanged += new System.Windows.Controls.SelectionChangedEventHandler(this.select_SelectionChanged);
            
            #line default
            #line hidden
            return;
            case 2:
            this.list = ((System.Windows.Controls.Grid)(target));
            return;
            case 3:
            this.ColorName = ((System.Windows.Controls.TextBlock)(target));
            return;
            case 4:
            this.colorPicker = ((HandyControl.Controls.ColorPicker)(target));
            
            #line 18 "..\..\..\..\WpfItem\MaterialShow.xaml"
            this.colorPicker.SelectedColorChanged += new System.EventHandler<HandyControl.Data.FunctionEventArgs<System.Windows.Media.Color>>(this.colorPicker_SelectedColorChanged);
            
            #line default
            #line hidden
            return;
            }
            this._contentLoaded = true;
        }
    }
}
