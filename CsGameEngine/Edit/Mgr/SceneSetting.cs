using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using CsGameEngine.Mgr;

namespace CsGameEngine.Edit.Mgr
{
    public static class SceneSetting
    {
        public static void LoadCubemap(string path)
        {
            FileInfo fileInfo = new FileInfo(path);
            if (fileInfo.Exists)
            {
                RenderThread.AddTask(() =>
                {
                    LoadCubemapAndInitIbl(path);
                });
                
            }
                
        }

        public static void SetBloomThreshold(float threshold)
        {
            RenderThread.AddTask(() =>
            {
                PostProcess_SetBloomThreshold(threshold);
            });
        }

        public static void SetBloomIntensity(float intensity)
        {
            RenderThread.AddTask(() =>
            {
                PostProcess_SetBloomIntensity(intensity);
            });
        }

        public static void SetUseBloom(bool isUse)
        {
            RenderThread.AddTask(() =>
            {
                PostProcess_SetUseBloom(isUse);
            });
        }


        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void LoadCubemapAndInitIbl(string path);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PostProcess_SetBloomThreshold(float threshold);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PostProcess_SetBloomIntensity(float intensity);

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PostProcess_SetUseBloom(bool isUse);

    }
}
