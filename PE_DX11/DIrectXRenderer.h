#ifdef _WIN64
#pragma once
#include <string>
#include "Vertex.h"
#include "SimpleShader.h"
#include <d3d11.h>
#include <wrl/client.h>
#include "DirectXAPI.h"
#include "IRenderer.h"
#include "DirectX11ShaderProgram.h"
class DirectXRenderer : public IRenderer
{
public:
	int numVertices;
	int numIndices;
	glm::vec4 vertexColor = { 1, 1, 1, 1 };
	PEImage diffuseTexture;
	DirectX11ShaderProgram shaderProgram;
};

struct MatrixConstantBuffer
{
	glm::vec4 colorTint;
	glm::mat4 world;
	glm::mat4 view;
	glm::mat4 projection;
};
#endif