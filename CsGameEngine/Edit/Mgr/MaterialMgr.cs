using Accord.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CsGameEngine.Edit.Mat;
using CsGameEngine.Engine;
using CsGameEngine.Mgr;
using CsGameEngine.WpfItem;

namespace CsGameEngine.Edit.Mgr
{
    static class MaterialMgr
    {
        public static MaterialHelper materialHelper;
        public static Dictionary<string, List<Material>> mats;
        public static string curMaterialEditorPath;
        public static void Init()
        {
            materialHelper = new MaterialHelper();
            mats = new Dictionary<string, List<Material>>();
            FileMgr.AddOpenFileDelegate("mat", OpenMaterialPath);
        }

        public static void CreatMaterialFile()
        {
            materialHelper.CreatMaterialFile();
        }

        public static void OpenMaterialPath(string localFilePath)
        {
            curMaterialEditorPath = localFilePath;
            materialHelper.OpenMaterialPath(localFilePath);
        }

        public static void Register(string matFilePath , Material mat)
        {
            if(mats.ContainsKey(matFilePath))
            {
                mats[matFilePath].Add(mat);
            }
            else
            {
                mats[matFilePath] = new List<Material>(new []{ mat });
            }
        }

        public static void UnRegister(string matFilePath, Material mat)
        {
            if (mats.ContainsKey(matFilePath))
            {
                int index = -1;
                for(int i = 0; i< mats[matFilePath].Count;i++)
                {
                    if(mat == mats[matFilePath][i])
                    {
                        index = i;
                        break;
                    }
                }
                if(index >=0)
                {
                    mats[matFilePath].RemoveAt(index);
                }
            }
        }

        public static void UpdateFloat(string name ,float f)
        {
            if (!mats.ContainsKey(curMaterialEditorPath))
                return;
            List<Material> materials = mats[curMaterialEditorPath];
            if (materials == null)
                return;
            RenderThread.AddTask(() =>
            {
                foreach (var mat in materials)
                {
                    mat.SetFloat(name, f);
                }
            });
        }

        public static void UpdateVec4(string name, Vector4 v)
        {
            if (!mats.ContainsKey(curMaterialEditorPath))
                return;
            List<Material> materials = mats[curMaterialEditorPath];
            if (materials == null)
                return;
            RenderThread.AddTask(() =>
            {
                foreach (var mat in materials)
            {
                mat.SetVec4(name, v);
            }
            });
        }

        public static void UpdateTex2D(string name, Tex2DShaderValue tex2D)
        {
            if (!mats.ContainsKey(curMaterialEditorPath))
                return;
            List<Material> materials = mats[curMaterialEditorPath];
            if (materials == null)
                return;
            RenderThread.AddTask(() =>
            {
                foreach (var mat in materials)
            {
                mat.SetTex2D(name, tex2D.path , tex2D.bindPos);
            }
            });
        }


    }
}
