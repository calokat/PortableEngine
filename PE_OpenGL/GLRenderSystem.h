#pragma once
#include "GLRenderer.h"
#include "Mesh.h"
#include "GL/glew.h"
#include "Camera.h"
#include "IRenderSystem.h"
#include <map>

// source: https://github.com/IGME-RIT/Basic-OpenGL-with-GLFW-Assimp/blob/master/OpenGLObjectLoading/mesh.cpp
#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \

class GLRenderSystem : public IRenderSystem
{
public:
	IRenderer& CreateRenderer(entt::registry& reg, entt::entity& e, ShaderType type);
	void Load(IRenderer* renderer, Camera& camera);
	void BindRenderer(IRenderer* renderer);
	void LoadMesh(IRenderer* renderer, Mesh& mesh);
	void Draw(IRenderer* renderer);
	void DrawWireframe(IRenderer* renderer);
	void UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera, DirectionalLight dirLight, PointLight pointLights[MAX_POINT_LIGHTS]);
	void DrawGizmo(Camera camera);
	void CreateTexture(PEImage& img);
	void LoadTexture(IRenderer* renderer, std::string imagePath);
	virtual void LoadTexture(PEImage& img);
	GLRenderSystem(IPlatform* plat);
private:
	IPlatform* platform;
	void BindTexture(GLRenderer& renderer);
#ifdef __EMSCRIPTEN__
	std::map<ShaderType, const char*> typeToVertexPath = { {ShaderType::Unlit_Color, "../../Shaders/GLSL-ES/vertex-unlit-color.glsl"}, {ShaderType::Unlit_Textured, "../../Shaders/GLSL-ES/vertex.glsl"} };
	std::map<ShaderType, const char*> typeToPixelPath = { {ShaderType::Unlit_Color, "../../Shaders/GLSL-ES/fragment-unlit-color.glsl" }, { ShaderType::Unlit_Textured, "../../Shaders/GLSL-ES/fragment.glsl" } };
#else
	std::map<ShaderType, const char*> typeToVertexPath = { {ShaderType::Unlit_Color, "../../Shaders/GLSL/vertex-unlit-color.glsl"}, {ShaderType::Unlit_Textured, "../../Shaders/GLSL/vertex.glsl"}, {ShaderType::Lit_Color, "../../Shaders/GLSL/vertex-lit-color.glsl"}, {ShaderType::Lit_Textured, "../../Shaders/GLSL/vertex-lit-textured.glsl"} };
	std::map<ShaderType, const char*> typeToPixelPath = { {ShaderType::Unlit_Color, "../../Shaders/GLSL/fragment-unlit-color.glsl" }, { ShaderType::Unlit_Textured, "../../Shaders/GLSL/fragment.glsl" }, {ShaderType::Lit_Color, "../../Shaders/GLSL/fragment-lit-color.glsl"}, {ShaderType::Lit_Textured, "../../Shaders/GLSL/fragment-lit-textured.glsl"} };
#endif
};