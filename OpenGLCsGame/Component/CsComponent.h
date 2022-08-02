#ifndef CsComponent_H
#define CsComponent_H
#include"Component.h"
typedef void (*CallCs)();
class CsComponent : public Component
{
public:
	const float moveSpeed = 50.0f;
	const float cameraSense = 0.1f;
	CallCs _update = 0;
	CallCs _lateUpdate = 0;
	CallCs _render = 0;
	CallCs _onDestory = 0;
	CsComponent(GameObject* gameObject);
	void Update()override;
	void LateUpdate()override;
	void Render()override;
	void OnDestory()override;
};

#ifndef DLLFUNC
#define DLLFUNC(return_type) extern "C" __declspec(dllexport) return_type __cdecl
#endif
DLLFUNC(void) CsComponent_RegisterCallBack(CsComponent* component, CallCs update, CallCs lateUpdate, CallCs render, CallCs onDestory);
#endif

