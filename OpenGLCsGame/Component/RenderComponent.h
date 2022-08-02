#ifndef RenderComponent_H
#define RenderComponent_H
#include "Component.h"
#include "../Render/Material.h"
class RenderComponent : public Component
{
public:
	Material material;
	RenderComponent();
	RenderComponent(GameObject* gameObject);
	void SetMaterial(Material& material);
	void Update()override;
	void LateUpdate()override;
	void Render()override;
};
#endif

