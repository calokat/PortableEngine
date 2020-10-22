#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "GL/glew.h"

// source: https://github.com/IGME-RIT/Basic-OpenGL-with-GLFW-Assimp/blob/master/OpenGLObjectLoading/mesh.cpp
#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \

void Load(Renderer& renderer);
void LoadMesh(Renderer& renderer, Mesh mesh);
void Draw(Renderer& renderer);
void UpdateRenderer(Renderer& renderer);
void DrawGizmo(Camera camera);