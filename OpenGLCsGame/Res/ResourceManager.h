#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <boost/filesystem.hpp>
#include <glad/glad.h>

#include "Texture.h"
#include "../Render/Shader.h"
#include "../Render/Material.h"
#include <set>


// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.
class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
    static Material* DefaultMat;
    static Material* AutoModelMat;
    static Material* ShadowMat;
    static bool IsFileExistent(const boost::filesystem::path& path);
    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    static Shader    LoadShader(const char* shaderFile);
    // retrieves a stored sader
    static Shader    GetShader(std::string name);
    static Shader*    GetShaderP(std::string name);
    // loads (and generates) a texture from file
    static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
    static Texture2D LoadTexture(const char* file, std::string name);
    // retrieves a stored texture
    static Texture2D GetTexture(std::string name);
    static Texture2D* GetTextureP(std::string name);
    static Material CreateMaterial(std::string base);             // these don't have the custom flag set (default material has default state and uses checkerboard texture as albedo (and black metallic, half roughness, purple normal, white ao)
    static Material* CreateMaterialToSet(std::string base);
    static Material* GetDefalutMat();
    static Material* GetAutoModelMat();
    static Material* GetShadowMat();
    static Material CreateMaterial(Shader* shader);
    static bool HaveTexture(std::string name);
    // properly de-allocates all loaded resources
    static void      Clear();
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    static std::set<Material*> matsSet;
    // loads and generates a shader from file
    static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    // loads a single texture from file
//    static Texture2D loadTextureFromFile(const char* file, bool alpha);
    static Texture2D loadTextureFromFile(const char* file);
    static std::string readShader(std::ifstream& file, const std::string& name, std::string path);
};

#ifndef DLLFUNC
#define DLLFUNC(return_type) extern "C" __declspec(dllexport) return_type __cdecl
#endif
DLLFUNC(Material*) ResourceManager_CreatMat(char* shaderName);

#endif
