#pragma once
#include "IRenderer.h"
#include "Camera.h"
#include "Mesh.h"
#include <entt.hpp>
#include "PEImage.h"
#include "OpenGLShaderProgram.h"

enum ShaderType
{
	Unlit_Color = ShaderProgramProperties::Colored | ShaderProgramProperties::Fully_3D,
	Unlit_Textured = ShaderProgramProperties::Fully_3D | ShaderProgramProperties::Textured | ShaderProgramProperties::Colored
};

class IRenderSystem
{
public:
	virtual IRenderer& CreateRenderer(entt::registry& reg, entt::entity& e, ShaderType type) = 0;
	virtual void Load(IRenderer* renderer, Camera& camera) = 0;
	virtual void BindRenderer(IRenderer* renderer) = 0;
	virtual void LoadMesh(IRenderer* renderer, Mesh& mesh/*, ID3D11Device* device*/) = 0;
	virtual void Draw(IRenderer* renderer/*, ID3D11DeviceContext* context*/) = 0;
	virtual void DrawWireframe(IRenderer* renderer/*, ID3D11DeviceContext* context*/) = 0;
	virtual void UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera) = 0;
	virtual void CreateTexture(PEImage& img) = 0;
	virtual void LoadTexture(IRenderer* renderer, std::string imagePath) = 0;
	virtual void LoadTexture(PEImage& img) = 0;
	virtual ~IRenderSystem() {};
};
