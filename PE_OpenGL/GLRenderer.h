#pragma once
#include "Shader.h"
//#include "IGraphicsAPI.h"
#include "IPlatform.h"
#include "Camera.h"
#include <string>
#include <vector>
#include "Vertex.h"
#include "IRenderer.h"
// source: https://github.com/IGME-RIT/Basic-OpenGL-with-GLFW-Assimp/blob/master/OpenGLObjectLoading/mesh.cpp
#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \


class GLRenderer : public IRenderer
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
	unsigned int numIndices;
	GLRenderer(std::string vertexShaderPathParam, std::string fragmentShaderPathParam);
	GLRenderer();
	//void LoadMesh(std::vector<Vertex>);
	//void Draw();
	//void Update();
	~GLRenderer();
	// Move assignment operator
	GLRenderer& operator=(GLRenderer&& other);
	// Move constructor
	GLRenderer (GLRenderer&& other) noexcept;
	GLRenderer(GLRenderer& other);
	glm::vec4 vertexColor = { 1, 1, 1, 1 };
	PEImage diffuseTexture;
};