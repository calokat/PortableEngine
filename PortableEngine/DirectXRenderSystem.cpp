#include "DirectXRenderSystem.h"

namespace DirectXRenderSystem
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	void Load(DirectXRenderer& renderer, Camera camera, DirectXAPI* dxApi, WindowsPlatform* winPlat)
	{
		renderer.vertexShader = new SimpleVertexShader(dxApi->device.Get(), dxApi->context.Get(), L"VertexShader.cso");
		renderer.pixelShader = new SimplePixelShader(dxApi->device.Get(), dxApi->context.Get(), L"PixelShader.cso");
		renderer.vertexShader->SetMatrix4x4("viewMatrix", camera.view);
		renderer.vertexShader->SetMatrix4x4("projectionMatrix", camera.projection);
		renderer.vertexShader->SetMatrix4x4("worldMatrix", glm::mat4(1.0f));
		renderer.vertexShader->CopyAllBufferData();
	}

	void LoadMesh(DirectXRenderer& renderer, Mesh& mesh, ID3D11Device* device)
	{
		renderer.numVertices = mesh.rawVertices.size();
		renderer.numIndices = mesh.numIndices;
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * renderer.numVertices;       // 3 = number of vertices in the buffer
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initialVertexData;
		initialVertexData.pSysMem = mesh.rawVertices.data();

		device->CreateBuffer(&vbd, &initialVertexData, renderer.vertexBuffer.GetAddressOf());

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(int) * renderer.numIndices;         // 3 = number of indices in the buffer
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initialIndexData;
		initialIndexData.pSysMem = mesh.rawIndices.data();

		device->CreateBuffer(&ibd, &initialIndexData, renderer.indexBuffer.GetAddressOf());
	}

	void Draw(DirectXRenderer& renderer, ID3D11DeviceContext* context)
	{
		context->IASetVertexBuffers(0, 1, renderer.vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(renderer.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		renderer.vertexShader->SetShader();
		renderer.pixelShader->SetShader();
		context->DrawIndexed(
			renderer.numIndices,     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices

	}

	void DrawWireframe(DirectXRenderer& renderer, ID3D11DeviceContext* context)
	{
		glm::vec4 oldRendererColor = renderer.vertexColor;
		renderer.vertexShader->SetFloat4("colorTint", glm::vec4(1, 1, 1, 1));
		renderer.vertexShader->CopyAllBufferData();
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		Draw(renderer, context);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderer.vertexShader->SetFloat4("colorTint", oldRendererColor);
	}

	void UpdateRenderer(DirectXRenderer& renderer, Transform meshTransform, Camera camera)
	{
		renderer.vertexShader->SetMatrix4x4("viewMatrix", camera.view);
		renderer.vertexShader->SetMatrix4x4("projectionMatrix", camera.projection);
		renderer.vertexShader->SetMatrix4x4("worldMatrix", meshTransform.worldMatrix);
		renderer.vertexShader->SetFloat4("colorTint", renderer.vertexColor);
		renderer.vertexShader->CopyAllBufferData();
	}

}