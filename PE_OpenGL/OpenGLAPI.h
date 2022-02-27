#pragma once
#include "IGraphicsAPI.h"
#include "GameWindow.h"
#ifdef _WIN64
#include "WinOpenGLContext.h"
#elif defined(__EMSCRIPTEN__) || defined(__linux__)
#include "OpenGLESContext.h"
#endif
#include "IPlatform.h"
#include "Shader.h"
#include "Camera.h"

class OpenGLAPI : public IGraphicsAPI
{
public:
	int Init();
	void ClearScreen();
	void Draw();
	OpenGLAPI(GameWindow*, IPlatform*, Camera&);
	void _SwapBuffers();
	void NewGuiFrame();
	void DrawGui();
	void OnResize();
	~OpenGLAPI();
	void BindToScreen();
	// unique to OpenGLAPI, and not shared with the IGraphicsAPI interface
	IOpenGLContext* GetOpenGLContext();
private:
	GameWindow* window;
//#ifdef _WIN64
	//WinOpenGLContext* glContext;
//#elif defined(__EMSCRIPTEN__)
	//OpenGLESContext* glContext;
//#endif
	IOpenGLContext* glContext;
	IPlatform* platform;
	Shader* vertex;
	Shader* pixel;
	Camera& engineCam;
};

