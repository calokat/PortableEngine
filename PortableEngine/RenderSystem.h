#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "GL/glew.h"
#include "Camera.h"

// source: https://github.com/IGME-RIT/Basic-OpenGL-with-GLFW-Assimp/blob/master/OpenGLObjectLoading/mesh.cpp
#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \

void Load(Renderer& renderer, Camera camera);
void LoadMesh(Renderer& renderer, Mesh mesh);
void Draw(Renderer& renderer);
void UpdateRenderer(Renderer& renderer, Transform meshTransform, Camera camera);
void DrawGizmo(Camera camera);