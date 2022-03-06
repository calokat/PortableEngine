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
	float4 Position;
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

cbuffer ExternalData : register (b0)
{
	float4 colorTint;
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}
// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage

// Struct representing the data we're sending down the pipeline
// - Should match our pixel shader's input (hence the name: Vertex to Pixel)
// - At a minimum, we need a piece of data defined tagged as SV_POSITION
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToPixelNormalMap main( VertexShaderInput input )
{
	// Set up output struct
	VertexToPixelNormalMap output;

	// Here we're essentially passing the input position directly through to the next
	// stage (rasterizer), though it needs to be a 4-component vector now.  
	// - To be considered within the bounds of the screen, the X and Y components 
	//   must be between -1 and 1.  
	// - The Z component must be between 0 and 1.  
	// - Each of these components is then automatically divided by the W component, 
	//   which we're leaving at 1.0 for now (this is more useful when dealing with 
	//   a perspective projection matrix, which we'll get to in future assignments).
	
	//output.position = float4(input.position, 1.0f);

	//output.position = mul(worldMatrix, float4(input.position, 1.0f));
	matrix wvp = mul(projectionMatrix,  mul(viewMatrix, worldMatrix));
	output.position	= mul(wvp, float4(input.position, 1.0f));
	output.worldPos = mul(worldMatrix, float4(input.position, 1)).xyz;
	output.tangent = normalize(mul((float3x3)worldMatrix, input.tangent));
	// Pass the color through 
	// - The values will be interpolated per-pixel by the rasterizer
	// - We don't need to alter it here, but we do need to send it to the pixel shader
	
	//output.color = input.color;

	output.color = colorTint;
	output.normal = normalize(mul((float3x3)worldMatrix, input.normal));
	output.UV = input.UV;
	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}