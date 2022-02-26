#include "ShaderHeader.hlsli"

cbuffer ExternalData : register (b0)
{
	float4 colorTint;
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

VertexToPixel main( VertexShaderInput input )
{
    VertexToPixel output;
    matrix wvp = mul(projectionMatrix,  mul(viewMatrix, worldMatrix));
    output.position	= mul(wvp, float4(input.position, 1.0f));
    output.worldPos = mul(worldMatrix, float4(input.position, 1)).xyz;
	output.color = colorTint;
	output.normal = mul((float3x3)worldMatrix, input.normal);
	output.UV = input.UV;
    return output;
}