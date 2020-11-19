#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Renderer::Renderer(std::string vertexShaderPathParam, std::string fragmentShaderPathParam)
{
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	//glGenBuffers(1, &vbo);
	//glGenBuffers(1, &ibo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	this->vertexShaderPath = vertexShaderPathParam;
	this->fragmentShaderPath = fragmentShaderPathParam;
#ifndef __EMSCRIPTEN__
	vertex = Shader(vertexShaderPathParam, GL_VERTEX_SHADER);
#else
	vertex = new Shader(fragmentShaderPathParam, GL_VERTEX_SHADER);
#endif
#ifndef __EMSCRIPTEN__
	pixel = Shader(fragmentShaderPathParam, GL_FRAGMENT_SHADER);
#else
	pixel = new Shader(fragmentShaderPathParam, GL_FRAGMENT_SHADER);
#endif
	vertex.Compile();
	pixel.Compile();
	//program = glCreateProgram();
	//glAttachShader(program, vertex->GetId());
	//glAttachShader(program, pixel->GetId());
	//glLinkProgram(program);
	//glUseProgram(program);
	//projLoc = glGetUniformLocation(program, "projection");
	//viewLoc = glGetUniformLocation(program, "view");
	//modelLoc = glGetUniformLocation(program, "model");
	//glm::mat4 model(1.0f);
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
}

Renderer::Renderer()
{
	program = -1;
	vao = -1;
	vbo = -1;
	ibo = -1;
	numVertices = -1;
}

//void Renderer::LoadMesh(std::vector<Vertex> rawVertices)
//{
//	glBindVertexArray(vao);
//	glBufferData(GL_ARRAY_BUFFER, rawVertices.size() * sizeof(Vertex), rawVertices.data(), GL_STATIC_DRAW);
//	GLint posAttrib = glGetAttribLocation(program, "in_position");
//	glEnableVertexAttribArray(posAttrib);
//	SetupAttribute(posAttrib, 3, GL_FLOAT, Vertex, Position);
//	numVertices = rawVertices.size();
//	glBindVertexArray(0);
//}

//void Renderer::Draw()
//{
//	glBindVertexArray(vao);
//	glDrawArrays(GL_TRIANGLES, 0, numVertices);
//	glBindVertexArray(0);
//}

//void Renderer::Update()
//{
//	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
//	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
//}

Renderer::~Renderer()
{
}

Renderer& Renderer::operator=(Renderer&& other)
{
	if (this != &other)
	{
		this->vertex = other.vertex;
		this->pixel = other.pixel;
		this->vao = other.vao;
		this->vbo = other.vbo;
		this->ibo = other.ibo;
		this->program = other.program;
		this->projLoc = other.projLoc;
		this->viewLoc = other.viewLoc;
		this->modelLoc = other.modelLoc;
		this->colorLoc = other.colorLoc;
		this->numVertices = other.numVertices;
		this->vertexShaderPath = other.vertexShaderPath;
		this->fragmentShaderPath = other.fragmentShaderPath;
	}
	return *this;
}

Renderer::Renderer(Renderer&& other) noexcept
{
	*this = std::move(other);
}

Renderer::Renderer(Renderer& other)
{
	if (this != &other)
	{
		*this = Renderer(other.vertexShaderPath, other.fragmentShaderPath);
	}
}
