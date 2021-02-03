#ifdef _WIN64
#include "DirectXRenderSystem.h"
#include "DirectX11ImageGraphicsData.h"
#include "ImageSystem.h"

IRenderer& DirectXRenderSystem::CreateRenderer(entt::registry& reg, entt::entity& e)
{
	IRenderer& rendererRef = reg.emplace<DirectXRenderer>(e);
	return rendererRef;
}

void DirectXRenderSystem::Load(IRenderer* renderer, Camera& camera)
{
	DirectXRenderer* dxRenderer = (DirectXRenderer*)renderer;
	dxRenderer->vertexShader = new SimpleVertexShader(device, context, L"VertexShader.cso");
	dxRenderer->pixelShader = new SimplePixelShader(device, context, L"PixelShader.cso");
	dxRenderer->vertexShader->SetMatrix4x4("viewMatrix", camera.view);
	dxRenderer->vertexShader->SetMatrix4x4("projectionMatrix", camera.projection);
	dxRenderer->vertexShader->SetMatrix4x4("worldMatrix", glm::mat4(1.0f));
	dxRenderer->vertexShader->CopyAllBufferData();

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &dxRenderer->samplerState);
	dxRenderer->pixelShader->SetSamplerState("samplerOptions", dxRenderer->samplerState);
}

// This method implementation is empty because it is not actually needed for DirectX, 
// but IRenderSystem needs it implemented. It is needed for OpenGL, specifically WebGL
void DirectXRenderSystem::BindRenderer(IRenderer* renderer)
{
}

void DirectXRenderSystem::LoadMesh(IRenderer* renderer, Mesh& mesh)
{
	DirectXRenderer* dxRenderer = (DirectXRenderer*)renderer;
	dxRenderer->numVertices = mesh.rawVertices.size();
	dxRenderer->numIndices = mesh.numIndices;
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * dxRenderer->numVertices;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = mesh.rawVertices.data();

	device->CreateBuffer(&vbd, &initialVertexData, dxRenderer->vertexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * dxRenderer->numIndices;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = mesh.rawIndices.data();

	device->CreateBuffer(&ibd, &initialIndexData, dxRenderer->indexBuffer.GetAddressOf());
}

void DirectXRenderSystem::Draw(IRenderer* renderer)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DirectXRenderer* dxRenderer = (DirectXRenderer*)renderer;
	BindTexture(dxRenderer);
	context->IASetVertexBuffers(0, 1, dxRenderer->vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(dxRenderer->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	dxRenderer->vertexShader->SetShader();
	dxRenderer->pixelShader->SetShader();
	context->DrawIndexed(
		dxRenderer->numIndices,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

}

void DirectXRenderSystem::DrawWireframe(IRenderer* renderer)
{
	DirectXRenderer* dxRenderer = (DirectXRenderer*)renderer;
	glm::vec4 oldRendererColor = dxRenderer->vertexColor;
	dxRenderer->vertexShader->SetFloat4("colorTint", glm::vec4(1, 1, 1, 1));
	dxRenderer->vertexShader->CopyAllBufferData();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	Draw(dxRenderer);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dxRenderer->vertexShader->SetFloat4("colorTint", oldRendererColor);
}

void DirectXRenderSystem::UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera)
{
	DirectXRenderer* dxRenderer = (DirectXRenderer*)renderer;
	dxRenderer->vertexShader->SetMatrix4x4("viewMatrix", camera.view);
	dxRenderer->vertexShader->SetMatrix4x4("projectionMatrix", camera.projection);
	dxRenderer->vertexShader->SetMatrix4x4("worldMatrix", meshTransform.worldMatrix);
	dxRenderer->vertexShader->SetFloat4("colorTint", dxRenderer->vertexColor);
	dxRenderer->vertexShader->CopyAllBufferData();
}

DirectXRenderSystem::DirectXRenderSystem(ID3D11Device* dev, ID3D11DeviceContext* ctx) : device(dev), context(ctx)
{
}

void DirectXRenderSystem::CreateTexture(PEImage& img)
{
	DirectX11ImageGraphicsData* dx11ImageGraphicsData = new DirectX11ImageGraphicsData();
	img.imageGraphicsData = dx11ImageGraphicsData;
	// stolen from https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#Example-for-DirectX11-users
	
	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = img.width;
	desc.Height = img.height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	
	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = img.data;
	subResource.SysMemPitch = desc.Width * 3;
	subResource.SysMemSlicePitch = 0;
	device->CreateTexture2D(&desc, &subResource, &pTexture);
	
	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(pTexture, &srvDesc, &dx11ImageGraphicsData->srv);
	pTexture->Release();

	ImageSystem::DestroyImage(img);
}

void DirectXRenderSystem::LoadTexture(IRenderer* renderer, std::string imagePath)
{
	DirectXRenderer* dxRenderer = (DirectXRenderer*)renderer;
	dxRenderer->diffuseTexture = PEImage(imagePath);
	ImageSystem::CreateImage(dxRenderer->diffuseTexture);
	CreateTexture(dxRenderer->diffuseTexture);

	DirectX11ImageGraphicsData* dx11ImageGraphicsData = (DirectX11ImageGraphicsData*)dxRenderer->diffuseTexture.imageGraphicsData;
	//DirectXRenderer* dx11Renderer = (DirectXRenderer*)renderer;

	dxRenderer->pixelShader->SetShaderResourceView("diffuseTexture", dx11ImageGraphicsData->srv);
	dxRenderer->pixelShader->CopyAllBufferData();
}

void DirectXRenderSystem::LoadTexture(PEImage& img)
{
	CreateTexture(img);
}

void DirectXRenderSystem::BindTexture(DirectXRenderer* renderer)
{
	DirectX11ImageGraphicsData* dx11ImageGraphicsData = (DirectX11ImageGraphicsData*)renderer->diffuseTexture.imageGraphicsData;
	//DirectXRenderer* dx11Renderer = (DirectXRenderer*)renderer;

	renderer->pixelShader->SetShaderResourceView("diffuseTexture", dx11ImageGraphicsData->srv);
	renderer->pixelShader->CopyAllBufferData();

}

#endif