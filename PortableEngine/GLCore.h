#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include "Core.h"
#include "Camera.h"
#include "Timer.h"
class GLCore : public Core {
public:
	long InitWindow();
	long InitGL();
	void Update(glm::mat4 &view);
	long Run();
	GLCore();
	~GLCore();
private:
	GLFWwindow* m_window;
	const int WIDTH = 800;
	const int HEIGHT = 600;
	int screenWidth;
	int screenHeight;
	Camera* camera;
	Timer timer;
	glm::vec3 cameraPos;
	glm::vec3 cameraUp;
	glm::mat4 cameraRot;
	glm::vec3 cameraFwd;
};