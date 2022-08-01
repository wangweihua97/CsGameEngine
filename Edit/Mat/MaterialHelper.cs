using Accord.Math;
using System;
using System.Collections.Generic;
using System.IO;
using CsGameEngine.Edit.Mgr;
using CsGameEngine.Mgr;
using CsGameEngine.WpfItem;
using YamlDotNet.Core.Events;
using YamlDotNet.Serialization;

namespace CsGameEngine.Edit.Mat
{
    class MaterialHelper
    {
        public Dictionary<string, BaseMaterial> materials;
        public List<string> shaderNames;
        public MaterialHelper()
        {
            materials = new Dictionary<string, BaseMaterial>();
            shaderNames = new List<string>();
            AddCommonDFN();
            AddDefault();
            AddIblPbr();
            AddIblPbr2();
            AddIblPbr3();
            AddIblPbr4();
            AddIblPbr5();
            AddTex2D();
        }

        public int GetIndexByShaderName(string shaderName)
        {
            for(int i = 0; i < shaderNames.Count;i++)
            {
                if (shaderName.Equals(shaderNames[i]))
                    return i;
            }
            return -1;
        }

        public void OpenMaterialPath(string localFilePath)
        {
            BaseMaterial material = ResourceMgr.Deserializer<BaseMaterial>(localFilePath ,new ShaderValueTypeFromValue());
            EditMaterial(material , localFilePath);
        }

        public BaseMaterial GetMaterial(string localFilePath)
        {
            BaseMaterial material = ResourceMgr.Deserializer<BaseMaterial>(localFilePath, new ShaderValueTypeFromValue());
            return material;
        }

        void EditMaterial(BaseMaterial baseMaterial , string localFilePath)
        {
            MaterialShow materialShow = new MaterialShow();
            materialShow.InitData(baseMaterial, localFilePath);
        }

        public void SaveMaterial(BaseMaterial baseMaterial, string localFilePath)
        {
            ResourceMgr.Serializer(baseMaterial, localFilePath);
        }

        public void CreatMaterialFile()
        {
            PopupMgr.PopupInputWindow("输入要创建的材质球名字", CreatMaterialFileCallBack, null);
        }

        void CreatMaterialFileCallBack(string input)
        {
            BaseMaterial def = materials["Default"];
            ResourceMgr.Serializer(def, FileMgr.curLocalFolderPath + "/" + input + ".mat");
        }

        void AddMaterial(BaseMaterial baseMaterial)
        {
            materials.Add(baseMaterial.shaderName, baseMaterial);
            shaderNames.Add(baseMaterial.shaderName);
        }

