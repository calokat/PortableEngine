#pragma once
#include "Shader.h"
//#include "IGraphicsAPI.h"
#include "IPlatform.h"
#include "Camera.h"
#include <string>
#include <vector>
#include "Vertex.h"
// source: https://github.com/IGME-RIT/Basic-OpenGL-with-GLFW-Assimp/blob/master/OpenGLObjectLoading/mesh.cpp
#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \


class Renderer
{
public:
	Shader vertex;
	Shader pixel;
	//IGraphicsAPI* glAPI;
	GLuint program;
	GLuint vao, vbo, ibo;
	GLint projLoc, viewLoc, modelLoc, colorLoc;
	int numVertices;
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	Renderer(std::string vertexShaderPathParam, std::string fragmentShaderPathParam);
	Renderer();
	//void LoadMesh(std::vector<Vertex>);
	//void Draw();
	//void Update();
	~Renderer();
	// Move assignment operator
	Renderer& operator=(Renderer&& other);
	// Move constructor
	Renderer (Renderer&& other) noexcept;
	Renderer(Renderer& other);
	glm::vec4 vertexColor = { 0, 0, 0, 1 };
};