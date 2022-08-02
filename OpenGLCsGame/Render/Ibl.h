#ifndef Ibl_H
#define Ibl_H
#include <string>
#include "Shader.h"
class Ibl
{
private:
	unsigned int captureFBO;
	unsigned int captureRBO;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	void renderQuad();
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	void renderCube();
	void Init();
	Shader* equirectangularToCubemapShader;
	Shader* irradianceShader;
	Shader* prefilterShader;
	Shader* brdfShader;
public:
	bool isInit = false;
	unsigned int envCubemap;
	unsigned int irradianceMap;
	unsigned int prefilterMap;
	unsigned int brdfLUTTexture;
	void Load(std::string path);
};
#endif

