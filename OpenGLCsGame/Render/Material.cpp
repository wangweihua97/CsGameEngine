#include "Material.h"
#include "../Res/ResourceManager.h"
Material::Material()
{

}
// --------------------------------------------------------------------------------------------
Material::Material(Shader* shader)
{
    m_Shader = shader;
}
// --------------------------------------------------------------------------------------------
Shader* Material::GetShader()
{
    return m_Shader;
}
// --------------------------------------------------------------------------------------------
void Material::SetShader(Shader* shader)
{
    m_Shader = shader;
}
// --------------------------------------------------------------------------------------------
Material Material::Copy()
{
    Material copy(m_Shader);

    copy.DepthTest = DepthTest;
    copy.DepthWrite = DepthWrite;
    copy.DepthCompare = DepthCompare;

    copy.Cull = Cull;
    copy.CullFace = CullFace;
    copy.CullWindingOrder = CullWindingOrder;

    copy.Blend = Blend;
    copy.BlendSrc = BlendSrc;
    copy.BlendDst = BlendDst;
    copy.BlendEquation = BlendEquation;

    copy.m_ShaderValues = m_ShaderValues;
    copy.m_ShaderTextures = m_ShaderTextures;

    return copy;
}
// --------------------------------------------------------------------------------------------
void Material::SetBool(std::string name, bool value)
{
    m_ShaderValues[name].Type = SHADER_TYPE_BOOL;
    m_ShaderValues[name].Bool = value;
}
// --------------------------------------------------------------------------------------------
void Material::SetInt(std::string name, int value)
{
    m_ShaderValues[name].Type = SHADER_TYPE_INT;
    m_ShaderValues[name].Int = value;
}
// --------------------------------------------------------------------------------------------
void Material::SetFloat(std::string name, float value)
{
    m_ShaderValues[name].Type = SHADER_TYPE_FLOAT;
    m_ShaderValues[name].Float = value;
}
// --------------------------------------------------------------------------------------------
void Material::SetTexture(std::string name, Texture2D* value , unsigned int unit)
{
    Texture2DWithBind t;
    t.Texture = value;
    t.bindPos = unit;
    m_ShaderTextures[name].TextureWithBind = t;
    /*switch (value->Format)
    {
    case GL_RED:
        m_ShaderTextures[name].Type = SHADER_TYPE_SAMPLER1D;
        break;
    case GL_RGB:
        m_ShaderTextures[name].Type = SHADER_TYPE_SAMPLER3D;
        break;
    case GL_RGBA:
        m_ShaderTextures[name].Type = SHADER_TYPE_SAMPLER4D;
        break;
    }*/
    m_ShaderTextures[name].Type = SHADER_TYPE_SAMPLER2D;

    if (m_Shader)
    {
        m_Shader->Use();
        m_Shader->SetInteger(name.c_str(), unit);
    }
}

void Material::SetTextureCube(std::string name, TextureCube* value, unsigned int unit)
{
    m_ShaderTextures[name].TextureCube.bindPos = unit;
    m_ShaderTextures[name].Type = SHADER_TYPE_SAMPLERCUBE;
    m_ShaderTextures[name].TextureCube.Texture = value;

    if (m_Shader)
    {
        m_Shader->Use();
        m_Shader->SetInteger(name.c_str(), unit);
    }
}
// ------------------------------------------------------------------------
void Material::SetVector(std::string name, glm::vec2 value)
{
    m_ShaderValues[name].Type = SHADER_TYPE_VEC2;
    m_ShaderValues[name].Vec2 = value;
}
// ------------------------------------------------------------------------
void Material::SetVector(std::string name, glm::vec3 value)
{
    m_ShaderValues[name].Type = SHADER_TYPE_VEC3;
    m_ShaderValues[name].Vec3 = value;
}
// ------------------------------------------------------------------------
void Material::SetVector(std::string name, glm::vec4 value)
{
    m_ShaderValues[name].Type = SHADER_TYPE_VEC4;
    m_ShaderValues[name].Vec4 = value;
}
// ------------------------------------------------------------------------
void Material::SetMatrix(std::string name, glm::mat2 value)
{
    m_ShaderValues[name].Type = SHADER_TYPE_MAT2;
    m_ShaderValues[name].Mat2 = value;
}
// ------------------------------------------------------------------------
void Material::SetMatrix(std::string name, glm::mat3 value)
{
    m_ShaderValues[name].Type = SHADER_TYPE_MAT3;
    m_ShaderValues[name].Mat3 = value;
}
// ------------------------------------------------------------------------
void Material::SetMatrix(std::string name, glm::mat4 value)
{
    m_ShaderValues[name].Type = SHADER_TYPE_MAT4;
    m_ShaderValues[name].Mat4 = value;
}
// ------------------------------------------------------------------------
std::map<std::string, ShaderValue>* Material::GetShaderValues()
{
    return &m_ShaderValues;
}
// ------------------------------------------------------------------------
std::map<std::string, ShaderTexture>* Material::GetShaderTextures()
{
    return &m_ShaderTextures;
}

void Material_SetUsePbr(void* p, bool b)
{
    Material* mat = reinterpret_cast<Material*>(p);
    mat->usePbr = b;
}

void Material_SetFloat(void* p, std::string name, float f)
{
    Material* mat = reinterpret_cast<Material*>(p);
    mat->SetFloat(name, f);
}

void Material_SetVec3(void* p, std::string name, glm::vec3 v)
{
    Material* mat = reinterpret_cast<Material*>(p);
    mat->SetVector(name, v);
}

void Material_SetVec4(void* p, std::string name, glm::vec4 v)
{
    Material* mat = reinterpret_cast<Material*>(p);
    mat->SetVector(name, v);
}

void Material_SetTex2D(void* p, std::string name, std::string path ,int bindPos)
{
    Material* mat = reinterpret_cast<Material*>(p);
    mat->SetTexture(name, ResourceManager::GetTextureP(path), bindPos);
}
