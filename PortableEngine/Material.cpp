#if 0
#include "Material.h"

Material::Material(DirectX::XMFLOAT4 tint, SimplePixelShader* pixel, SimpleVertexShader* vertex, float specInt, ID3D11ShaderResourceView* srvParam, ID3D11ShaderResourceView* normalSrvParam, ID3D11SamplerState* state)
{
	colorTint = tint;
	pixelShader = pixel;
	vertexShader = vertex;
	specularIntensity = specInt;
	srv = srvParam;
	samplerState = state;
	normalMapResource = normalSrvParam;
}

SimplePixelShader* Material::GetPixelShader()
{
	return pixelShader;
}

SimpleVertexShader* Material::GetVertexShader()
{
	return vertexShader;
}

DirectX::XMFLOAT4 Material::GetColorTint()
{
	return colorTint;
}

void Material::SetColorTint(DirectX::XMFLOAT4 newTint)
{
	colorTint = newTint;
}

float Material::GetSpecularIntensity()
{
	return specularIntensity;
}

ID3D11ShaderResourceView* Material::GetShaderResourceView()
{
	return srv;
}

ID3D11ShaderResourceView* Material::GetNormalMapResourceView()
{
	return normalMapResource;
}

ID3D11SamplerState* Material::GetSamplerState()
{
	return samplerState;
}
#endif