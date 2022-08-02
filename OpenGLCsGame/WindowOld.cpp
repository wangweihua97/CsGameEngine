#include "WindowOld.h"
#include "Scene/Scene.h"
#include "Time.h"
#include "Input.h"
#include "EventCenter.h"
#include "Res/ResourceManager.h"
#include <boost/bind/bind.hpp>
#include "Render/Shader.h"
#include <stb_image.h>
#include "Tween.h"
WindowOld* WindowOld::Instance = NULL;
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
WindowOld::WindowOld(HWND hwnd)
{
	m_hwnd = hwnd;
	Width = 1024;
	Height = 1024;
	/*
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


#endif

	// glfw window creation
	// --------------------
	window_ptr = glfwCreateWindow(width, height, "CSGame", NULL, NULL);
	if (window_ptr == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window_ptr);
	glfwSetFramebufferSizeCallback(window_ptr, Resize);
	glfwSetCursorPosCallback(window_ptr, mouseCallback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}*/
	WindowOld::Instance = this;

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
	Resize(nullptr, 1024, 1024); 
	/*
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = 40;
	pfd.nVersion = 1;
	//支持绘制到窗口、支持OPENGL、支持GDI
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 0;
	pfd.cRedBits = 0;
	pfd.cRedShift = 0;
	pfd.cGreenBits = 0;
	pfd.cGreenShift = 0;
	pfd.cBlueBits = 0;
	pfd.cBlueShift = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAccumBits = 0;
	pfd.cAccumRedBits = 0;
	pfd.cAccumGreenBits = 0;
	pfd.cAccumBlueBits = 0;
	pfd.cAccumAlphaBits = 0;
	pfd.cDepthBits = 0;
	pfd.cStencilBits = 0;
	pfd.cAuxBuffers = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.bReserved = 0;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.dwDamageMask = 0;

	//获取屏幕的设备环境
	HDC hdc = GetDC(hwnd);
	//选择像素格式
	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	//设置像素格式
	SetPixelFormat(hdc, pixelFormat, &pfd);
	//创建OpenGL渲染环境
	HGLRC hglrc = wglCreateContext(hdc);
	//为当前线程指定设备环境和渲染环境
	wglMakeCurrent(hdc, hglrc);
	gladLoadGL();
	Resize(nullptr, 100, 100);*/
	stbi_set_flip_vertically_on_load2(true);
	InitGbuffer();
	InitFramebuffer();
	InitResource();
	InitEvent();
	InitInput();
	InitSkyBox();
	InitScene();

	//starTime = glfwGetTime();
	starTime = ((float)GetTickCount()) / 1000;
	currentFrame, lastFrame = starTime;
	SetStartTime(starTime);
}

WindowOld::~WindowOld()
{
	glfwTerminate();
}

void WindowOld::InitEvent()
{
	EventCenter::Instance->AddEvent<string>("Log", boost::bind(&WindowOld::Log, this, placeholders::_1));
	EventCenter::Instance->AddEvent<Key>("KeyDown", boost::bind(&WindowOld::KeyDown, this, placeholders::_1));
	EventCenter::Instance->AddEvent<Key>("KeyUp", boost::bind(&WindowOld::KeyUp, this, placeholders::_1));
}

void WindowOld::InitInput()
{
	_input = new Input();
}

void WindowOld::InitResource()
{
	
	ResourceManager::LoadShader("Shader/cube_vs.vs", "Shader/cube_fs.fs", nullptr, "CubeShader");
	ResourceManager::LoadShader("Shader/model.vs", "Shader/model.fs", nullptr, "ModelShader");
	ResourceManager::LoadShader("Shader/g_skeleton_model.vs", "Shader/g_skeleton_model.fs", nullptr, "SkeletonModelShader");
	ResourceManager::LoadShader("Shader/ambientGBuffer.vs", "Shader/ambientGBuffer.fs", nullptr, "AmbientGBuffer");
	ResourceManager::LoadShader("Shader/specularGBuffer.vs", "Shader/specularGBuffer.fs", nullptr, "SpecularGBuffer");
	ResourceManager::LoadShader("Shader/readSkyBox.vs", "Shader/readSkyBox.fs", nullptr, "ReadSkyBox");
	ResourceManager::LoadShader("Shader/skyBox.vs", "Shader/skyBox.fs", nullptr, "SkyBox");
	ResourceManager::LoadShader("Shader/animationVolume.vs", "Shader/volume.fs", "Shader/volume.gs", "Volume");
	ResourceManager::LoadShader("Shader/final.vs", "Shader/final.fs", nullptr, "Final");
	_gBufferShader = ResourceManager::GetShaderP("AmbientGBuffer");
	_specularGBufferShader = ResourceManager::GetShaderP("SpecularGBuffer");
	_framebufferShader = ResourceManager::GetShaderP("Final");
	ResourceManager::LoadTexture("Texture/map.jpeg", false, "map");
}

