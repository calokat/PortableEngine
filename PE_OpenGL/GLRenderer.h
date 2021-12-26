#pragma once
#include "Shader.h"
//#include "IGraphicsAPI.h"
#include "IPlatform.h"
#include "Camera.h"
#include <string>
#include <vector>
#include "Vertex.h"
#include "IRenderer.h"
#include "OpenGLShaderProgram.h"
// source: https://github.com/IGME-RIT/Basic-OpenGL-with-GLFW-Assimp/blob/master/OpenGLObjectLoading/mesh.cpp
#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \


class GLRenderer : public IRenderer
{
public:
	int numVertices;
	unsigned int numIndices;
	GLRenderer(std::string vertexShaderPathParam, std::string fragmentShaderPathParam, ShaderType type);
	GLRenderer();
	glm::vec4 vertexColor = { 1, 1, 1, 1 };
	PEImage diffuseTexture;
	OpenGLShaderProgram shaderProgram;
};