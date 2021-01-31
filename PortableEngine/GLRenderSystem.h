#pragma once
#include "GLRenderer.h"
#include "Mesh.h"
#include "GL/glew.h"
#include "Camera.h"
#include "IRenderSystem.h"
// source: https://github.com/IGME-RIT/Basic-OpenGL-with-GLFW-Assimp/blob/master/OpenGLObjectLoading/mesh.cpp
#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \

class GLRenderSystem : public IRenderSystem
{
public:
	IRenderer& CreateRenderer(entt::registry& reg, entt::entity& e);
	void Load(IRenderer* renderer, Camera& camera);
	void LoadMesh(IRenderer* renderer, Mesh& mesh);
	void Draw(IRenderer* renderer);
	void DrawWireframe(IRenderer* renderer);
	void UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera);
	void DrawGizmo(Camera camera);
	void CreateTexture(PEImage& img);
	void LoadTexture(IRenderer* renderer, PEImage& img);
	GLRenderSystem(IPlatform* plat);
private:
	IPlatform* platform;
	void BindTexture(GLRenderer& renderer);
};