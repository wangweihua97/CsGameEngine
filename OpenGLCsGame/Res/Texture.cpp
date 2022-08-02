#include <iostream>

#include "Texture.h"


Texture2D::Texture2D()
    : Width(0), Height(0), InternalFormat(GL_RGB), Format(GL_RGB), WrapS(GL_REPEAT), WrapT(GL_REPEAT), FilterMin(GL_LINEAR), FilterMax(GL_LINEAR)
{
    
}

void Texture2D::Resize(unsigned int width, unsigned int height)
{
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, Format, Type, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
    this->Width = width;
    this->Height = height;
    // create Texture
    glGenTextures(1, &ID);
    Bind(0);
    //glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, width, height, 0, this->Format, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);
    if (Mipmapping)
        glGenerateMipmap(GL_TEXTURE_2D);
    // unbind texture
    Unbind();
}

void Texture2D::Generate(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, void* data)
{
    //glGenTextures(1, &ID);

    Width = width;
    Height = height;
    InternalFormat = internalFormat;
    Format = format;
    Type = type;
    glGenTextures(1, &ID);
    Bind(0);
    //glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterMax);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT);
    if (Mipmapping)
        glGenerateMipmap(GL_TEXTURE_2D);
    Unbind();
}

void Texture2D::SetWrapMode(GLenum wrapMode)
{
    WrapS = wrapMode;
    WrapT = wrapMode;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
}

void Texture2D::Bind(int unit)
{
    if (unit >= 0)
        glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}
// --------------------------------------------------------------------------------------------
void Texture2D::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
