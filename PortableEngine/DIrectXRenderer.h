#ifdef _WIN64
#pragma once
#include <string>
#include "Vertex.h"
#include "SimpleShader.h"
#include <d3d11.h>
#include <wrl/client.h>
#include "DirectXAPI.h"
#include "IRenderer.h"
class DirectXRenderer : public IRenderer
{
public:
	DirectXRenderer();
	DirectXRenderer(DirectXRenderer&& other);
	DirectXRenderer& operator=(DirectXRenderer&& other) noexcept;
	//DirectXRenderer(DirectXAPI* dxApi);
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int numVertices;
	int numIndices;
	glm::vec4 vertexColor = { 0, 0, 0, 1 };
	~DirectXRenderer();
};
#endif