void Resize(GLFWwindow* window, int width, int height)
{
	/*glfwGetWindowSize(window, &width, &height);
	if (width != WindowOld::Instance->Width || WindowOld::Instance->Height)
	{
		WindowOld::Instance->Width = width;
		WindowOld::Instance->Height = height;
		glViewport(0, 0, width, height);
		printf("resized \n");
	}*/
	/*WindowOld::Instance->Width = width;
	WindowOld::Instance->Height = height;
	glViewport(0, 0, width, height);*/
}
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Input::xoffset = xpos - Input::xpos;
	Input::yoffset = ypos - Input::ypos;

	Input::xpos = xpos;
	Input::ypos = ypos;
}

void WindowOld::Mainloop()
{
	
		//currentFrame = glfwGetTime();
	    currentFrame = ((float)GetTickCount()) / 1000;
		SetCurTime(currentFrame);
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		SetDeltaTime(deltaTime);
		Time::Update();
		_input->Update();
		POINT p;
		GetCursorPos(&p);
		mouseCallback(nullptr, p.x, p.y);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		
		//for each frame 

		Scene::Instace->Update();
		Scene::Instace->LateUpdate();
		Scene::Instace->Render();

		//glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		//RenderShadowVolIntoStencil();
		//SpecularGBuffer();
		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
		// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
		// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
		// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
		// -----------------------------------------------------------------------------------------------------------------------
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDepthMask(GL_FALSE);
		//glDrawBuffer(GL_BACK);
		AmbientGBuffer();
		glEnable(GL_STENCIL_TEST);
		//glEnable(GL_DEPTH_CLAMP);
		//glEnable(GL_DEPTH_TEST);
		//glDepthMask(GL_TRUE);
		RenderShadowVolIntoStencil();

		glDrawBuffer(GL_BACK);
		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_DEPTH_CLAMP);
		// prevent update to the stencil buffer
		glEnable(GL_STENCIL_TEST);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_EQUAL, 0x0, 0xFF);
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);



		//glDrawBuffer(GL_BACK);
		SpecularGBuffer();
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer);
		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);
		RenderSkyBox();
		//glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		_framebufferShader->Use();
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(_framebufferShader->ID, "finalTexture"), 0);
		glBindTexture(GL_TEXTURE_2D, m_texColorBuffer);
		renderQuad();
		glDisable(GL_BLEND);
		/*glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
		/*glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
		/*glDepthFunc(GL_LEQUAL);
		RenderSkyBox();
		glDepthFunc(GL_LESS);*
		
		//glClear(GL_DEPTH_BUFFER_BIT);

		/*glfwSwapBuffers(window_ptr);
		glfwPollEvents();*/
		glFlush();
		//SwapBuffers(GetDC(m_hwnd));
	
}

void WindowOld::InitGbuffer()
{
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//位置缓冲
	glGenTextures(1, &g_positionMap);
	glBindTexture(GL_TEXTURE_2D, g_positionMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_positionMap, 0);

	//颜色缓冲
	glGenTextures(1, &g_albedoMap);
	glBindTexture(GL_TEXTURE_2D, g_albedoMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_albedoMap, 0);

	//高光颜色缓冲
	glGenTextures(1, &g_specularMap);
	glBindTexture(GL_TEXTURE_2D, g_specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_specularMap, 0);

	//法线缓冲
	glGenTextures(1, &g_normalMap);
	glBindTexture(GL_TEXTURE_2D, g_normalMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, g_normalMap, 0);

	//高度缓冲,AO缓冲 ,光泽度缓冲,粗糙度缓冲
	glGenTextures(1, &g_height_ao_metallic_roughnessMap);
	glBindTexture(GL_TEXTURE_2D, g_height_ao_metallic_roughnessMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, g_height_ao_metallic_roughnessMap, 0);


	unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4,};
	glDrawBuffers(5, attachments);

	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WindowOld::SetDeltaTime(float time)
{
	Time::_deltaTime = time;
}

