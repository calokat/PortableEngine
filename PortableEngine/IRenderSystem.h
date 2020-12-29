#pragma once
#include "IRenderer.h"
#include "Camera.h"
#include "Mesh.h"
#include <entt.hpp>
class IRenderSystem
{
public:
	virtual IRenderer& CreateRenderer(entt::registry& reg, entt::entity& e) = 0;
	virtual void Load(IRenderer* renderer, Camera& camera) = 0;
	virtual void LoadMesh(IRenderer* renderer, Mesh& mesh/*, ID3D11Device* device*/) = 0;
	virtual void Draw(IRenderer* renderer/*, ID3D11DeviceContext* context*/) = 0;
	virtual void DrawWireframe(IRenderer* renderer/*, ID3D11DeviceContext* context*/) = 0;
	virtual void UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera) = 0;
	virtual ~IRenderSystem() {};
};
