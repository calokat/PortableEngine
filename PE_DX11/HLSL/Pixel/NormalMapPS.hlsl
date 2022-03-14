// structs
struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float4 Direction;
};
struct PointLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Position;
	float Intensity;
};
struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;
	float2 UV			: TEXCOORD;
	float3 tangent		: TANGENT;
};
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
	float2 UV			: TEXCOORD;
};

struct VertexToPixelNormalMap
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
	float2 UV			: TEXCOORD;
	float3 tangent		: TANGENT;
};

cbuffer LightData: register(b0)
{
	DirectionalLight dirLight;
	PointLight pointLight;
	float3 cameraPos;
	float specularIntensity;
};

Texture2D diffuseTexture : register(t0);
Texture2D normalMap		 : register(t1);
SamplerState samplerOptions : register(s0);

float PhongPoint(PointLight light, VertexToPixelNormalMap input)
{
	float3 R = normalize(reflect(light.Position - input.worldPos, input.normal));
	float3 V = normalize(cameraPos - input.worldPos);
	float RdotV = saturate(dot(R, V));
	float spec = pow(RdotV, 256);
	spec = spec * specularIntensity;
	spec *= any(light.DiffuseColor);
	return spec;
}

float3 CalculatePointLight(PointLight light, VertexToPixelNormalMap input)
{
	input.normal = normalize(input.normal);
	float3 normalizedNegatedLightDir = normalize(light.Position - input.worldPos);
	float lightAmount = saturate(dot(normalizedNegatedLightDir, input.normal));
	lightAmount = lightAmount * light.Intensity / distance(light.Position, input.worldPos);
	float3 finalColor = lightAmount * light.DiffuseColor * input.color + light.AmbientColor + PhongPoint(light, input);
	return finalColor;
}

float3 CalculateDirLight(DirectionalLight light, VertexToPixelNormalMap input)
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
float4 main(VertexToPixelNormalMap input) : SV_TARGET
{
	//input.normal = normalize(input.normal);
	//float3 normalizedNegatedLightDir = normalize(-light.Direction);
	//float lightAmount = saturate(dot(normalizedNegatedLightDir, input.normal));
	//float3 finalColor = lightAmount * light.DiffuseColor * input.color + light.AmbientColor * input.color;
	float3 unpackedNormal = normalMap.Sample(samplerOptions, input.UV).rgb * 2 - 1;
	// Simplify this code as you see fit
	float3 N = normalize(input.normal); // Must be normalized
	float3 T = normalize(input.tangent); // Must be normalized
	T = normalize(T - N * dot(T, N)); // Gram-Schmidt orthogonalization
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);

	// Assumes that	input.normal is used later in the shader
	input.normal = mul(unpackedNormal, TBN); // Note multiplication order
	float3 surfaceColor = diffuseTexture.Sample(samplerOptions, input.UV).rgb;
	float3 finalColor = surfaceColor * input.color * CalculateDirLight(dirLight, input) * CalculatePointLight(pointLight, input);
	//return float4(input.normal, 1);
return float4(finalColor, 1);
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	//return input.color;
}