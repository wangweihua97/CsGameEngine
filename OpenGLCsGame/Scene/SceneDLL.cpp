#include "SceneDLL.h"

GameObject* AddGameObectToScene(char* name, Transform* parent)
{
	return Scene::Instace->AddGameObject(name, parent);
}

Scene* CreatNewScene()
{
	Window::Instance->InitScene();
	return Scene::Instace;
}

Transform* GetSceneRootTransform()
{
	return Transform::Root;
}

DirectionalLightFromCS GetMainLight()
{
	return Scene::Instace->light.GetData();
}

void SetMainLight(DirectionalLightFromCS data)
{
	Scene::Instace->light.SetData(data);
}

void LoadCubemapAndInitIbl(char* path)
{
	Scene::Instace->LoadCubemapAndInitIbl(path);
}
