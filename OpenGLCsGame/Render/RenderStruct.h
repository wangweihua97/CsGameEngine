#ifndef RenderStruct_H
#define RenderStruct_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Res/Texture.h"
#include "../Res/TextureCube.h"
#include <iostream>

struct Texture {
    Texture2D* texture2D;
    std::string type;
    std::string path;
    bool haveData;
};
enum SHADER_TYPE
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

struct ShaderValue
{
    SHADER_TYPE  Type;
    union
    {
        bool       Bool;
        int        Int;
        float      Float;
        glm::vec2 Vec2;
        glm::vec3 Vec3;
        glm::vec4 Vec4;
        glm::mat2 Mat2;
        glm::mat3 Mat3;
        glm::mat4 Mat4;
    };

    ShaderValue() {}
};

struct Texture2DWithBind
{
    unsigned int bindPos;
    Texture2D* Texture;
};

struct TextureCubeWithBind
{
    unsigned int bindPos;
    TextureCube* Texture;
};

struct ShaderTexture
{
    SHADER_TYPE  Type;
    union
    {
        Texture2DWithBind TextureWithBind;
        TextureCubeWithBind TextureCube;
    };
    
    ShaderTexture() {}
};
#endif

