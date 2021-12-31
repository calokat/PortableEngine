#ifdef _WIN64
#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include <map>
#include "IRenderSystem.h"

enum ConstantBufferValueType { Vec1f, Vec2f, Vec3f, Vec4f, Mat4f, Array };

struct ConstantBufferValue
{
	size_t byteSize;
};

struct DirectXConstantBuffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	const char* constantBufferFields[16];
	std::map<const char*, ConstantBufferValue> constantBufferMap;
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