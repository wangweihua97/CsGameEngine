#ifndef Component_H
#define Component_H
#include <string>
using namespace std;
class GameObject;
class Component
{
public:
	virtual string GetName();
	GameObject* gameObject;
	Component();
	Component(GameObject* gameObject);
	void SetNext(Component* next);
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();
	virtual void RenderShadow();
	virtual void OnDestory();
	virtual ~Component();
private:
	Component* _next;
	string _name;
protected:
	void SetName(string name);

};
#endif

