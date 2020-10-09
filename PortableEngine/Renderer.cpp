#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Renderer::Renderer(IPlatform* plat, Camera* cam) : camera(cam)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	vertex = new Shader(plat->GetAssetPath("../../Shaders/GLSL/vertex.glsl"), GL_VERTEX_SHADER);
	pixel = new Shader(plat->GetAssetPath("../../Shaders/GLSL/fragment.glsl"), GL_FRAGMENT_SHADER);
	vertex->Compile();
	pixel->Compile();
	program = glCreateProgram();
	glAttachShader(program, vertex->GetId());
	glAttachShader(program, pixel->GetId());
	glLinkProgram(program);
	glUseProgram(program);
	projLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	glm::mat4 model(1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	//glm::mat4 proj = glm::perspective(45.f, (float)800 / 600, .1f, 100.f);
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
}

void Renderer::LoadMesh(std::vector<Vertex> rawVertices)
{
	glBufferData(GL_ARRAY_BUFFER, rawVertices.size() * sizeof(Vertex), rawVertices.data(), GL_STATIC_DRAW);
	GLint posAttrib = glGetAttribLocation(program, "in_position");
	glEnableVertexAttribArray(posAttrib);
	SetupAttribute(posAttrib, 3, GL_FLOAT, Vertex, Position);
	numVertices = rawVertices.size();
}

void Renderer::Draw()
{

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

void Renderer::Update()
{
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
}

Renderer::~Renderer()
{
	delete vertex;
	delete pixel;
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
		this->numVertices = other.numVertices;
		this->camera = other.camera;
	}
	return *this;
}

Renderer::Renderer(Renderer&& other) noexcept
{
	*this = std::move(other);
}

Renderer::Renderer(Renderer& other)
{
	*this = std::move(other);
}
