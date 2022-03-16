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
	IRenderer& CreateDefaultRenderer(entt::registry& reg, entt::entity e);
	IRenderer& CreateRenderer(entt::registry& reg, entt::entity e, ShaderType type);
	void Load(IRenderer* renderer);
	void BindRenderer(IRenderer* renderer);
	void LoadMesh(IRenderer* renderer, Mesh& mesh);
	void Draw(IRenderer* renderer);
	void DrawWireframe(IRenderer* renderer);
	void UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera, DirectionalLight dirLights[MAX_DIR_LIGHTS], PointLight pointLights[MAX_POINT_LIGHTS], SpotLight spotLights[MAX_SPOT_LIGHTS]);
	void DrawGizmo(Camera camera);
	void CreateTexture(PEImage& img);
	void LoadTexture(IRenderer* renderer, std::map<TextureType, const char*> imagePaths);
	GLRenderSystem(IPlatform* plat);
private:
	IPlatform* platform;
	void BindTexture(GLRenderer& renderer);
	void LoadTexture(PEImage& texture, PE::String imagePath);
#ifdef __EMSCRIPTEN__
	std::map<ShaderType, const char*> typeToVertexPath = { {ShaderType::Unlit_Color, "../../Shaders/GLSL-ES/vertex-unlit-color.glsl"}, {ShaderType::Unlit_Textured, "../../Shaders/GLSL-ES/vertex.glsl"}, {ShaderType::Lit_Color, "../../Shaders/GLSL-ES/vertex-lit-color.glsl"}, {ShaderType::Lit_Textured, "../../Shaders/GLSL-ES/vertex-lit-textured.glsl"}, {ShaderType::Lit_Textured_Normal, "../../Shaders/GLSL-ES/vertex-lit-textured-normal.glsl"} };
	std::map<ShaderType, const char*> typeToPixelPath = { {ShaderType::Unlit_Color, "../../Shaders/GLSL-ES/fragment-unlit-color.glsl" }, { ShaderType::Unlit_Textured, "../../Shaders/GLSL-ES/fragment.glsl" }, {ShaderType::Lit_Color, "../../Shaders/GLSL-ES/fragment-lit-color.glsl"}, {ShaderType::Lit_Textured, "../../Shaders/GLSL-ES/fragment-lit-textured.glsl"}, {ShaderType::Lit_Textured_Normal, "../../Shaders/GLSL-ES/fragment-lit-textured-normal.glsl"} };
#else
	std::map<ShaderType, const char*> typeToVertexPath = { {ShaderType::Unlit_Color, "../../Shaders/GLSL/vertex-unlit-color.glsl"}, {ShaderType::Unlit_Textured, "../../Shaders/GLSL/vertex.glsl"}, {ShaderType::Lit_Color, "../../Shaders/GLSL/vertex-lit-color.glsl"}, {ShaderType::Lit_Textured, "../../Shaders/GLSL/vertex-lit-textured.glsl"}, {ShaderType::Lit_Textured_Normal, "../../Shaders/GLSL/vertex-lit-textured-normal.glsl"} };
	std::map<ShaderType, const char*> typeToPixelPath = { {ShaderType::Unlit_Color, "../../Shaders/GLSL/fragment-unlit-color.glsl" }, { ShaderType::Unlit_Textured, "../../Shaders/GLSL/fragment.glsl" }, {ShaderType::Lit_Color, "../../Shaders/GLSL/fragment-lit-color.glsl"}, {ShaderType::Lit_Textured, "../../Shaders/GLSL/fragment-lit-textured.glsl"}, {ShaderType::Lit_Textured_Normal, "../../Shaders/GLSL/fragment-lit-textured-normal.glsl"}};
#endif
};