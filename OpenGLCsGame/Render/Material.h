#ifndef Material_H
#define Material_H
#include "Shader.h"
#include <map>
#include "RenderStruct.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Res/Texture.h"
#include "../Res/TextureCube.h"
enum MaterialType
{
    MATERIAL_DEFAULT,
    MATERIAL_CUSTOM,
    MATERIAL_POST_PROCESS,
};
class Material
{
private:
    // shader state
    Shader* m_Shader;
    std::map<std::string, ShaderValue>        m_ShaderValues;
    std::map<std::string, ShaderTexture> m_ShaderTextures;
public:
    MaterialType Type = MATERIAL_CUSTOM;
    // depth state
    bool   DepthTest = true;
    bool   DepthWrite = true;
    GLenum DepthCompare = GL_LEQUAL;

    // face culling state
    bool   Cull = true;
    GLenum CullFace = GL_BACK;
    GLenum CullWindingOrder = GL_CCW;

    // blending state
    bool   Blend = false;
    GLenum BlendSrc = GL_ONE; // pre-multiplied alpha
    GLenum BlendDst = GL_ONE_MINUS_SRC_ALPHA;
    GLenum BlendEquation = GL_FUNC_ADD;

    // shadow state
    bool ShadowCast = true;
    bool ShadowReceive = true;

    bool usePbr = false;

private:

public:
    Material();
    Material(Shader* shader);

    Shader* GetShader();
    void    SetShader(Shader* shader);

    Material Copy();

    void SetBool(std::string name, bool value);
    void SetInt(std::string name, int value);
    void SetFloat(std::string name, float value);
    void SetTexture(std::string name, Texture2D* value , unsigned int unit = 0);
    void SetTextureCube(std::string name, TextureCube* value, unsigned int unit = 0);
    void SetVector(std::string name, glm::vec2 value);
    void SetVector(std::string name, glm::vec3 value);
    void SetVector(std::string name, glm::vec4 value);
    void SetMatrix(std::string name, glm::mat2 value);
    void SetMatrix(std::string name, glm::mat3 value);
    void SetMatrix(std::string name, glm::mat4 value);

    std::map<std::string, ShaderValue>* GetShaderValues();
    std::map<std::string, ShaderTexture>* GetShaderTextures();
};

#ifndef DLLFUNC
#define DLLFUNC(return_type) extern "C" __declspec(dllexport) return_type __cdecl
#endif

void Material_SetUsePbr(void* p, bool b);
void Material_SetFloat(void* p, std::string name, float f);
void Material_SetVec3(void* p, std::string name, glm::vec3 v);
void Material_SetVec4(void* p, std::string name, glm::vec4 v);
void Material_SetTex2D(void* p, std::string name, std::string path, int bindPos);

#endif

