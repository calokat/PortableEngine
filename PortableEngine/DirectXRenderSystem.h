#ifdef _WIN64
#pragma once
#include "DirectXRenderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "DirectXAPI.h"
#include "IRenderSystem.h"

class DirectXRenderSystem : public IRenderSystem
{
public:
	IRenderer& CreateRenderer(entt::registry& reg, entt::entity& e);
	void Load(IRenderer* renderer, Camera& camera);
	void LoadMesh(IRenderer* renderer, Mesh& mesh);
	void Draw(IRenderer* DirectXRenderer);
	void DrawWireframe(IRenderer* DirectXRenderer);
	void UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera);
	DirectXRenderSystem(ID3D11Device* dev, ID3D11DeviceContext* ctx);
	void LoadTexture(IRenderer* renderer, PEImage& img);
private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
};
//void DrawGizmo(Camera camera);
#endif