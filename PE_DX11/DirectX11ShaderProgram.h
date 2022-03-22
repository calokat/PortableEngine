#ifdef _WIN64
#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include <map>
#include "IRenderSystem.h"

struct DirectXConstantBuffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	size_t byteSize;
};

struct DirectX11ShaderProgram
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	DirectXConstantBuffer vertexConstBuffer;
	DirectXConstantBuffer pixelConstBuffer;
	ShaderType shaderType;
};
#endif