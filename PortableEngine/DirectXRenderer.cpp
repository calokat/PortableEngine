#ifdef _WIN64
#include "DirectXRenderer.h"

DirectXRenderer::DirectXRenderer()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
}

DirectXRenderer::DirectXRenderer(DirectXRenderer&& other)
{
	*this = std::move(other);
}

DirectXRenderer& DirectXRenderer::operator=(DirectXRenderer&& other) noexcept
{
	this->vertexShaderPath = other.vertexShaderPath;
	this->fragmentShaderPath = other.fragmentShaderPath;
	this->vertexShader = other.vertexShader;
	other.vertexShader = nullptr;
	this->pixelShader = other.pixelShader;
	other.pixelShader = nullptr;
	this->vertexBuffer.Swap(other.vertexBuffer);
	this->indexBuffer.Swap(other.indexBuffer);
	this->numIndices = other.numIndices;
	this->numVertices = other.numVertices;
	return *this;
}

//DirectXRenderer::DirectXRenderer(DirectXAPI* dxApi)
//{
//	this->vertexShader = new SimpleVertexShader(dxApi->device.Get(), dxApi->context.Get(), L"VertexShader.cso");
//	this->pixelShader = new SimplePixelShader(dxApi->device.Get(), dxApi->context.Get(), L"PixelShader.cso");
//}

DirectXRenderer::~DirectXRenderer()
{
	delete vertexShader;
	delete pixelShader;
}
#endif