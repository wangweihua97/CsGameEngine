#ifndef CsEvents_H
#define CsEvents_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#ifndef DLLFUNC
#define DLLFUNC(return_type) extern "C" __declspec(dllexport) return_type __cdecl
#endif
void IniCsEvents();
DLLFUNC(void) CsEvent_Bool(char* s ,bool b);
DLLFUNC(void) CsEvent_IntPtr_Bool(char* s, void* p ,bool b);

DLLFUNC(void) CsEvent_Int(char* s, int i);
DLLFUNC(void) CsEvent_IntPtr_Int(char* s, void* p, int i);

DLLFUNC(void) CsEvent_Float(char* s, float f);
DLLFUNC(void) CsEvent_IntPtr_Float(char* s, void* p, float f);

DLLFUNC(void) CsEvent_String(char* s, char* str);
DLLFUNC(void) CsEvent_IntPtr_String(char* s, void* p, char* str);

DLLFUNC(void) CsEvent_Vec3(char* s, glm::vec3 v);
DLLFUNC(void) CsEvent_IntPtr_Vec3(char* s, void* p, glm::vec3 v);

DLLFUNC(void) CsEvent_Vec4(char* s, glm::vec4 v);
DLLFUNC(void) CsEvent_IntPtr_Vec4(char* s, void* p, glm::vec4 v);

DLLFUNC(void) CsEvent_Mat4(char* s, glm::mat4 m);
DLLFUNC(void) CsEvent_IntPtr_Mat4(char* s, void* p, glm::mat4 m);

DLLFUNC(void) CsEvent_IntPtr_String_Float(char* s, void* p, char* str ,float f);
DLLFUNC(void) CsEvent_IntPtr_String_Vec3(char* s, void* p, char* str ,glm::vec3 v);
DLLFUNC(void) CsEvent_IntPtr_String_Vec4(char* s, void* p, char* str, glm::vec4 v);
DLLFUNC(void) CsEvent_IntPtr_String_String_Int(char* s, void* p, char* str , char* str2, int i);
#endif 

