#ifndef Bloom_H
#define Bloom_H
#include "../Render/RenderTarget.h"
#include "../Render/Shader.h"
class PostProcess;
class Bloom
{
private:
	RenderTarget* m_Bloom;
	RenderTarget* m_rtHorizontal;
	RenderTarget* rtVertical;
	Shader* m_BloomShader;
	Shader* m_BlurShader;
	PostProcess* m_Post;
public:
	Bloom(PostProcess* post);
	RenderTarget* DoBloom(RenderTarget* src);
};


#endif

