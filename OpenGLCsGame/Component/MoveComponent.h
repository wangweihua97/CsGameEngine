#ifndef MoveComponent_H
#define MoveComponent_H
#include"Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class MoveComponent : public Component
{
public:
	const float moveSpeed = 50.0f;
	const float cameraSense = 0.1f;
	MoveComponent(GameObject* gameObject);
	void Update()override;
	void LateUpdate()override;
	void Render()override;
private:
	glm::vec2 lastMousePos = glm::vec2(-1);
};
#endif

