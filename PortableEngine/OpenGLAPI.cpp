#include "OpenGLAPI.h"
//#include <GL/glew.h>
//#include <GL/wglew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <backends/imgui_impl_opengl3.h>
#include "CameraSystem.h"
#include "XRGraphicsPlugin_OpenGL.h"

//float vertices[] = {
//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
// 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
// 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
// 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
// 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
// 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//};

int OpenGLAPI::Init()
{
	glContext->GetContext(window);
	glEnable(GL_DEPTH_TEST);
	//GLuint vao, vbo;
	//glGenVertexArrays(1, &vao);
	//glGenBuffers(1, &vbo);
	//glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//vertex = new Shader("C:/Users/caleb/Documents/GitHub/PortableEngine/Shaders/GLSL/vertex.glsl", GL_VERTEX_SHADER);
	//vertex->Compile();
	//pixel = new Shader("C:/Users/caleb/Documents/GitHub/PortableEngine/Shaders/GLSL/fragment.glsl", GL_FRAGMENT_SHADER);
	//pixel->Compile();
	//GLint program = glCreateProgram();
	//glAttachShader(program, vertex->GetId());
	//glAttachShader(program, pixel->GetId());
	//glLinkProgram(program);
	//glUseProgram(program);



	//glm::mat4 model(1.0f);
	//GLint modelLoc = glGetUniformLocation(program, "model");
	//GLint viewLoc = glGetUniformLocation(program, "view");
	//GLint projLoc = glGetUniformLocation(program, "projection");
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//glm::mat4 view = glm::lookAtRH(glm::vec3(0, 0, -5), glm::vec3(0, 0, -4), glm::vec3(0, 1, 0));
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	//glm::mat4 proj = glm::perspective(3.14f / 3, (float)window->width / window->height, .1f, 100.f);
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//GLint posLoc = glGetAttribLocation(program, "in_position");
	//glEnableVertexAttribArray(posLoc);
	//glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)0);
	ImGui_ImplOpenGL3_Init();
	return 0;
}

void OpenGLAPI::ClearScreen()
{
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // rgb(33,150,243)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//BOOL res = SwapBuffers((HDC)window->deviceContext);
	// TODO: Make swapping buffers the Platform API's responsibility
	//glContext->_SwapBuffers(window->deviceContext);
	//glContext->_SwapBuffers(-1);
	// SwapBuffers((HDC)window->deviceContext);
}

void OpenGLAPI::Draw()
{
	ClearScreen();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glContext->_SwapBuffers(-1);
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
	xrGraphics = new XRGraphicsPlugin_OpenGL(window, glContext);
}

OpenGLAPI::~OpenGLAPI()
{
	ImGui_ImplOpenGL3_Shutdown();
	delete glContext;
	delete vertex;
	delete pixel;
	delete xrGraphics;
}

IXRGraphicsPlugin* OpenGLAPI::GetXRGraphicsPlugin()
{
	return xrGraphics;
}

void OpenGLAPI::BindToScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//OnResize();
}
