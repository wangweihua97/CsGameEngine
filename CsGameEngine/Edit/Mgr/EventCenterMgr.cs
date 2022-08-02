using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CsGameEngine.Edit.Mgr
{
    static class EventCenterMgr
    {
        public interface IEventInfo { }
        // 实现两个个参数
        public class EventInfo2<T0, T1> : IEventInfo
        {
            public delegate void Action(T0 t0, T1 t1);
            public Action actions;
            public EventInfo2(Action action)
            {
                actions += action;
            }
        }
        // 实现一个参数
        public class EventInfo<T> : IEventInfo
        {
            public delegate void Action(T t);
            public Action actions;
            public EventInfo(Action action)
            {
                actions += action;
            }
        }
        // 实现无参
        public class EventInfo : IEventInfo
        {
            public delegate void Action();
            public Action actions;
            public EventInfo(Action action)
            {
                actions += action;
            }
        }
        #region 私有成员
        // key对应事件的名字，value对应的是监听这个事件对应的委托函数【们】
        private static Dictionary<string, IEventInfo> eventDic = new Dictionary<string, IEventInfo>();
        #endregion

        #region 公共方法

        // 添加事件监听，两个个参数的
        public static void AddEventListener<T0, T1>(string name, EventInfo2<T0, T1>.Action action)
        {
            if (eventDic.ContainsKey(name))
                (eventDic[name] as EventInfo2<T0, T1>).actions += action;
            else
                eventDic.Add(name, new EventInfo2<T0, T1>(action));
        }

        // 添加事件监听，一个参数的
        public static void AddEventListener<T>(string name, EventInfo<T>.Action action)
        {
            if (eventDic.ContainsKey(name))
                (eventDic[name] as EventInfo<T>).actions += action;
            else
                eventDic.Add(name, new EventInfo<T>(action));
        }

        // 添加事件监听，无参数的
        public static void AddEventListener(string name, EventInfo.Action action)
        {
            if (eventDic.ContainsKey(name))
                (eventDic[name] as EventInfo).actions += action;
            else
                eventDic.Add(name, new EventInfo(action));
        }

        // 事件触发，无参的
        public static void EventTrigger(string name)
        {
            if (eventDic.ContainsKey(name))
                (eventDic[name] as EventInfo).actions?.Invoke();
        }

        //事件触发，一个参数的
        public static void EventTrigger<T>(string name, T info)
        {
            if (eventDic.ContainsKey(name))
                (eventDic[name] as EventInfo<T>).actions?.Invoke(info);
        }

        //事件触发，两个个参数的
        public static void EventTrigger<T0, T1>(string name, T0 info, T1 info2)
        {
            if (eventDic.ContainsKey(name))
                (eventDic[name] as EventInfo2<T0, T1>).actions?.Invoke(info, info2);
        }

        //移除监听，无参的
        public static void RemoveEventListener(string name, EventInfo.Action action)
        {
            if (eventDic.ContainsKey(name))
                (eventDic[name] as EventInfo).actions -= action;
        }

        //移除监听，一个参数的
        public static void RemoveEventListener<T>(string name, EventInfo<T>.Action action)
        {
            if (eventDic.ContainsKey(name))
                (eventDic[name] as EventInfo<T>).actions -= action;
        }

        //移除监听，两个个参数的
        public static void RemoveEventListener<T0, T1>(string name, EventInfo2<T0, T1>.Action action)
        {
            if (eventDic.ContainsKey(name))
                (eventDic[name] as EventInfo2<T0, T1>).actions -= action;
        }

        public static bool ContainsKey(string name)
        {
            return eventDic.ContainsKey(name);
        }

        // 清空事件中心，主要用在场景切换时
        public static void Clear()
        {
            eventDic.Clear();
        }
        #endregion
    }
}