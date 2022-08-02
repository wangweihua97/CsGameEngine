#include "Window.h"
#include "Scene/Scene.h"
#include "Time.h"
#include "Input.h"
#include "EventCenter.h"
#include "Res/ResourceManager.h"
#include <boost/bind/bind.hpp>
#include "Render/Shader.h"
#include <stb_image.h>
#include "Tween.h"
#include <thread>
#include "CsEvents.h"
Window* Window::Instance = NULL;
static HMODULE glModleInst;
static void* cWGLGetProcAddr(const char* name)
{
	auto ret = wglGetProcAddress(name);
	if (ret == NULL)
	{
		ret = GetProcAddress(glModleInst, name);
	}
	return ret;
}
static bool selectHDCFormat(HDC hdc) noexcept
{
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	auto pxfmt = ChoosePixelFormat(hdc, &pfd);
	if (pxfmt == 0)                                             // 不支持的像素格式
	{
		return false;
	}
	auto _ = SetPixelFormat(hdc, pxfmt, &pfd);
	assert(_);
	return true;
}
Window::Window(HWND hwnd)
{
	m_hwnd = hwnd;
	Width = 1024;
	Height = 700;
	Window::Instance = this;

	auto hDC = GetDC(hwnd);
	auto glInst = LoadLibraryA("opengl32.dll");
	auto hRC = wglCreateContext(hDC);
	if (wglMakeCurrent(hDC, hRC) == FALSE)
	{
		return;
	}
	assert(wglGetCurrentContext() != NULL);
	glModleInst = glInst;
	if (gladLoadGLLoader(cWGLGetProcAddr) == 0)
	{
		return;
	}
	Resize(nullptr, 1024, 700);
	stbi_set_flip_vertically_on_load2(true);
	//InitGbuffer();
	//InitFramebuffer();
	m_render.SetRenderSize(1024, 700);
	m_render.Init(cWGLGetProcAddr);
	InitResource();
	InitEvent();
	IniCsEvents();
	InitInput();
	//InitSkyBox();
	//InitScene();

	//starTime = glfwGetTime();
	starTime = ((float)GetTickCount()) / 1000;
	currentFrame, lastFrame = starTime;
	SetStartTime(starTime);
}

Window::~Window()
{
	glfwTerminate();
}

void Window::UpdateTime()
{
	currentFrame = (double)GetTickCount64() / 1000;
	SetCurTime(currentFrame);
	float deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	SetDeltaTime(deltaTime);
	Time::Update();
}

void Window::InitEvent()
{
	EventCenter::Instance->AddEvent<string>("Log", boost::bind(&Window::Log, this, placeholders::_1));
	EventCenter::Instance->AddEvent<Key>("KeyDown", boost::bind(&Window::KeyDown, this, placeholders::_1));
	EventCenter::Instance->AddEvent<Key>("KeyUp", boost::bind(&Window::KeyUp, this, placeholders::_1));
}

void Window::InitInput()
{
	_input = new Input();
}

void Window::InitResource()
{
	ResourceManager::LoadShader("Shader/CommonDSN.vs", "Shader/CommonDSN.fs", nullptr, "comShader");
	ResourceManager::LoadShader("Shader/cube_vs.vs", "Shader/cube_fs.fs", nullptr, "CubeShader");
	ResourceManager::LoadShader("Shader/model.vs", "Shader/model.fs", nullptr, "ModelShader");
	//ResourceManager::LoadShader("Shader/g_skeleton_model.vs", "Shader/g_skeleton_model.fs", nullptr, "SkeletonModelShader");
	//ResourceManager::LoadShader("Shader/ambientGBuffer.vs", "Shader/ambientGBuffer.fs", nullptr, "AmbientGBuffer");
	//ResourceManager::LoadShader("Shader/specularGBuffer.vs", "Shader/specularGBuffer.fs", nullptr, "SpecularGBuffer");
	ResourceManager::LoadShader("Shader/readSkyBox.vs", "Shader/readSkyBox.fs", nullptr, "ReadSkyBox");
	ResourceManager::LoadShader("Shader/skyBox.vs", "Shader/skyBox.fs", nullptr, "SkyBox");
	//ResourceManager::LoadShader("Shader/animationVolume.vs", "Shader/volume.fs", "Shader/volume.gs", "Volume");
	//ResourceManager::LoadShader("Shader/final.vs", "Shader/final.fs", nullptr, "Final");
	ResourceManager::LoadShader("Shader/Default.vs", "Shader/Default.fs", nullptr, "Default");
	ResourceManager::LoadShader("Shader/autoModelShader.vs", "Shader/autoModelShader.fs", nullptr, "autoModelShader");
	ResourceManager::LoadShader("Shader/shadow.vs", "Shader/shadow.fs", nullptr, "shadow");
	ResourceManager::LoadShader("Shader/IblPbr.vs", "Shader/IblPbr_M_R_AO.fs", nullptr, "IblPbr_M_R_AO");
	ResourceManager::LoadShader("Shader/IblPbr.vs", "Shader/IblPbr_R_M_AO.fs", nullptr, "IblPbr_R_M_AO");
	ResourceManager::LoadShader("Shader/IblPbr.vs", "Shader/IblPbr_M_R_iAO.fs", nullptr, "IblPbr_M_R_iAO");
	ResourceManager::LoadShader("Shader/IblPbr.vs", "Shader/IblPbr_sM_sR_sAO.fs", nullptr, "IblPbr_sM_sR_sAO");
	ResourceManager::LoadShader("Shader/IblPbr.vs", "Shader/IblPbr_AO_R_M.fs", nullptr, "IblPbr_AO_R_M");
	
	ResourceManager::LoadShader("Shader/Tex2D.vs", "Shader/Tex2D.fs", nullptr, "Tex2D");
	ResourceManager::LoadShader("Shader/quad.vs", "Shader/quad.fs", nullptr, "Quad");
}

