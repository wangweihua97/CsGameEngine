#include "Scene.h"
#include "../GameObject/Transform.h"
#include "../GameObject/GameObject.h"
#include "../EventCenter.h"
#include <glm/gtc/type_ptr.hpp>
#include "../Component/CubeMeshComponent.h"
#include "../Res/ResourceManager.h"
#include "../Component/MoveComponent.h"
#include "../Component/ModelComponent.h"
#include "../Component/RenderComponent.h"
Scene* Scene::Instace = nullptr;

Scene::Scene()
{
	Scene::Instace = this;
	light.Direction = glm::normalize(glm::vec3(0.0f, -1.0f, -1.5f));
	light.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	light.CastShadows = true;
	ambient = glm::vec3(0.2f);
	Window::Instance->m_render.AddLight(&light);

	glGenBuffers(1, &_uboPV);
	glBindBuffer(GL_UNIFORM_BUFFER, _uboPV);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, _uboPV, 0, 2 * sizeof(glm::mat4));


	glGenBuffers(1, &_uboBaseLight);
	glBindBuffer(GL_UNIFORM_BUFFER, _uboBaseLight);
	glBufferData(GL_UNIFORM_BUFFER,3 * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, _uboBaseLight, 0, 3 * sizeof(glm::vec4));

	glGenBuffers(1, &_uboBaseView);
	glBindBuffer(GL_UNIFORM_BUFFER, _uboBaseView);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 2, _uboBaseView, 0, sizeof(glm::vec3));
	
	/*glBindBuffer(GL_UNIFORM_BUFFER, _uboPV);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(glm::mat4(1.0f)));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(glm::mat4(1.0f)));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);*/

	RootGameObject = new GameObject("Root", nullptr);
	Transform::Root = new Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), nullptr);
	Transform::Root->gameObject = RootGameObject;

	/*player = new GameObject("mySelf", Transform::Root);
	player->transform->localPosition = glm::vec3(0.0f, 1.0f, 0.0f);
	CameraComponent* camera = player->AddComponent<CameraComponent>();
	MoveComponent* move = player->AddComponent<MoveComponent>();
	camera->Active();
	cube = new GameObject("Cube", Transform::Root);
	cube->transform->localPosition = glm::vec3(0.0f, 0.0f, 10.0f);
	CubeMeshComponent* cubeMeshComponent = cube->AddComponent<CubeMeshComponent>();
	cubeMeshComponent->SetShader(ResourceManager::GetShaderP("CubeShader"));
	cubeMeshComponent->SetBound(100.0f, 0.2f, 100.0f);

	GameObject* d = new GameObject("zombie", Transform::Root);
	d->transform->localPosition = glm::vec3(10.0f, 0.5f, 0.0f);
	d->transform->localScale = glm::vec3(0.1f, 0.1f, 0.1f);
	ModelComponent* model_d = d->AddComponent<ModelComponent>();
	model_d->LoadModel(ResourceManager::GetShaderP("SkeletonModelShader"), "Model/zombieWalk/scene.gltf", false);
	model_d->InitAnimation();*/

	player = new GameObject("mySelf", Transform::Root);
	player->transform->localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	CameraComponent* camera = player->AddComponent<CameraComponent>();
	MoveComponent* move = player->AddComponent<MoveComponent>();
	camera->Active();

	/*GameObject* c = new GameObject("swat", Transform::Root);
	c->transform->localPosition = glm::vec3(0.0f, 0.5f, 0.0f);
	c->transform->localScale = glm::vec3(0.1f, 0.1f, 0.1f);
	ModelComponent* model = c->AddComponent<ModelComponent>();
	model->LoadModel("Model/swat/Swat.fbx" ,false);
	RenderComponent* render = c->AddComponent<RenderComponent>();
	auto mat = ResourceManager::CreateMaterial("comShader");
	ResourceManager::LoadTexture("Model/swat/Soldier_Body_diffuse.png", false, "diffuse");
	ResourceManager::LoadTexture("Model/swat/Soldier_Body_normal.png", false, "normal");
	ResourceManager::LoadTexture("Model/swat/Soldier_Body_specular.png", false, "specular");
	mat.SetTexture("albedoMap", ResourceManager::GetTextureP("diffuse"),0);
	mat.SetTexture("specularMap", ResourceManager::GetTextureP("specular"), 1);
	mat.SetTexture("normalMap", ResourceManager::GetTextureP("normal"), 2);
	render->SetMaterial(mat);*/

	/*GameObject* e = new GameObject("Gun", Transform::Root);
	e->transform->localPosition = glm::vec3(20.0f, 15.0f, 0.0f);
	e->transform->localScale = glm::vec3(10.0f);
	ModelComponent* model_e = e->AddComponent<ModelComponent>();
	model_e->LoadModel(ResourceManager::GetShaderP("SkeletonModelShader"), "Model/machinegun/scene.gltf", false);
	model_e->InitAnimation();*/
}

Scene* Scene::Init()
{
	return new Scene();
}

void Scene::Update()
{
	Transform::Root->Updete();
}

void Scene::LateUpdate()
{
	glBindBuffer(GL_UNIFORM_BUFFER, _uboBaseLight);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(glm::vec4(light.Direction,1)));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(glm::vec4(light.Color ,1)));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(glm::vec4(ambient, light.Intensity)));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	RefreshCamera();
	Transform::Root->LateUpdate();
}

void Scene::Render()
{
	Transform::Root->Render();
}

void Scene::RenderShadow()
{
	Transform::Root->RenderShadow();
}

void Scene::RefreshCamera()
{
	if (Scene::MainCamera != nullptr)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, _uboPV);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(Scene::MainCamera->GetPerspective()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Scene::MainCamera->GetViewMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, _uboBaseView);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(Scene::MainCamera->gameObject->transform->GetWordPosition()));
		//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(glm::vec3), glm::value_ptr(Scene::MainCamera->gameObject->transform->GetWordForward()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

GameObject* Scene::AddGameObject(string name, Transform* parent)
{
	GameObject* go;
	if (parent == NULL)
		go = new GameObject(name, Transform::Root);
	else
	{
		go = new GameObject(name, parent);
	}
	return go;
}

void Scene::LoadCubemapAndInitIbl(string path)
{
	ibl.Load(path);
	SkyBox = Material(ResourceManager::GetShaderP("SkyBox"));
	SkyBox.ShadowReceive = false;
	SkyBox.DepthWrite = false;
}
