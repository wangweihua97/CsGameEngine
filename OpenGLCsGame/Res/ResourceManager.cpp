#include "ResourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/system.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/regex.hpp>

#include <vector>
#include "stb_image.h"

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;
std::set<Material*>  ResourceManager::matsSet;
Material* ResourceManager::DefaultMat = 0;
Material* ResourceManager::AutoModelMat = 0;
Material* ResourceManager::ShadowMat = 0;



Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::LoadShader(const char* shaderFile)
{
    Shader shader;
    std::string shaderCode;
    try
    {
        std::ifstream file(shaderFile);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << file.rdbuf();
        shaderCode = shaderStream.str();
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    std::vector < std::string> fields;
    boost::split_regex(fields, shaderCode, boost::regex("\n----\n"));
    //std::vector<std::string> nameLine;
    //boost::split(nameLine, fields[0], boost::is_any_of("\n"), boost::token_compress_on);
    std::string shaderName = fields[0];
    if (fields.size() <= 3)
    {
        shader.Compile(fields[1].c_str(), fields[2].c_str(), nullptr);
    }
    else
    {
        shader.Compile(fields[1].c_str(), fields[2].c_str(), fields[3].c_str());
    }
    Shaders[shaderName] = shader;
    return shader;
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Shader* ResourceManager::GetShaderP(std::string name)
{
    return &Shaders[name];
}

bool ResourceManager::IsFileExistent(const boost::filesystem::path& path) {
    boost::system::error_code error;
    return boost::filesystem::is_regular_file(path, error);
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
    //Textures[name] = loadTextureFromFile(file, alpha);
    Textures[name] = loadTextureFromFile(file);
    return Textures[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, std::string name)
{
    Textures[name] = loadTextureFromFile(file);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    if (Textures.find(name) == Textures.end())
        LoadTexture(name.c_str(), false, name);
    return Textures[name];
}

Texture2D* ResourceManager::GetTextureP(std::string name)
{
    if (Textures.find(name) == Textures.end())
        LoadTexture(name.c_str(), false, name);
    return &Textures[name];
}

Material ResourceManager::CreateMaterial(std::string base)
{
    return CreateMaterial(GetShaderP(base));
}
Material* ResourceManager::CreateMaterialToSet(std::string base)
{
    Material* mat =  new Material(GetShaderP(base));
    mat->Type = MATERIAL_CUSTOM;
    matsSet.insert(mat);
    return mat;
}
Material* ResourceManager::GetDefalutMat()
{
    if (DefaultMat == nullptr)
    {
        DefaultMat = new Material(GetShaderP("Default"));
        DefaultMat->Type = MATERIAL_CUSTOM;
        DefaultMat->SetVector("_MainColor", glm::vec4(1.0));
    }
    return DefaultMat;
}

Material* ResourceManager::GetAutoModelMat()
{
    if (AutoModelMat == nullptr)
    {
        AutoModelMat = new Material(GetShaderP("autoModelShader"));
        AutoModelMat->Type = MATERIAL_CUSTOM;
        AutoModelMat->usePbr = true;
    }
    return AutoModelMat;
}
Material* ResourceManager::GetShadowMat()
{
    if (ShadowMat == nullptr)
    {
        ShadowMat = new Material(GetShaderP("shadow"));
        ShadowMat->Type = MATERIAL_CUSTOM;
    }
    return ShadowMat;
}
// --------------------------------------------------------------------------------------------
Material ResourceManager::CreateMaterial(Shader* shader)
{
    Material mat(shader);
    mat.Type = MATERIAL_CUSTOM;
    return mat;
}

bool ResourceManager::HaveTexture(std::string name)
{
    auto find = Textures.find(name);
    return find != Textures.end();
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::string vsFileStr(vShaderFile), fsFileStr(fShaderFile);
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        //std::stringstream vShaderStream, fShaderStream;
        
        std::string vsDirectory = vsFileStr.substr(0, vsFileStr.find_last_of("/\\"));
        std::string fsDirectory = fsFileStr.substr(0, fsFileStr.find_last_of("/\\"));
        // read file's buffer contents into streams
        //vShaderStream << vertexShaderFile.rdbuf();
        //fShaderStream << fragmentShaderFile.rdbuf();
        // 
        vertexCode = readShader(vertexShaderFile, vsFileStr, vsDirectory);
        fragmentCode = readShader(fragmentShaderFile, fsFileStr, fsDirectory);
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        //vertexCode = vShaderStream.str();
        //fragmentCode = fShaderStream.str();
        
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::string gsFileStr(gShaderFile);
            std::ifstream geometryShaderFile(gShaderFile);
            std::string gsDirectory = gsFileStr.substr(0, gsFileStr.find_last_of("/\\"));
            //std::stringstream gShaderStream;
            //gShaderStream << geometryShaderFile.rdbuf();
            geometryCode = readShader(geometryShaderFile, gsFileStr, gsDirectory);
            geometryShaderFile.close();
            //geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

//Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
//{
//    // create texture object
//    Texture2D texture;
//    if (alpha)
//    {
//        texture.Internal_Format = GL_RGBA;
//        texture.Image_Format = GL_RGBA;
//    }
//    // load image
//    int width, height, nrChannels;
//    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
//    // now generate texture
//    texture.Generate(width, height, data);
//    // and finally free image data
//    stbi_image_free(data);
//    return texture;
//}

Texture2D ResourceManager::loadTextureFromFile(const char* file)
{
    Texture2D texture;
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if(data)
    {
        if (nrChannels == 1)
        {
            texture.InternalFormat = GL_RED;
            texture.Format = GL_RED;
        }
        else if (nrChannels == 3)
        {
            texture.InternalFormat = GL_RGB;
            texture.Format = GL_RGB;
        }
        else if (nrChannels == 4)
        {
            texture.InternalFormat = GL_RGBA;
            texture.Format = GL_RGBA;
        }
    }
    else
    {
        std::cout << "Texture failed to load at path: " << file << std::endl;
    }
    // load image
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}

std::string ResourceManager::readShader(std::ifstream& file, const std::string& name, std::string path)
{
    std::string directory = path.substr(0, path.find_last_of("/\\"));
    std::string source, line;
    while (std::getline(file, line))
    {
        // if we encounter an #include line, include other shader source
        if (line.substr(0, 8) == "#include")
        {
            std::string includePath = directory + "/" + line.substr(9);
            std::ifstream includeFile(includePath);
            if (includeFile.is_open())
            {
                // we recursively read the shader file to support any shader include depth
                source += readShader(includeFile, name, includePath);
            }
            else
            {
                std::cout<<("Shader: " + name + ": include: " + includePath + " failed to open.");
            }
            includeFile.close();
        }
        else
            source += line + "\n";
    }
    return source;
}

Material* ResourceManager_CreatMat(char* shaderName)
{
    return ResourceManager::CreateMaterialToSet(shaderName);
}


