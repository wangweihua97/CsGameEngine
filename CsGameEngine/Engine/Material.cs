using Accord.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using CsGameEngine.Edit.Mat;
using CsGameEngine.Mgr;

namespace CsGameEngine.Engine
{
    public class Material
    {
        public string shaderName;
        public IntPtr _mat;
        public BaseMaterial baseMaterial;
        public Material()
        {

        }

        public Material(BaseMaterial baseMaterial)
        {
            this.shaderName = baseMaterial.shaderName;
            this.baseMaterial = baseMaterial;
            InitMaterialData();
        }

        void InitMaterialData()
        {
            _mat = ResourceManager_CreatMat(shaderName);
            SendMessageToCPP.CsEvent_IntPtr_Bool("Material_SetUsePbr", _mat, baseMaterial.usePbr);
            foreach (var kvp in baseMaterial.UniformValues)
            {
                switch (kvp.Value.type)
                {
                    case SHADER_TYPE.SHADER_TYPE_BOOL:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_INT:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_FLOAT:
                        FloatShaderValue floatShaderValue = kvp.Value.value as FloatShaderValue;
                        SendMessageToCPP.CsEvent_IntPtr_String_Float("Material_SetFloat", _mat, kvp.Key.ToCharArray(), floatShaderValue.f);
                        break;
                    case SHADER_TYPE.SHADER_TYPE_SAMPLER1D:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_SAMPLER2D:
                        Tex2DShaderValue tex2DShaderValue = kvp.Value.value as Tex2DShaderValue;
                        SendMessageToCPP.CsEvent_IntPtr_String_String_Int("Material_SetTex2D", _mat, kvp.Key.ToCharArray(), tex2DShaderValue.path.ToCharArray(), tex2DShaderValue.bindPos);
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
                        Vec3ShaderValue vec3ShaderValue = kvp.Value.value as Vec3ShaderValue;
                        SendMessageToCPP.CsEvent_IntPtr_String_Vec3("Material_SetVec3", _mat, kvp.Key.ToCharArray(), vec3ShaderValue.v);
                        break;
                    case SHADER_TYPE.SHADER_TYPE_VEC4:
                        Vec4ShaderValue vec4ShaderValue = kvp.Value.value as Vec4ShaderValue;
                        SendMessageToCPP.CsEvent_IntPtr_String_Vec4("Material_SetVec4", _mat, kvp.Key.ToCharArray(), vec4ShaderValue.v);
                        break;
                    case SHADER_TYPE.SHADER_TYPE_MAT2:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_MAT3:
                        break;
                    case SHADER_TYPE.SHADER_TYPE_MAT4:
                        break;
                }
            }
        }

        public void SetFloat(string name ,float f)
        {
            SendMessageToCPP.CsEvent_IntPtr_String_Float("Material_SetFloat", _mat, name.ToCharArray(), f);
        }

        public void SetVec4(string name, Vector4 v)
        {
            SendMessageToCPP.CsEvent_IntPtr_String_Vec4("Material_SetVec4", _mat, name.ToCharArray(), v);
        }

        public void SetTex2D(string name, string path ,int bindPos)
        {
            SendMessageToCPP.CsEvent_IntPtr_String_String_Int("Material_SetTex2D", _mat, name.ToCharArray(), path.ToCharArray() ,bindPos);
        }

        [DllImport("OpenGLCsGame.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ResourceManager_CreatMat(string shaderName);
    }
}
