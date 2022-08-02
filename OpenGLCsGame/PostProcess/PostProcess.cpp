#include "PostProcess.h"
#include "../Res/ResourceManager.h"
#include "../Render/GLRender.h"
#include "../Window.h"
PostProcess::PostProcess(GLRender* renderer):m_bloom(this)
{
	m_renderer = renderer;
	m_PostProcessTarget = renderer->m_PostProcessTarget;
	Quad = ResourceManager::GetShaderP("Quad");
}

PostProcess::~PostProcess()
{
}

void PostProcess::Bloom(RenderTarget* src)
{
	glDisable(GL_DEPTH_TEST);
	auto bloom = m_bloom.DoBloom(src);

	glBindFramebuffer(GL_FRAMEBUFFER, m_PostProcessTarget->ID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	Quad->Use();
	src->GetColorTexture(0)->Bind(0);
	RenderQuad();

	bloom->GetColorTexture(0)->Bind(0);
	RenderQuad();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	
}

void PostProcess::DoPostProcess(RenderTarget* src)
{
	if (UseBloom)
	{
		Bloom(src);
	}
	else
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, src->ID);
	    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_PostProcessTarget->ID);
	    glBlitFramebuffer(0, 0, m_PostProcessTarget->Width, m_PostProcessTarget->Height, 0, 0, m_PostProcessTarget->Width, m_PostProcessTarget->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
}

void PostProcess::RenderQuad()
{
	Window::Instance->renderQuad();
}

void PostProcess_SetBloomThreshold(float threshold)
{
	Window::Instance->m_render.m_PostProcessor->BloomThreshold = threshold;
}
void PostProcess_SetBloomIntensity(float intensity)
{
	Window::Instance->m_render.m_PostProcessor->BloomIntensity = intensity;
}
DLLFUNC(void) PostProcess_SetUseBloom(bool isUse)
{
	Window::Instance->m_render.m_PostProcessor->UseBloom = isUse;
}
