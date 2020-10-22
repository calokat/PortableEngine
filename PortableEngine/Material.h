#if 0
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "SimpleShader.h"
class Material
{
private:
	DirectX::XMFLOAT4 colorTint;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	float specularIntensity;
	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* normalMapResource;
	ID3D11SamplerState* samplerState;
	//ID3D11PixelShader* pixelShader;
	//ID3D11VertexShader* vertexShader;
public:
	Material(DirectX::XMFLOAT4 tint, SimplePixelShader* pixel, SimpleVertexShader* vertex, float specInt, ID3D11ShaderResourceView* srvParam, ID3D11ShaderResourceView* normalSrvParam, ID3D11SamplerState* state);
	SimplePixelShader* GetPixelShader();
	SimpleVertexShader* GetVertexShader();
	DirectX::XMFLOAT4 GetColorTint();
	void SetColorTint(DirectX::XMFLOAT4 newTint);
	float GetSpecularIntensity();
	ID3D11ShaderResourceView* GetShaderResourceView();
	ID3D11ShaderResourceView* GetNormalMapResourceView();
	ID3D11SamplerState* GetSamplerState();

};

#endif
