using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace CsGameEngine.Mgr
{
    public static class RenderThread
    {
        public delegate void RenderThreadTask();
        public static Thread RThread;
        public static Queue<RenderThreadTask> tasks = new Queue<RenderThreadTask>();
        private static object taskLock = new object();
        public static void SetRenderThread(Thread thread)
        {
            RThread = thread;
        }

        public static void AddTask(RenderThreadTask task)
        {
            lock (taskLock)
            {
                tasks.Enqueue(task);
            }
        }

        public static void ExcuteTask()
        {
            lock(taskLock)
            {
                while (tasks.Count >= 1)
                {
                    tasks.Dequeue().Invoke();
                }
            }
        }
    }
}
