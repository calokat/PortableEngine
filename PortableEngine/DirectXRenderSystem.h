#pragma once
#include "DirectXRenderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "IPlatform.h"

void Load(DirectXRenderer& renderer, Camera camera, ID3D11Device* device, ID3D11DeviceContext* context, IPlatform* plat);
void LoadMesh(DirectXRenderer& renderer, Mesh& mesh, ID3D11Device* device);
void Draw(DirectXRenderer& DirectXRenderer, ID3D11DeviceContext* context);
void UpdateRenderer(DirectXRenderer& renderer, Transform meshTransform, Camera camera);
//void DrawGizmo(Camera camera);