        void AddCommonDFN()
        {
            BaseMaterial baseMaterial = new BaseMaterial();
            baseMaterial.shaderName = "comShader";

            Vec4ShaderValue mainColor = new Vec4ShaderValue();
            mainColor.v = new Vector4(1.0f);
            baseMaterial.Add("_MainColor", SHADER_TYPE.SHADER_TYPE_VEC4, mainColor);

            Tex2DShaderValue diffuse = new Tex2DShaderValue();
            diffuse.bindPos = 0;
            baseMaterial.Add("albedoMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, diffuse);

            Tex2DShaderValue specular = new Tex2DShaderValue();
            specular.bindPos = 1;
            baseMaterial.Add("specularMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, specular);

            Tex2DShaderValue normal = new Tex2DShaderValue();
            normal.bindPos = 2;
            baseMaterial.Add("normalMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, normal);

            AddMaterial(baseMaterial);

        }

        void AddDefault()
        {
            BaseMaterial baseMaterial = new BaseMaterial();
            baseMaterial.shaderName = "Default";

            Vec4ShaderValue mainColor = new Vec4ShaderValue();
            mainColor.v = new Vector4(1.0f);
            baseMaterial.Add("_MainColor", SHADER_TYPE.SHADER_TYPE_VEC4, mainColor);
            AddMaterial(baseMaterial);
        }

        void AddIblPbr()
        {
            BaseMaterial baseMaterial = new BaseMaterial();
            baseMaterial.shaderName = "IblPbr_M_R_AO";
            baseMaterial.usePbr = true;

            FloatShaderValue floatShader = new FloatShaderValue();
            floatShader.f = 0.3f;
            baseMaterial.Add("_EmissiveIntensity", SHADER_TYPE.SHADER_TYPE_FLOAT, floatShader);

            Tex2DShaderValue albedoMap = new Tex2DShaderValue();
            albedoMap.bindPos = 0;
            baseMaterial.Add("albedoMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, albedoMap);

            Tex2DShaderValue normalMap = new Tex2DShaderValue();
            normalMap.bindPos = 1;
            baseMaterial.Add("normalMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, normalMap);

            Tex2DShaderValue rmoMap = new Tex2DShaderValue();
            rmoMap.bindPos = 2;
            baseMaterial.Add("rmoMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, rmoMap);

            Tex2DShaderValue emissiveMap = new Tex2DShaderValue();
            emissiveMap.bindPos = 3;
            baseMaterial.Add("emissiveMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, emissiveMap);
            AddMaterial(baseMaterial);
        }

        void AddIblPbr2()
        {
            BaseMaterial baseMaterial = new BaseMaterial();
            baseMaterial.shaderName = "IblPbr_R_M_AO";
            baseMaterial.usePbr = true;

            FloatShaderValue floatShader = new FloatShaderValue();
            floatShader.f = 0.3f;
            baseMaterial.Add("_EmissiveIntensity", SHADER_TYPE.SHADER_TYPE_FLOAT, floatShader);

            Tex2DShaderValue albedoMap = new Tex2DShaderValue();
            albedoMap.bindPos = 0;
            baseMaterial.Add("albedoMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, albedoMap);

            Tex2DShaderValue normalMap = new Tex2DShaderValue();
            normalMap.bindPos = 1;
            baseMaterial.Add("normalMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, normalMap);

            Tex2DShaderValue rmoMap = new Tex2DShaderValue();
            rmoMap.bindPos = 2;
            baseMaterial.Add("rmoMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, rmoMap);

            Tex2DShaderValue emissiveMap = new Tex2DShaderValue();
            emissiveMap.bindPos = 3;
            baseMaterial.Add("emissiveMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, emissiveMap);
            AddMaterial(baseMaterial);
        }

        void AddIblPbr3()
        {
            BaseMaterial baseMaterial = new BaseMaterial();
            baseMaterial.shaderName = "IblPbr_M_R_iAO";
            baseMaterial.usePbr = true;

            FloatShaderValue floatShader = new FloatShaderValue();
            floatShader.f = 0.3f;
            baseMaterial.Add("_EmissiveIntensity", SHADER_TYPE.SHADER_TYPE_FLOAT, floatShader);

            Tex2DShaderValue albedoMap = new Tex2DShaderValue();
            albedoMap.bindPos = 0;
            baseMaterial.Add("albedoMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, albedoMap);

            Tex2DShaderValue normalMap = new Tex2DShaderValue();
            normalMap.bindPos = 1;
            baseMaterial.Add("normalMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, normalMap);

            Tex2DShaderValue rmoMap = new Tex2DShaderValue();
            rmoMap.bindPos = 2;
            baseMaterial.Add("rmoMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, rmoMap);

            Tex2DShaderValue emissiveMap = new Tex2DShaderValue();
            emissiveMap.bindPos = 3;
            baseMaterial.Add("emissiveMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, emissiveMap);
            AddMaterial(baseMaterial);
        }

        void AddIblPbr4()
        {
            BaseMaterial baseMaterial = new BaseMaterial();
            baseMaterial.shaderName = "IblPbr_sM_sR_sAO";
            baseMaterial.usePbr = true;

            FloatShaderValue floatShader = new FloatShaderValue();
            floatShader.f = 0.3f;
            baseMaterial.Add("_EmissiveIntensity", SHADER_TYPE.SHADER_TYPE_FLOAT, floatShader);

            Tex2DShaderValue albedoMap = new Tex2DShaderValue();
            albedoMap.bindPos = 0;
            baseMaterial.Add("albedoMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, albedoMap);

            Tex2DShaderValue normalMap = new Tex2DShaderValue();
            normalMap.bindPos = 1;
            baseMaterial.Add("normalMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, normalMap);

            Tex2DShaderValue metallicMap = new Tex2DShaderValue();
            metallicMap.bindPos = 2;
            baseMaterial.Add("metallicMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, metallicMap);

            Tex2DShaderValue roughnessMap = new Tex2DShaderValue();
            roughnessMap.bindPos = 3;
            baseMaterial.Add("roughnessMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, roughnessMap);

            Tex2DShaderValue aoMap = new Tex2DShaderValue();
            aoMap.bindPos = 4;
            baseMaterial.Add("aoMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, aoMap);

            Tex2DShaderValue emissiveMap = new Tex2DShaderValue();
            emissiveMap.bindPos = 5;
            baseMaterial.Add("emissiveMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, emissiveMap);
            AddMaterial(baseMaterial);
        }

        void AddIblPbr5()
        {
            BaseMaterial baseMaterial = new BaseMaterial();
            baseMaterial.shaderName = "IblPbr_AO_R_M";
            baseMaterial.usePbr = true;

            FloatShaderValue floatShader = new FloatShaderValue();
            floatShader.f = 0.3f;
            baseMaterial.Add("_EmissiveIntensity", SHADER_TYPE.SHADER_TYPE_FLOAT, floatShader);

            Tex2DShaderValue albedoMap = new Tex2DShaderValue();
            albedoMap.bindPos = 0;
            baseMaterial.Add("albedoMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, albedoMap);

            Tex2DShaderValue normalMap = new Tex2DShaderValue();
            normalMap.bindPos = 1;
            baseMaterial.Add("normalMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, normalMap);

            Tex2DShaderValue rmoMap = new Tex2DShaderValue();
            rmoMap.bindPos = 2;
            baseMaterial.Add("rmoMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, rmoMap);

            Tex2DShaderValue emissiveMap = new Tex2DShaderValue();
            emissiveMap.bindPos = 3;
            baseMaterial.Add("emissiveMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, emissiveMap);
            AddMaterial(baseMaterial);

        }


        void AddTex2D()
        {
            BaseMaterial baseMaterial = new BaseMaterial(); 
            baseMaterial.shaderName = "Tex2D";

            Vec4ShaderValue mainColor = new Vec4ShaderValue();
            mainColor.v = new Vector4(1.0f);
            baseMaterial.Add("_MainColor", SHADER_TYPE.SHADER_TYPE_VEC4, mainColor);

            Tex2DShaderValue albedoMap = new Tex2DShaderValue();
            albedoMap.bindPos = 0;
            baseMaterial.Add("albedoMap", SHADER_TYPE.SHADER_TYPE_SAMPLER2D, albedoMap);
            AddMaterial(baseMaterial);
        }
    }


    public class ShaderValueTypeFromValue : INodeTypeResolver
    {
        public static SHADER_TYPE LastType;
        public bool Resolve(NodeEvent nodeEvent, ref Type currentType)
        {
            if(currentType == typeof(SHADER_TYPE))
            {
                var scalar = nodeEvent as Scalar;
                LastType = (SHADER_TYPE)System.Enum.Parse(typeof(SHADER_TYPE), scalar.Value);
                return false;
            }
            if(currentType == typeof(ShaderValue))
            {
                switch (LastType)
                {
                    case SHADER_TYPE.SHADER_TYPE_BOOL:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_INT:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_FLOAT:
                        currentType = typeof(FloatShaderValue);
                        break;
                    case SHADER_TYPE.SHADER_TYPE_SAMPLER1D:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_SAMPLER2D:
                        currentType = typeof(Tex2DShaderValue);
                        break;
                    case SHADER_TYPE.SHADER_TYPE_SAMPLER3D:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_SAMPLER4D:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_SAMPLERCUBE:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_VEC2:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_VEC3:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_VEC4:
                        currentType = typeof(Vec4ShaderValue);
                        break;
                    case SHADER_TYPE.SHADER_TYPE_MAT2:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_MAT3:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_MAT4:
                        break;
                }
                return true;
            }
            return false;
        }
    }
}
