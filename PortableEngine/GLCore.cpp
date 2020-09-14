#if 0
#pragma once
#include "GLCore.h"
#include <iostream>
#include "Mesh.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Vertex.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
long GLCore::InitWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_window = glfwCreateWindow(WIDTH, HEIGHT, "OpenXR", nullptr, nullptr);
	glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

	if (m_window == nullptr)
	{
		// If the window returns a null pointer, meaning the window creation was not successful
		// we print out the messsage and terminate the glfw using glfwTerminate()
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		// Since the application was not able to create a window we exit the program returning EXIT_FAILURE
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(m_window);
	return S_OK;
}

long GLCore::InitGL()
{
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		// If the initalization is not successful, print out the message and exit the program with return value EXIT_FAILURE
		std::cout << "Failed to initialize GLEW" << std::endl;

		return EXIT_FAILURE;
	}
	glViewport(0, 0, screenWidth, screenHeight);
	return S_OK;
}

void GLCore::Update(glm::mat4& view)
{
	float movementSpeed = 10;
	float dt = timer.Tick();
	if (GetAsyncKeyState('W') & 0x8000)
	{
		//view = glm::translate(view, glm::vec3(0, 0, dt * movementSpeed));
		cameraPos += glm::vec3(0, 0, dt * movementSpeed);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		//view = glm::translate(view, glm::vec3(0, 0, -dt * movementSpeed));
		cameraPos += glm::vec3(0, 0, -dt * movementSpeed);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		//view = glm::translate(view, glm::vec3(-dt * movementSpeed, 0, 0));
		cameraPos += glm::vec3(dt * movementSpeed, 0, 0);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		//view = glm::translate(view, glm::vec3(dt * movementSpeed, 0, 0));
		cameraPos += glm::vec3(-dt * movementSpeed, 0, 0);
	}
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		//glm::translate(view, glm::vec3(0, -dt * movementSpeed, 0));
		cameraPos += glm::vec3(0, -dt * movementSpeed, 0);
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		//glm::translate(view, glm::vec3(0, dt * movementSpeed, 0));
		cameraPos += glm::vec3(0, dt * movementSpeed, 0);
	}
	view = glm::lookAt(cameraPos, cameraPos + cameraFwd, cameraUp);
}

long GLCore::Run() 
{
	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	//glGenBuffers(1, &ibo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	Shader vertexShader = Shader(GetFullPathTo("../../Shaders/GLSL/vertex.glsl"), GL_VERTEX_SHADER);
	vertexShader.Compile();
	Shader fragmentShader = Shader(GetFullPathTo("../../Shaders/GLSL/fragment.glsl"), GL_FRAGMENT_SHADER);
	fragmentShader.Compile();
	GLint program = glCreateProgram();
	glAttachShader(program, vertexShader.GetId());
	glAttachShader(program, fragmentShader.GetId());
	glLinkProgram(program);
	glUseProgram(program);
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//camera->GetTransform()->MoveRelative(glm::vec3(5, 0, -5));

	glm::mat4 model(1.0f);
	//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFwd, cameraUp);
	//view = glm::translate(view, glm::vec3(0, 0, -5));
	//glm::mat4 proj = glm::perspective(35.0f, float(WIDTH) / HEIGHT, .1f, 50.0f);
	//glm::mat4 proj(1.0f);
	GLint modelLoc = glGetUniformLocation(program, "model");
	GLint viewLoc = glGetUniformLocation(program, "view");
	GLint projLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
	Mesh sphere = Mesh(GetFullPathTo("../../Assets/Models/sphere.obj").c_str());
	//glBufferData(GL_ARRAY_BUFFER, sphere.GetRawVertices().size() * sizeof(Vertex), &sphere.GetRawVertices()[0], GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.GetRawIndices().size() * sizeof(UINT), &sphere.GetRawIndices()[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	HWND hwnd = glfwGetWin32Window(m_window);
	camera->UpdateProjectionMatrix((float)screenWidth / screenHeight);
	//camera->Update(0, hwnd);
	timer.Start();
	while (!glfwWindowShouldClose(m_window))
	{
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);
		camera->Update(timer.Tick(), hwnd);
		glClearColor(1, 1, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		//Update(view);
		static float angle = 0;
		model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
		angle += .005f;
		if (angle > 360)
		{
			angle = 0;
		}
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, sphere.GetRawVertices().size());
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	glfwTerminate();
	return S_OK;
}

GLCore::GLCore()
{
	m_window = nullptr;
	screenWidth = -1;
	screenHeight = -1;
	camera = new Camera(glm::vec3(0, 0, -5), (float)WIDTH / HEIGHT);
	cameraUp = glm::vec3(0, 1, 0);
	cameraFwd = glm::vec3(0, 0, 1);
	//camera = Camera(glm::vec3(0, 0, 1), 3.14f / 2);
}

GLCore::~GLCore()
{
	delete camera;
}
#endif