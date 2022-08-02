#include "CsComponent.h"
CsComponent::CsComponent(GameObject* gameObject) :Component(gameObject)
{
}

void CsComponent::Update()
{
    __super::Update();
    if (_update != 0)
        _update();
}

void CsComponent::LateUpdate()
{
    __super::LateUpdate();
    if (_lateUpdate != 0)
        _lateUpdate();
}

void CsComponent::Render()
{
    __super::Render();
    if (_render != 0)
        _render();
}

void CsComponent::OnDestory()
{
    __super::OnDestory();
    if (_onDestory != 0)
        _onDestory();
}

void CsComponent_RegisterCallBack(CsComponent* component, CallCs update, CallCs lateUpdate, CallCs render, CallCs onDestory)
{
    component->_update = update;
    component->_lateUpdate = lateUpdate;
    component->_render = render;
    component->_onDestory = onDestory;
}
