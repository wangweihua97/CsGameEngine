using Accord.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CsGameEngine.Mgr;
using YamlDotNet.RepresentationModel;
using YamlDotNet.Serialization;

namespace CsGameEngine.Edit.Mat
{
    public enum SHADER_TYPE
    {
        SHADER_TYPE_BOOL,
        SHADER_TYPE_INT,
        SHADER_TYPE_FLOAT,
        SHADER_TYPE_SAMPLER1D,
        SHADER_TYPE_SAMPLER2D,
        SHADER_TYPE_SAMPLER3D,
        SHADER_TYPE_SAMPLER4D,
        SHADER_TYPE_SAMPLERCUBE,
        SHADER_TYPE_VEC2,
        SHADER_TYPE_VEC3,
        SHADER_TYPE_VEC4,
        SHADER_TYPE_MAT2,
        SHADER_TYPE_MAT3,
        SHADER_TYPE_MAT4,
    };
    public class ShaderValue
    {

    }
    public class BoolShaderValue : ShaderValue , LoadYaml
    {
        public bool b = false;

        public void Load(YamlMappingNode node)
        {
        }
    }
    public class IntShaderValue : ShaderValue, LoadYaml
    {
        public int i = 0;
        public void Load(YamlMappingNode node)
        {
        }
    }
    public class FloatShaderValue : ShaderValue, LoadYaml
    {
        public float f = 1.0f;
        public void Load(YamlMappingNode node)
        {
        }
    }
    public class Vec2ShaderValue : ShaderValue, LoadYaml
    {
        public System.Numerics.Vector2 v;
        public void Load(YamlMappingNode node)
        {
        }
    }

    public class Vec3ShaderValue : ShaderValue, LoadYaml
    {
        public Vector3 v;
        public void Load(YamlMappingNode node)
        {
        }
    }

    public class Vec4ShaderValue : ShaderValue, LoadYaml
    {
        public Vector4 v = new Vector4(1);
        public void Load(YamlMappingNode node)
        {
        }
    }

    public class Mat4ShaderValue : ShaderValue, LoadYaml
    {
        public Matrix4x4 m;
        public void Load(YamlMappingNode node)
        {
        }
    }

    public class Tex2DShaderValue : ShaderValue, LoadYaml
    {
        public string path = "";
        public int bindPos = 0;
        public void Load(YamlMappingNode node)
        {
        }
    }

    public class TexCubeShaderValue : ShaderValue, LoadYaml
    {
        public string path = "";
        public int bindPos = 0;
        public void Load(YamlMappingNode node)
        {
        }
    }

    public class UniformValue : LoadYaml
    {
        public SHADER_TYPE type;
        public ShaderValue value;

        public void Load(YamlMappingNode node)
        {
            type = (SHADER_TYPE)System.Enum.Parse(typeof(SHADER_TYPE), (string)node["type"]);
            switch (type)
            {
                case SHADER_TYPE.SHADER_TYPE_BOOL:
                    BoolShaderValue boolShaderValue = new BoolShaderValue();
                    YamlHelper.Deserialize<BoolShaderValue>(boolShaderValue, (YamlMappingNode)node["value"]);
                    value = boolShaderValue;
                    break;
                case SHADER_TYPE.SHADER_TYPE_INT:
                    break;
                case SHADER_TYPE.SHADER_TYPE_FLOAT:
                    FloatShaderValue floatShaderValue = new FloatShaderValue();
                    YamlHelper.Deserialize<FloatShaderValue>(floatShaderValue, (YamlMappingNode)node["value"]);
                    value = floatShaderValue;
                    break;
                case SHADER_TYPE.SHADER_TYPE_SAMPLER1D:
                    break;
                case SHADER_TYPE.SHADER_TYPE_SAMPLER2D:
                    Tex2DShaderValue tex2DShaderValue = new Tex2DShaderValue();
                    YamlHelper.Deserialize<Tex2DShaderValue>(tex2DShaderValue, (YamlMappingNode)node["value"]);
                    value = tex2DShaderValue;
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
                    Vec4ShaderValue vec4ShaderValue = new Vec4ShaderValue();
                    YamlHelper.Deserialize<Vec4ShaderValue>(vec4ShaderValue, (YamlMappingNode)node["value"]);
                    value = vec4ShaderValue;
                    break;
                case SHADER_TYPE.SHADER_TYPE_MAT2:
                    break;
                case SHADER_TYPE.SHADER_TYPE_MAT3:
                    break;
                case SHADER_TYPE.SHADER_TYPE_MAT4:
                    break;
                default:
                    break;
            }
        }
    }

    public class BaseMaterial : LoadYaml
    {
        public string shaderName;
        public Dictionary<string, UniformValue> UniformValues;
        public bool usePbr = false;

        public BaseMaterial()
        {
            UniformValues = new Dictionary<string, UniformValue>();
        }

        public void Add(string name ,SHADER_TYPE type, ShaderValue value)
        {
            UniformValue uniform = new UniformValue();
            uniform.type = type;
            uniform.value = value;
            UniformValues.Add(name, uniform);
        }

        public void Load(YamlMappingNode node)
        {
            shaderName = (string)node["shaderName"];
            UniformValues = new Dictionary<string, UniformValue>();
            var items = (YamlMappingNode)node["UniformValues"];
            
            foreach (var item in items.Children)
            {
                UniformValue uniformValue = new UniformValue();
                var name = (string)item.Key;
                uniformValue.Load((YamlMappingNode)item.Value);
                UniformValues.Add(name ,uniformValue);
            }
        }
    }
}
