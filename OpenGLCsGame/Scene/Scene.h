#ifndef Scene_H
#define Scene_H
#include "../Window.h"
#include "../Component/CameraComponent.h"
#include <glm/glm.hpp>
#include <vector>
#include "../Render/Ibl.h"
#include "../Render/Material.h"
using namespace std;
class Transform;
class GameObject;
class CameraComponent;
class Scene
{
private:
	 unsigned int _uboBaseLight;
	 unsigned int _uboPV;
	 unsigned int _uboBaseView;
	Scene();
	static Scene* Init();
	friend void Window::InitScene();
public:
	static Scene* Instace;
	GameObject* player;
	GameObject* cube;
	DirectionalLight light;
	glm::vec3 ambient;
	Ibl ibl;
	GameObject* RootGameObject;
	Transform* RootTransform;
	CameraComponent* MainCamera;
	vector<GameObject*> Gos;
	Material SkyBox;

	void Update();
	void LateUpdate();
	void Render();
	void RenderShadow();
	void RefreshCamera();
	GameObject* AddGameObject(string name, Transform* parent);
	void LoadCubemapAndInitIbl(string path);
};
#endif