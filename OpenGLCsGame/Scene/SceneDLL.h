#ifndef SceneDLL_H
#define SceneDLL_H
#ifndef DLLFUNC
#define DLLFUNC(return_type) extern "C" __declspec(dllexport) return_type __cdecl
#endif

#include"Scene.h"

DLLFUNC(GameObject*) AddGameObectToScene(char* name, Transform* parent);

DLLFUNC(Scene*) CreatNewScene();

DLLFUNC(Transform*) GetSceneRootTransform();

DLLFUNC(DirectionalLightFromCS) GetMainLight();

DLLFUNC(void) SetMainLight(DirectionalLightFromCS data);

DLLFUNC(void) LoadCubemapAndInitIbl(char* path);
#endif
