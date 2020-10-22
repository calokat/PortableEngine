#include "RenderSystem.h"
#include <imgui.h>
#include <ImGuizmo.h>

void Load(Renderer& renderer)
{
	glGenVertexArrays(1, &renderer.vao);
	glBindVertexArray(renderer.vao);
	glGenBuffers(1, &renderer.vbo);
	glGenBuffers(1, &renderer.ibo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
	renderer.program = glCreateProgram();
	glAttachShader(renderer.program, renderer.vertex->GetId());
	glAttachShader(renderer.program, renderer.pixel->GetId());
	glLinkProgram(renderer.program);
	glUseProgram(renderer.program);
	renderer.projLoc = glGetUniformLocation(renderer.program, "projection");
	renderer.viewLoc = glGetUniformLocation(renderer.program, "view");
	renderer.modelLoc = glGetUniformLocation(renderer.program, "model");
	glm::mat4 model(1.0f);
	glUniformMatrix4fv(renderer.modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(renderer.viewLoc, 1, GL_FALSE, glm::value_ptr(renderer.camera->GetViewMatrix()));
	glUniformMatrix4fv(renderer.projLoc, 1, GL_FALSE, glm::value_ptr(renderer.camera->GetProjectionMatrix()));
}

void LoadMesh(Renderer& renderer, Mesh mesh)
{
	glBindVertexArray(renderer.vao);
	glBufferData(GL_ARRAY_BUFFER, mesh.GetRawVertices().size() * sizeof(Vertex), mesh.GetRawVertices().data(), GL_STATIC_DRAW);
	GLint posAttrib = glGetAttribLocation(renderer.program, "in_position");
	glEnableVertexAttribArray(posAttrib);
	SetupAttribute(posAttrib, 3, GL_FLOAT, Vertex, Position);
	GLint vertColorAttrib = glGetAttribLocation(renderer.program, "in_color");
	glEnableVertexAttribArray(vertColorAttrib);
	SetupAttribute(vertColorAttrib, 3, GL_FLOAT, Vertex, Color);
	renderer.numVertices = mesh.GetRawVertices().size();
	glBindVertexArray(0);
}

void Draw(Renderer& renderer)
{
	glBindVertexArray(renderer.vao);
	glDrawArrays(GL_TRIANGLES, 0, renderer.numVertices);
	glBindVertexArray(0);
}

void UpdateRenderer(Renderer& renderer)
{
	glUseProgram(renderer.program);
	glUniformMatrix4fv(renderer.viewLoc, 1, GL_FALSE, glm::value_ptr(renderer.camera->GetViewMatrix()));
	glUniformMatrix4fv(renderer.projLoc, 1, GL_FALSE, glm::value_ptr(renderer.camera->GetProjectionMatrix()));
}

void DrawGizmo(Camera camera)
{
	static glm::mat4 newView(1.0f);
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(glm::value_ptr(camera.GetViewMatrix()), glm::value_ptr(camera.GetProjectionMatrix()), ImGuizmo::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(newView));
}
