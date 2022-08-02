#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D
{
public:
    // holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    unsigned int ID;
    // texture image dimensions
    unsigned int Width, Height; // width and height of loaded image in pixels
    // texture Format
    GLenum InternalFormat; // format of texture object
    GLenum Format; // format of loaded image
    // texture configuration
    GLenum WrapS = GL_REPEAT; // wrapping mode on S axis
    GLenum WrapT = GL_REPEAT; // wrapping mode on T axis
    GLenum FilterMin = GL_LINEAR_MIPMAP_LINEAR; // filtering mode if texture pixels < screen pixels
    GLenum FilterMax = GL_LINEAR; // filtering mode if texture pixels > screen pixels
    GLenum Type = GL_UNSIGNED_BYTE;
    bool Mipmapping = true;
    // constructor (sets default texture modes)
    Texture2D();
    void Resize(unsigned int width, unsigned int height = 0);
    // generates texture from image data
    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    void Generate(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, void* data);
    void SetWrapMode(GLenum wrapMode);
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind(int unit = -1);
    void Unbind();
};

#endif

