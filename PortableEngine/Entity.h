#pragma once
#include "Transform.h"
#include "Mesh.h"
#include <d3d11.h>
#include "Camera.h"
#include "Material.h"
class Entity
{
private:
	Transform transform;
	Mesh* mesh;
	UINT stride;
	UINT offset;
	DirectX::XMFLOAT4 colorTint;
	Material* material;
public:
	Entity(Mesh* meshPtr, Material* mat);
	Transform* GetTransform();
	Mesh* GetMesh();
	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera);
};

