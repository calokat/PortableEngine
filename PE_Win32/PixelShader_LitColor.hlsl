#include "ShaderHeader.hlsli"

cbuffer LightData: register(b0)
{
	DirectionalLight dirLight;
	PointLight pointLight;
	float3 cameraPos;
	float specularIntensity;
};

// Texture2D diffuseTexture : register(t0);
// SamplerState samplerOptions : register(s0);

float PhongPoint(PointLight light, VertexToPixel input)
{
	float3 R = normalize(reflect(input.worldPos - light.Position, input.normal));
	float3 V = normalize(cameraPos - input.worldPos);
	float RdotV = saturate(dot(R, V));
	float spec = pow(RdotV, 256);
	spec = spec * specularIntensity;
	return spec;
}

float3 CalculatePointLight(PointLight light, VertexToPixel input)
{
	input.normal = normalize(input.normal);
	float3 normalizedNegatedLightDir = normalize(light.Position - input.worldPos);
	float lightAmount = saturate(dot(normalizedNegatedLightDir, input.normal));
	float3 finalColor = lightAmount * float4(light.DiffuseColor.xyz, 1) * input.color + float4(light.AmbientColor.xyz, 1) + PhongPoint(light, input);
	return finalColor;
}

float3 CalculateDirLight(DirectionalLight light, VertexToPixel input)
{
	input.normal = normalize(input.normal);
	float3 normalizedNegatedLightDir = normalize(-light.Direction);
	float lightAmount = saturate(dot(normalizedNegatedLightDir, input.normal));
	float3 finalColor = lightAmount * light.DiffuseColor * input.color + light.AmbientColor * input.color;
	return finalColor;
}


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	//input.normal = normalize(input.normal);
	//float3 normalizedNegatedLightDir = normalize(-light.Direction);
	//float lightAmount = saturate(dot(normalizedNegatedLightDir, input.normal));
	//float3 finalColor = lightAmount * light.DiffuseColor * input.color + light.AmbientColor * input.color;
	float3 finalColor = CalculateDirLight(dirLight, input) + CalculatePointLight(pointLight, input);
	//loop for (int i = 0; i < 8; i = i++)
	//{
	//	finalColor = finalColor + CalculatePointLight(pointLights[i], input);
	//} 
	// float3 surfaceColor = diffuseTexture.Sample(samplerOptions, input.UV).rgb;

	// finalColor = finalColor * surfaceColor * input.color;
	finalColor = finalColor * input.color;
	//return float4(input.normal, 1);
	//return float4(finalColor, 1);
	//return float4(input.color);
	return float4(finalColor, 1);
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	//return input.color;
}