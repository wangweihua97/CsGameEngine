#ifndef CubeMeshComponent_H
#define CubeMeshComponent_H
#include"Component.h"
#include"../Render/Shader.h"
#include "../Mesh/Mesh.h"
#include "../Render/Material.h"
class CubeMeshComponent :public Component
{
public:
	unsigned int cubeVAO;
	unsigned int cubeVBO;
	Mesh*  mesh;
	Material* meshsMat;
	CubeMeshComponent(GameObject* gameObject);
	void Update()override;
	void LateUpdate()override;
	void Render()override;
	void RenderShadow()override;
private:
	Shader* _shader;
};
#ifndef DLLFUNC
#define DLLFUNC(return_type) extern "C" __declspec(dllexport) return_type __cdecl
#endif

DLLFUNC(void) CubeMeshComponent_SetMeshMat(CubeMeshComponent* cube, Material* mat);
#endif

