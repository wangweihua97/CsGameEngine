using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using CsGameEngine.Engine;
using CsGameEngine.Struct;

namespace CsGameEngine.Mgr
{
    public static class SceneMgr
    {
        public static Scene CurScene;
        private static string defaultSceneName = "default";
        public static Scene GetDefaultScene()
        {
            if (ResourceMgr.HaveSceneFile(defaultSceneName))
            {
                SceneStruct sceneStruct = ResourceMgr.GetScene(defaultSceneName);
                return Load(sceneStruct);
            }
            else
            {
                ResourceMgr.CreatSceneFile(defaultSceneName);
                SceneStruct sceneStruct = ResourceMgr.GetScene(defaultSceneName);
                return Load(sceneStruct);

            }
        }

        public static void CreatNewScene()
        {
            RenderThread.AddTask(() =>
            {
                CurScene = new Scene();
                CurScene.directLight.Color = new Accord.Math.Vector3(1.0f,1.0f,1.0f);
                Accord.Math.Vector3 dir = new Accord.Math.Vector3(0.0f, -1.0f, -1.5f);
                dir.Normalize();
                CurScene.directLight.Direction = dir;
                CurScene.directLight.Intensity = 1;
                CurScene.directLight.CastShadows = true;
                CurScene.InitScene();
            });
            
        }

        public static Scene Load(SceneStruct sceneStruct)
        {
            return new Scene();
        }
    }
}
