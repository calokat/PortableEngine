#pragma once
#include "GLRenderer.h"
#include "Mesh.h"
#include "GL/glew.h"
#include "Camera.h"

namespace GLRenderSystem 
{
	// source: https://github.com/IGME-RIT/Basic-OpenGL-with-GLFW-Assimp/blob/master/OpenGLObjectLoading/mesh.cpp
#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \
	
	void Load(GLRenderer& renderer, Camera camera);
	void LoadMesh(GLRenderer& renderer, Mesh& mesh);
	void Draw(GLRenderer& renderer);
	void DrawWireframe(GLRenderer& renderer);
	void UpdateRenderer(GLRenderer& renderer, Transform meshTransform, Camera camera);
	void DrawGizmo(Camera camera);
}