#pragma once
#include "IRenderer.h"
#include "Camera.h"
#include "Mesh.h"
#include <entt/entt.hpp>
#include "PEImage.h"
#include "lights.h"
#include <map>
class IRenderSystem
{
public:
	// Used when default initializing a renderer when a mesh is added
	virtual IRenderer& CreateDefaultRenderer(entt::registry& reg, entt::entity e) = 0;
	virtual IRenderer& CreateRenderer(entt::registry& reg, entt::entity e, ShaderType type) = 0;
	virtual void Load(IRenderer* renderer) = 0;
	virtual void BindRenderer(IRenderer* renderer) = 0;
	virtual void LoadMesh(IRenderer* renderer, Mesh& mesh/*, ID3D11Device* device*/) = 0;
	virtual void Draw(IRenderer* renderer/*, ID3D11DeviceContext* context*/) = 0;
	virtual void DrawWireframe(IRenderer* renderer/*, ID3D11DeviceContext* context*/) = 0;
	virtual void UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera, DirectionalLight dirLights[MAX_DIR_LIGHTS], PointLight pointLights[MAX_POINT_LIGHTS], SpotLight spotLights[MAX_SPOT_LIGHTS]) = 0;
	virtual void CreateTexture(PEImage& img) = 0;
	virtual void LoadTexture(IRenderer* renderer, std::map<TextureType, const char*> imagePaths) = 0;
	virtual ~IRenderSystem() {};
};
