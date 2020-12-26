#include "GLRenderSystem.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "CameraSystem.h"

static glm::mat4 newView(1.0f);

IRenderer& GLRenderSystem::CreateRenderer(entt::registry& reg, entt::entity& e)
{
	IRenderer& rendererRef = reg.emplace<GLRenderer>(e, platform->GetAssetPath("../../Shaders/GLSL/vertex.glsl"), platform->GetAssetPath("../../Shaders/GLSL/fragment.glsl"));
	return rendererRef;
}

void GLRenderSystem::Load(IRenderer* renderer, Camera& camera)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glGenVertexArrays(1, &glRenderer->vao);
	glBindVertexArray(glRenderer->vao);
	glGenBuffers(1, &glRenderer->vbo);
	glGenBuffers(1, &glRenderer->ibo);
	glBindBuffer(GL_ARRAY_BUFFER, glRenderer->vbo);
	glRenderer->program = glCreateProgram();
	glAttachShader(glRenderer->program, glRenderer->vertex.GetId());
	glAttachShader(glRenderer->program, glRenderer->pixel.GetId());
	glLinkProgram(glRenderer->program);
	glUseProgram(glRenderer->program);
	glRenderer->projLoc = glGetUniformLocation(glRenderer->program, "projection");
	glRenderer->viewLoc = glGetUniformLocation(glRenderer->program, "view");
	glRenderer->modelLoc = glGetUniformLocation(glRenderer->program, "model");
	glRenderer->colorLoc = glGetUniformLocation(glRenderer->program, "in_color");
	glm::mat4 model(1.0f);
	glUniformMatrix4fv(glRenderer->modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glRenderer->viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(glRenderer->projLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));
	glUniform4f(glRenderer->colorLoc, glRenderer->vertexColor.x, glRenderer->vertexColor.y, glRenderer->vertexColor.z, glRenderer->vertexColor.w);
}

void GLRenderSystem::LoadMesh(IRenderer* renderer, Mesh& mesh)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glBindVertexArray(glRenderer->vao);
	glBufferData(GL_ARRAY_BUFFER, mesh.rawVertices.size() * sizeof(Vertex), mesh.rawVertices.data(), GL_STATIC_DRAW);
	GLint posAttrib = glGetAttribLocation(glRenderer->program, "in_position");
	glEnableVertexAttribArray(posAttrib);
	SetupAttribute(posAttrib, 3, GL_FLOAT, Vertex, Position);
	//GLint vertColorAttrib = glGetAttribLocation(glRenderer->program, "in_color");
	GLint vertColorAttrib = glGetUniformLocation(glRenderer->program, "in_color");
	//glEnableVertexAttribArray(vertColorAttrib);

	//SetupAttribute(vertColorAttrib, 3, GL_FLOAT, Vertex, Color);
	//glVertexAttribPointer(vertColorAttrib, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)offsetof(structure, element));
	glRenderer->numVertices = mesh.rawVertices.size();
}

void GLRenderSystem::Draw(IRenderer* renderer)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glBindVertexArray(glRenderer->vao);
	if (glRenderer->numVertices == 2)
	{
		glDrawArrays(GL_LINES, 0, glRenderer->numVertices);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, glRenderer->numVertices);
	}
}

void GLRenderSystem::DrawWireframe(IRenderer* renderer)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glBindVertexArray(glRenderer->vao);
	glUseProgram(glRenderer->program);
	glUniform4f(glRenderer->colorLoc, 1, 1, 1, 1);
	glDrawArrays(GL_LINES, 0, glRenderer->numVertices);
}

void GLRenderSystem::UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera)
{
	GLRenderer* glRenderer = (GLRenderer*)renderer;
	glUseProgram(glRenderer->program);
	glUniformMatrix4fv(glRenderer->viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(glRenderer->projLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));
	glUniformMatrix4fv(glRenderer->modelLoc, 1, GL_FALSE, glm::value_ptr(meshTransform.worldMatrix));
	glUniform4f(glRenderer->colorLoc, glRenderer->vertexColor.x, glRenderer->vertexColor.y, glRenderer->vertexColor.z, glRenderer->vertexColor.w);
}

void GLRenderSystem::DrawGizmo(Camera camera)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(glm::value_ptr(camera.view), glm::value_ptr(camera.projection), ImGuizmo::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(newView));
}

GLRenderSystem::GLRenderSystem(IPlatform* plat) : platform(plat)
{
}
