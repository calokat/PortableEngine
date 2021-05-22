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
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	ID3D11SamplerState* samplerState;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int numVertices;
	int numIndices;
	glm::vec4 vertexColor = { 0, 0, 0, 1 };
	PEImage diffuseTexture;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	~DirectXRenderer();
};

struct MatrixConstantBuffer
{
	glm::vec4 colorTint;
	glm::mat4 view;
	glm::mat4 world;
	glm::mat4 projection;
};
#endif