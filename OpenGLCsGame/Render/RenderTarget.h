#ifndef RenderTarget_H
#define RenderTarget_H
#include <glad/glad.h>
#include "RenderStruct.h"
#include <vector>

class RenderTarget
{
public:
    unsigned int ID;

    unsigned int Width;
    unsigned int Height;
    GLenum       Type;

    bool HasDepthAndStencil;
private:
    GLenum       m_Target = GL_TEXTURE_2D;
    Texture2D              m_DepthStencil;
    std::vector<Texture2D> m_ColorAttachments;
public:
    RenderTarget(unsigned int width, unsigned int height, GLenum type = GL_UNSIGNED_BYTE, unsigned int nrColorAttachments = 1, bool depthAndStencil = true);

    Texture2D* GetDepthStencilTexture();
    Texture2D* GetColorTexture(unsigned int index);

    void Resize(unsigned int width, unsigned int height);
    void SetTarget(GLenum target);
};
#endif

