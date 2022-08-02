#ifndef InterfaceBase_H
#define InterfaceBase_H
#include <string>
#include<Windows.h>

#ifndef DLLFUNC
#define DLLFUNC(return_type) extern "C" __declspec(dllexport) return_type __cdecl
#endif

/// \brief initializes the opengl functions (glad)
/// \return true on success
DLLFUNC(bool) initialize(HWND hwnd);

DLLFUNC(void) updateTime();

/// \brief main render logic for opengl
/// \return true on success
DLLFUNC(bool) render();

/// \brief indicates that the size of the client area has changed. 
///        Will be called after intitialize as well
/// \param width 
/// \param height 
/// \return true on success
DLLFUNC(bool) resize(int width, int height);

DLLFUNC(void) CheckKey(int key, bool isDown);

/// \brief retrieves last error from set_error()
/// \param length of the error
/// \return last error
DLLFUNC(const char*) get_error(int& length);

/// \brief sets the get_error() return value
/// \param error
void set_error(std::string error);
#endif