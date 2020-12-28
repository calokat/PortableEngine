#ifdef _WIN64
#include "DirectXRenderSystem.h"


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

#endif