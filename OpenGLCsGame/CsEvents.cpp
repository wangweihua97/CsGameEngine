#include "CsEvents.h"
#include "EventCenter.h"
#include "Render/Material.h"
#include <boost/bind/bind.hpp>
extern void Material_SetUsePbr(void* p, bool b);
extern void Material_SetFloat(void* p, string name, float f);
extern void Material_SetVec3(void* p, string name, glm::vec3 v);
extern void Material_SetVec4(void* p, string name, glm::vec4 v);
extern void Material_SetTex2D(void* p, string name, string path ,int bindPos);


void IniCsEvents()
{
	//boost::function<void(void*, string, float)> a = boost::bind(Material_SetFloat, placeholders::_1, placeholders::_2, placeholders::_3);
	//boost::function<void(void*, string, glm::vec3)> b = boost::bind(Material_SetVec3, placeholders::_1, placeholders::_2, placeholders::_3);
	//boost::function<void(void*, string, glm::vec4)> c = boost::bind(Material_SetVec4, placeholders::_1, placeholders::_2, placeholders::_3);
	//boost::function<void(void*, string, string , int)> d = boost::bind(Material_SetTex2D, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	EventCenter::Instance->AddEvent<void*, bool>("Material_SetUsePbr", boost::bind(Material_SetUsePbr, placeholders::_1, placeholders::_2));
	EventCenter::Instance->AddEvent<void*, string , float>("Material_SetFloat", boost::bind(Material_SetFloat, placeholders::_1, placeholders::_2, placeholders::_3));
	EventCenter::Instance->AddEvent<void*, string, glm::vec3>("Material_SetVec3", boost::bind(Material_SetVec3, placeholders::_1, placeholders::_2, placeholders::_3));
	EventCenter::Instance->AddEvent<void*, string, glm::vec4>("Material_SetVec4", boost::bind(Material_SetVec4, placeholders::_1, placeholders::_2, placeholders::_3));
	EventCenter::Instance->AddEvent<void*, string, string, int>("Material_SetTex2D", boost::bind(Material_SetTex2D, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
}

void CsEvent_Bool(char* s, bool b)
{
	EventCenter::Instance->Invoke(s, b);
}
void CsEvent_IntPtr_Bool(char* s, void* p, bool b)
{
	EventCenter::Instance->Invoke(s, p, b);
}

void CsEvent_Int(char* s, int i)
{
	EventCenter::Instance->Invoke(s, i);
}
void CsEvent_IntPtr_Int(char* s, void* p, int i)
{
	EventCenter::Instance->Invoke(s, p, i);
}

void CsEvent_Float(char* s, float f)
{
	EventCenter::Instance->Invoke(s, f);
}
void CsEvent_IntPtr_Float(char* s, void* p, float f)
{
	EventCenter::Instance->Invoke(s, p, f);
}

void CsEvent_String(char* s, char* str)
{
	EventCenter::Instance->Invoke(s, str);
}
void CsEvent_IntPtr_String(char* s, void* p, char* str)
{
	EventCenter::Instance->Invoke(s, p, str);
}

void CsEvent_Vec3(char* s, glm::vec3 v)
{
	EventCenter::Instance->Invoke(s, v);
}
void CsEvent_IntPtr_Vec3(char* s, void* p, glm::vec3 v)
{
	EventCenter::Instance->Invoke(s, p, v);
}

void CsEvent_Vec4(char* s, glm::vec4 v)
{
	EventCenter::Instance->Invoke(s, v);
}
void CsEvent_IntPtr_Vec4(char* s, void* p, glm::vec4 v)
{
	EventCenter::Instance->Invoke(s, p, v);
}

void CsEvent_Mat4(char* s, glm::mat4 m)
{
	EventCenter::Instance->Invoke(s, m);
}
void CsEvent_IntPtr_Mat4(char* s, void* p, glm::mat4 m)
{
	EventCenter::Instance->Invoke(s, p, m);
}

void CsEvent_IntPtr_String_Float(char* s, void* p, char* str, float f)
{
	string ss(s);
	string sstr(str);
	EventCenter::Instance->Invoke(ss, p, sstr,f);
}
void CsEvent_IntPtr_String_Vec3(char* s, void* p, char* str, glm::vec3 v)
{
	string ss(s);
	string sstr(str);
	EventCenter::Instance->Invoke(ss, p, sstr, v);
}
void CsEvent_IntPtr_String_Vec4(char* s, void* p, char* str, glm::vec4 v)
{
	string ss(s);
	string sstr(str);
	EventCenter::Instance->Invoke(ss, p, sstr, v);
}
void CsEvent_IntPtr_String_String_Int(char* s, void* p, char* str, char* str2, int i)
{
	string ss(s);
	string sstr(str);
	string sstr2(str2);
	EventCenter::Instance->Invoke(ss, p, sstr, sstr2,i);
}
