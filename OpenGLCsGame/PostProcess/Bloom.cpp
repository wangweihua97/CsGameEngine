#include "Bloom.h"
#include "../Window.h"
#include "../Res/ResourceManager.h"
#include "PostProcess.h"
Bloom::Bloom(PostProcess* post)
{
	m_Bloom = new RenderTarget(Window::Instance->Width, Window::Instance->Height, GL_HALF_FLOAT, 1, false);
	m_rtHorizontal = new RenderTarget(Window::Instance->Width, Window::Instance->Height, GL_HALF_FLOAT, 1, false);
    rtVertical = new RenderTarget(Window::Instance->Width, Window::Instance->Height, GL_HALF_FLOAT, 1, false);
	m_Post = post;
    ResourceManager::LoadShader("Shader/quad.vs", "Shader/PostProcessor/bloom.fs", nullptr, "BloomShader");
    ResourceManager::LoadShader("Shader/quad.vs", "Shader/PostProcessor/blur.fs", nullptr, "BlurShade");
    m_BloomShader = ResourceManager::GetShaderP("BloomShader");
    m_BlurShader = ResourceManager::GetShaderP("BlurShade");
}

RenderTarget* Bloom::DoBloom(RenderTarget* src)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Bloom->ID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_BloomShader->Use();
    m_BloomShader->SetFloat("_LuminanceThreshold", m_Post->BloomThreshold);
    m_BloomShader->SetFloat("_Intensity", m_Post->BloomIntensity);
	src->GetColorTexture(0)->Bind(0);
	m_Post->RenderQuad();

    bool horizontal = true;
    m_BlurShader->Use();
    for (int i = 0; i < 16; ++i, horizontal = !horizontal)
    {
        m_BlurShader->SetInteger("horizontal", horizontal);
        if (i == 0)
        {
            m_Bloom->GetColorTexture(0)->Bind(0);
        }
        else if (horizontal)
        {
            rtVertical->GetColorTexture(0)->Bind(0);
        }
        else if (!horizontal)
        {
            m_rtHorizontal->GetColorTexture(0)->Bind(0);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, horizontal ? m_rtHorizontal->ID : rtVertical->ID);
        m_Post->RenderQuad();
    }
    return rtVertical;
}
