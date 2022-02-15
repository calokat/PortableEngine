#include "OpenGLAPI.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui_impl_opengl3.h>
#include "CameraSystem.h"

int OpenGLAPI::Init()
{
	glContext->GetContext(window);
	glEnable(GL_DEPTH_TEST);
	ImGui_ImplOpenGL3_Init();
	return 0;
}

void OpenGLAPI::ClearScreen()
{
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // rgb(33,150,243)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLAPI::Draw()
{
	ClearScreen();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void OpenGLAPI::_SwapBuffers()
{
	glContext->_SwapBuffers(-1);
}

void OpenGLAPI::NewGuiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
}

void OpenGLAPI::DrawGui()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void OpenGLAPI::OnResize()
{
	glViewport(window->x, window->y, window->width, window->height);
	CameraSystem::CalculateProjectionMatrixLH(engineCam, (float)window->width / window->height);
}

OpenGLAPI::OpenGLAPI(GameWindow* win, IPlatform* plat, Camera& cam) : window(win), platform(plat), engineCam(cam)
{
#ifdef _WIN64
	glContext = new WinOpenGLContext(platform);
#elif defined(__EMSCRIPTEN__)
	glContext = new OpenGLESContext(platform);
#endif
}

OpenGLAPI::~OpenGLAPI()
{
	ImGui_ImplOpenGL3_Shutdown();
	delete glContext;
	delete vertex;
	delete pixel;
}
#ifndef __EMSCRIPTEN__
#endif
void OpenGLAPI::BindToScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(window->x, window->y, window->width, window->height);
	//OnResize();
}

IOpenGLContext* OpenGLAPI::GetOpenGLContext()
{
	return glContext;
}
