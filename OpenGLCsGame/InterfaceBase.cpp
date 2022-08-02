#include "InterfaceBase.h"
#include <glad/glad.h>
#include <string>
#include "Window.h"
#include "Input.h"

static std::string s_lastError;
static Window* window = NULL;
void output(int i)
{
	cout << i << endl;
}

const char* get_error(int& length)
{
	length = static_cast<int>(s_lastError.length());
	return s_lastError.data();
}

void set_error(std::string error)
{
	s_lastError = move(error);
}

bool initialize(HWND hwnd)
{
	window = new Window(hwnd);
	return true;
}

void updateTime()
{
	window->UpdateTime();
}

bool render()
{
	window->Mainloop();
	return true;
}

bool resize(int width, int height)
{
	// glViewport should not be called with width or height zero
	if (width == 0 || height == 0) return true;

	Resize(window->window_ptr , width , height);
	return true;
}

void CheckKey(int key, bool isDown)
{
	Input::Instance->Check((Key)key, isDown);
}
