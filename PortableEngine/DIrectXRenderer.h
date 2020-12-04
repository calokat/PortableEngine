#pragma once
#include <string>
#include "Vertex.h"
#include "SimpleShader.h"
#include <d3d11.h>
#include <wrl/client.h>
#include "DirectXAPI.h"
class DirectXRenderer
{
public:
	DirectXRenderer(DirectXRenderer&& other);
	DirectXRenderer& operator=(DirectXRenderer&& other) noexcept;
	DirectXRenderer(DirectXAPI* dxApi);
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int numVertices;
	int numIndices;
	~DirectXRenderer();
};