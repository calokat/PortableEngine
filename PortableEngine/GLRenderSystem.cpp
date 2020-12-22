#include "GLRenderSystem.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "CameraSystem.h"

static glm::mat4 newView(1.0f);

namespace GLRenderSystem
{
	void Load(GLRenderer& renderer, Camera camera)
	{
		glGenVertexArrays(1, &renderer.vao);
		glBindVertexArray(renderer.vao);
		glGenBuffers(1, &renderer.vbo);
		glGenBuffers(1, &renderer.ibo);
		glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
		renderer.program = glCreateProgram();
		glAttachShader(renderer.program, renderer.vertex.GetId());
		glAttachShader(renderer.program, renderer.pixel.GetId());
		glLinkProgram(renderer.program);
		glUseProgram(renderer.program);
		renderer.projLoc = glGetUniformLocation(renderer.program, "projection");
		renderer.viewLoc = glGetUniformLocation(renderer.program, "view");
		renderer.modelLoc = glGetUniformLocation(renderer.program, "model");
		renderer.colorLoc = glGetUniformLocation(renderer.program, "in_color");
		glm::mat4 model(1.0f);
		glUniformMatrix4fv(renderer.modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(renderer.viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
		glUniformMatrix4fv(renderer.projLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));
		glUniform4f(renderer.colorLoc, renderer.vertexColor.x, renderer.vertexColor.y, renderer.vertexColor.z, renderer.vertexColor.w);
	}

	void LoadMesh(GLRenderer& renderer, Mesh& mesh)
	{
		glBindVertexArray(renderer.vao);
		glBufferData(GL_ARRAY_BUFFER, mesh.rawVertices.size() * sizeof(Vertex), mesh.rawVertices.data(), GL_STATIC_DRAW);
		GLint posAttrib = glGetAttribLocation(renderer.program, "in_position");
		glEnableVertexAttribArray(posAttrib);
		SetupAttribute(posAttrib, 3, GL_FLOAT, Vertex, Position);
		//GLint vertColorAttrib = glGetAttribLocation(renderer.program, "in_color");
		GLint vertColorAttrib = glGetUniformLocation(renderer.program, "in_color");
		//glEnableVertexAttribArray(vertColorAttrib);

		//SetupAttribute(vertColorAttrib, 3, GL_FLOAT, Vertex, Color);
		//glVertexAttribPointer(vertColorAttrib, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)offsetof(structure, element));
		renderer.numVertices = mesh.rawVertices.size();
		glBindVertexArray(0);
	}

	void Draw(GLRenderer& renderer)
	{
		glBindVertexArray(renderer.vao);
		if (renderer.numVertices == 2)
		{
			glDrawArrays(GL_LINES, 0, renderer.numVertices);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, renderer.numVertices);
		}
		glBindVertexArray(0);
	}

	void UpdateRenderer(GLRenderer& renderer, Transform meshTransform, Camera camera)
	{
		glUseProgram(renderer.program);
		glUniformMatrix4fv(renderer.viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
		glUniformMatrix4fv(renderer.projLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));
		glUniformMatrix4fv(renderer.modelLoc, 1, GL_FALSE, glm::value_ptr(meshTransform.worldMatrix));
		glUniform4f(renderer.colorLoc, renderer.vertexColor.x, renderer.vertexColor.y, renderer.vertexColor.z, renderer.vertexColor.w);
	}

	void DrawGizmo(Camera camera)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(glm::value_ptr(camera.view), glm::value_ptr(camera.projection), ImGuizmo::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(newView));
	}
}