void WindowOld::SetStartTime(float time)
{
	Time::_startTime = time;
}

void WindowOld::SetCurTime(float time)
{
	Time::_curTime = time;
}

void WindowOld::InitScene()
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

void WindowOld::renderQuad()
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

void WindowOld::Log(string str)
{
	cout << str << endl;
}

void WindowOld::InitSkyBox()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	string skyboxPath = "Resources/hdr/parched_canal_4k.hdr";
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	// pbr: load the HDR environment map
	// ---------------------------------
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(skyboxPath.c_str(), &width, &height, &nrComponents, 0);
	unsigned int hdrTexture;
	if (data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}

	// pbr: setup cubemap to render to and attach to framebuffer
	// ---------------------------------------------------------
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
	// ----------------------------------------------------------------------------------------------
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// pbr: convert HDR equirectangular environment map to cubemap equivalent
	// ----------------------------------------------------------------------
	ResourceManager::GetShaderP("ReadSkyBox")->Use();
	ResourceManager::GetShaderP("ReadSkyBox")->SetInteger("equirectangularMap", 0);
	ResourceManager::GetShaderP("ReadSkyBox")->SetMatrix4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	//glUniform1i(glGetUniformLocation(ResourceManager::GetShaderP("ReadSkyBox")->ID, "equirectangularMap"), 0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		ResourceManager::GetShaderP("ReadSkyBox")->SetMatrix4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Width, Height);
	//glDeleteTextures(1, &hdrTexture);
}

void WindowOld::RenderCube()
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

void WindowOld::RenderSkyBox()
{
	ResourceManager::GetShaderP("SkyBox")->Use();
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(ResourceManager::GetShaderP("SkyBox")->ID, "environmentMap"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
	RenderCube();
}

void WindowOld::AmbientGBuffer()
{
	_gBufferShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(_gBufferShader->ID, "gPosition"), 0);
	// and finally bind the texture
	glBindTexture(GL_TEXTURE_2D, g_positionMap);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(_gBufferShader->ID, "gAlbedoMap"), 1);
	glBindTexture(GL_TEXTURE_2D, g_albedoMap);
	glActiveTexture(GL_TEXTURE2);
	glUniform1i(glGetUniformLocation(_gBufferShader->ID, "gSpecularMap"), 2);
	glBindTexture(GL_TEXTURE_2D, g_specularMap);
	glActiveTexture(GL_TEXTURE3);
	glUniform1i(glGetUniformLocation(_gBufferShader->ID, "gNormalMap"), 3);
	glBindTexture(GL_TEXTURE_2D, g_normalMap);
	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(_gBufferShader->ID, "gHeightAoMetallicRoughnessMap"), 4);
	glBindTexture(GL_TEXTURE_2D, g_height_ao_metallic_roughnessMap);
	renderQuad();
}

void WindowOld::SpecularGBuffer()
{
	_specularGBufferShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(_specularGBufferShader->ID, "gPosition"), 0);
	// and finally bind the texture
	glBindTexture(GL_TEXTURE_2D, g_positionMap);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(_specularGBufferShader->ID, "gAlbedoMap"), 1);
	glBindTexture(GL_TEXTURE_2D, g_albedoMap);
	glActiveTexture(GL_TEXTURE2);
	glUniform1i(glGetUniformLocation(_specularGBufferShader->ID, "gSpecularMap"), 2);
	glBindTexture(GL_TEXTURE_2D, g_specularMap);
	glActiveTexture(GL_TEXTURE3);
	glUniform1i(glGetUniformLocation(_specularGBufferShader->ID, "gNormalMap"), 3);
	glBindTexture(GL_TEXTURE_2D, g_normalMap);
	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(_specularGBufferShader->ID, "gHeightAoMetallicRoughnessMap"), 4);
	glBindTexture(GL_TEXTURE_2D, g_height_ao_metallic_roughnessMap);
	renderQuad();
}

void WindowOld::InitFramebuffer()
{
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	glGenTextures(1, &m_texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texColorBuffer, 0);
	glGenRenderbuffers(1, &m_frameRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_frameRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_frameRenderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WindowOld::KeyDown(Key key)
{
	Log(to_string((int)key));
}

void WindowOld::KeyUp(Key key)
{
	Log(to_string((int)key));
}

void WindowOld::RenderShadowVolIntoStencil()
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
