#if 0
#include "Entity.h"
#include "Vertex.h"
#include "BufferStructs.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace DirectX;
Entity::Entity(Mesh* meshPtr, Material* mat)
{
	mesh = meshPtr;
	material = mat;
	stride = sizeof(Vertex);
	offset = 0;
	colorTint = mat->GetColorTint();
}

Transform* Entity::GetTransform()
{
	return &transform;
}

Mesh* Entity::GetMesh()
{
	return mesh;
}

void Entity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera)
{
	context->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	material->GetVertexShader()->SetShader();
	material->GetPixelShader()->SetShader();
	// map the constant buffers
	//VertexShaderExternalData vsData;
	//
	////vsData.colorTint = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
	//vsData.colorTint = material->GetColorTint();
	////XMStoreFloat4x4(&vsData.worldMatrix, XMMatrixIdentity());
	//vsData.worldMatrix = transform.GetWorldMatrix();
	//vsData.projectionMatrix = camera->GetProjectionMatrix();
	//vsData.viewMatrix = camera->GetViewMatrix();
	///*vsData.projectionMatrix = Identity;
	//vsData.viewMatrix = Identity;*/
	//D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	//context->Map(constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	//memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	//context->Unmap(constBuffer, 0);

	SimpleVertexShader* vs = material->GetVertexShader();
	//   Simplifies next few lines 
	vs->SetFloat4("colorTint", material->GetColorTint());
	vs->SetMatrix4x4("worldMatrix", glm::value_ptr(transform.GetWorldMatrix()));
	vs->SetMatrix4x4("viewMatrix", glm::value_ptr(camera->GetViewMatrix()));
	vs->SetMatrix4x4("projectionMatrix", glm::value_ptr(camera->GetProjectionMatrix()));
	vs->CopyAllBufferData();

	SimplePixelShader* ps = material->GetPixelShader();
	ps->SetFloat3("cameraPos", glm::value_ptr(camera->GetTransform()->GetPosition()));
	ps->SetFloat("specularIntensity", material->GetSpecularIntensity());
	ps->SetSamplerState("samplerOptions", material->GetSamplerState());
	ps->SetShaderResourceView("diffuseTexture", material->GetShaderResourceView());
	bool success = false;
	if (material->GetNormalMapResourceView())
	{
		 success = ps->SetShaderResourceView("normalMap", material->GetNormalMapResourceView());
	}
	ps->CopyAllBufferData();
	//context->VSSetConstantBuffers(
	//	0,	// Which slot (register) to bind the buffer to?
	//	1,	// How many are	we activating ? Can do multiple at once
	//	&constBuffer);

	context->DrawIndexed(
		mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}
#endif