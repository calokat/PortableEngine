#pragma once
#include "DirectXRenderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "DirectXAPI.h"

namespace DirectXRenderSystem
{
	void Load(DirectXRenderer& renderer, Camera camera, DirectXAPI* dxApi, WindowsPlatform* winPlat);
	void LoadMesh(DirectXRenderer& renderer, Mesh& mesh, ID3D11Device* device);
	void Draw(DirectXRenderer& DirectXRenderer, ID3D11DeviceContext* context);
	void UpdateRenderer(DirectXRenderer& renderer, Transform meshTransform, Camera camera);
}
//void DrawGizmo(Camera camera);