void Resize(GLFWwindow* window, int width, int height)
{
	/*glfwGetWindowSize(window, &width, &height);
	if (width != Window::Instance->Width || Window::Instance->Height)
	{
		Window::Instance->Width = width;
		Window::Instance->Height = height;
		glViewport(0, 0, width, height);
		printf("resized \n");
	}*/
	Window::Instance->Width = width;
	Window::Instance->Height = height;
	glViewport(0, 0, width, height);
}
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Input::xoffset = xpos - Input::xpos;
	Input::yoffset = ypos - Input::ypos;

	Input::xpos = xpos;
	Input::ypos = ypos;
}

void Window::Mainloop()
{
		//currentFrame = glfwGetTime();
		_input->Update();
		POINT p;
		GetCursorPos(&p);
		mouseCallback(nullptr, p.x, p.y);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		if (Scene::Instace == nullptr)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			return;
		}

		Scene::Instace->Update();
		Scene::Instace->LateUpdate();
		m_render.SetTarget(m_render.m_CustomTarget);
		Scene::Instace->RenderShadow();
		if (Scene::Instace->ibl.isInit)
			m_render.PushRender(0, &(Scene::Instace->SkyBox), glm::mat4(), glm::mat4(), boost::bind(&Window::RenderSkyBox, this));
		Scene::Instace->Render();
		m_render.RenderPushedCommands();
		m_render.m_PostProcessor->DoPostProcess(m_render.m_CustomTarget);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_render.m_PostProcessTarget->ID);
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_render.m_ShadowRenderTargets[0]->ID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		/*glfwSwapBuffers(window_ptr);
		glfwPollEvents();*/
		glFlush();
		//SwapBuffers(GetDC(m_hwnd));
	
}

void Window::SetDeltaTime(float time)
{
	Time::_deltaTime = time;
}

void Window::SetStartTime(float time)
{
	Time::_startTime = time;
}

void Window::SetCurTime(float time)
{
	Time::_curTime = time;
}

void Window::InitScene()
{
	

	m_scene = Scene::Init();
	/*
	auto x = [this](float p)
	{
		Log("当前时间" + to_string(p));
	};
	Tween::DoTween<float>(0.0f, 2.0f, 2.0f, x);

	auto y = [this](glm::vec3 p)
	{
		Log("当前x值" + to_string(p.x) + "---当前y值" + to_string(p.y) + "---当前z值" + to_string(p.z));
	};
	Tween::DoTween<glm::vec3>(glm::vec3(0.0,1.0,2.0), glm::vec3(10.0, 2.0, -2.0), 1.0f, y ,Ease::InOutSine);
	auto a = [this]()
	{
		int k = 0;
		Log(to_string(Width) + "----------" + to_string(Height));
		this->Log("延迟2秒后");
		auto b = [&k ,this]()
		{
			this->Log("延迟1秒输出" + to_string(k));
			k++;
		};
		TimesScheduleCommand* c = new TimesScheduleCommand(b, 10, 1);
		auto m = [this]()
		{
			this->Log("任务完成");
		};
		c->onComplete = m;
		Time::PushCommand(c);
	};
	DelayScheduleCommand* d = new  DelayScheduleCommand(a, 2.0);
	Time::PushCommand(d);*/
	
}

void Window::renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Window::Log(string str)
{
	cout << str << endl;
}

void Window::RenderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	glDisable(GL_CULL_FACE);
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);
}

void Window::RenderSkyBox()
{
	//ResourceManager::GetShaderP("SkyBox")->SetInteger("environmentMap", 0);
	//glUniform1i(glGetUniformLocation(ResourceManager::GetShaderP("SkyBox")->ID, "environmentMap"), 0);
	ResourceManager::GetShaderP("SkyBox")->SetInteger("environmentMap", 14);
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Scene::Instace->ibl.envCubemap);
	
	RenderCube();
}

void Window::KeyDown(Key key)
{
	Log(to_string((int)key) + "down");
}

void Window::KeyUp(Key key)
{
	Log(to_string((int)key) + "up");
}

void Window::RenderShadowVolIntoStencil()
{
	glDrawBuffer(GL_NONE);
	glDepthMask(GL_FALSE);

	glDisable(GL_CULL_FACE);

	// We need the stencil test to be enabled but we want it
	// to succeed always. Only the depth test matters.
	glStencilFunc(GL_ALWAYS, 0, 0xff);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	Scene::Instace->RenderShadow();

	glEnable(GL_CULL_FACE);
}

void stbi_set_flip_vertically_on_load2(bool b)
{
	stbi_set_flip_vertically_on_load(b);
}
