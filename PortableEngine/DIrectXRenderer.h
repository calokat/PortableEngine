#pragma once
#include <string>
#include "Vertex.h"
#include "SimpleShader.h"
#include <d3d11.h>
#include <wrl/client.h>
class DirectXRenderer
{
public:
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	SimpleVertexShader vertexShader;
	SimplePixelShader pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int numVertices;
	int numIndices;
};