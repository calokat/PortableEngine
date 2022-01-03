#ifdef _WIN64
#include "DirectXRenderSystem.h"
#include "DirectX11ImageGraphicsData.h"
#include "ImageSystem.h"
#include "lights.h"

IRenderer& DirectXRenderSystem::CreateRenderer(entt::registry& reg, entt::entity& e, ShaderType type)
{
	DirectXRenderer& rendererRef = reg.emplace<DirectXRenderer>(e);
	rendererRef.shaderProgram.shaderType = type;
	switch (rendererRef.shaderProgram.shaderType)
	{
		case ShaderType::Unlit_Color:
		case ShaderType::Unlit_Textured:
		case ShaderType::Lit_Color:
		case ShaderType::Lit_Textured:
			rendererRef.shaderProgram.vertexConstBuffer.constantBufferMap = {
				{"colorTint", { 16 } },
				{"world", { 64 } },
				{"view", { 64 } },
				{"projection", { 64 } }
			};
			break;
	}

	switch (rendererRef.shaderProgram.shaderType)
	{
		case ShaderType::Lit_Color:
			rendererRef.shaderProgram.pixelConstBuffer.constantBufferMap = {
				{ "dirLight", { sizeof(DirectionalLight) } },
				{ "pointLight", { sizeof(PointLight) } },
				{ "cameraPos", { sizeof(glm::vec3) } },
				{ "specularIntensity", { sizeof(float) } },
			};
			break;
	}
	return rendererRef;
}

void DirectXRenderSystem::Load(IRenderer* renderer, Camera& camera)
{
	DirectXRenderer* dxRenderer = (DirectXRenderer*)renderer;
	ID3DBlob* shaderBlob = nullptr;
	D3DReadFileToBlob(
		typeToVertexPath[dxRenderer->shaderProgram.shaderType],
		&shaderBlob);

	device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		0,							
		dxRenderer->shaderProgram.vertexShader.GetAddressOf());

	// Create an input layout that describes the vertex format
	// used by the vertex shader we're using
	//  - This is used by the pipeline to know how to interpret the raw data
	//     sitting inside a vertex buffer
	//  - Doing this NOW because it requires a vertex shader's byte code to verify against!
	//  - Luckily, we already have that loaded (the blob above)
	D3D11_INPUT_ELEMENT_DESC inputElements[4] = {};

	// Set up the first element - a position, which is 3 float values
	inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// Most formats are described as color channels; really it just means "Three 32-bit floats"
	inputElements[0].SemanticName = "POSITION";							// This is "POSITION" - needs to match the semantics in our vertex shader input!
	inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// How far into the vertex is this?  Assume it's after the previous element

	// Set up the second element - a color, which is 4 more float values
	inputElements[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;			
	inputElements[1].SemanticName = "NORMAL";
	inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	inputElements[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElements[2].SemanticName = "TEXCOORD";
	inputElements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	inputElements[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElements[3].SemanticName = "TANGENT";
	inputElements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	//Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	// Create the input layout, verifying our description against actual shader code
	device->CreateInputLayout(
		inputElements,					// An array of descriptions
		4,								// How many elements in that array
		shaderBlob->GetBufferPointer(),	// Pointer to the code of a shader that uses this layout
		shaderBlob->GetBufferSize(),	// Size of the shader code that uses this layout
		dxRenderer->shaderProgram.inputLayout.GetAddressOf());	// Address of the resulting ID3D11InputLayout*
	context->IASetInputLayout(dxRenderer->shaderProgram.inputLayout.Get());


	//unsigned int size = sizeof(MatrixConstantBuffer);
	unsigned int size = 0;
	for (auto it = dxRenderer->shaderProgram.vertexConstBuffer.constantBufferMap.begin(); it != dxRenderer->shaderProgram.vertexConstBuffer.constantBufferMap.end(); ++it)
	{
		size += it->second.byteSize;
	}
	size = (size + 15) / 16 * 16;

	D3D11_BUFFER_DESC matrixBufferDesc = {};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = size;
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	device->CreateBuffer(&matrixBufferDesc, 0, dxRenderer->shaderProgram.vertexConstBuffer.constantBuffer.GetAddressOf());

	D3DReadFileToBlob(
		typeToPixelPath[dxRenderer->shaderProgram.shaderType],
		&shaderBlob);

	device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		0,
		dxRenderer->shaderProgram.pixelShader.GetAddressOf());

	shaderBlob->Release();
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT res = device->CreateSamplerState(&samplerDesc, &dxRenderer->shaderProgram.samplerState);

	size = 0;
	for (auto it = dxRenderer->shaderProgram.pixelConstBuffer.constantBufferMap.begin(); it != dxRenderer->shaderProgram.pixelConstBuffer.constantBufferMap.end(); ++it)
	{
		size += it->second.byteSize;
	}
	size = (size + 15) / 16 * 16;

	D3D11_BUFFER_DESC lightBufferDesc = {};
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferData);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	device->CreateBuffer(&lightBufferDesc, 0, dxRenderer->shaderProgram.pixelConstBuffer.constantBuffer.GetAddressOf());

	context->PSSetSamplers(0, 1, &dxRenderer->shaderProgram.samplerState);
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
	vbd.ByteWidth = sizeof(Vertex) * dxRenderer->numVertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = mesh.rawVertices.data();

	device->CreateBuffer(&vbd, &initialVertexData, dxRenderer->shaderProgram.vertexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * dxRenderer->numIndices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = mesh.rawIndices.data();

	device->CreateBuffer(&ibd, &initialIndexData, dxRenderer->shaderProgram.indexBuffer.GetAddressOf());
}

void DirectXRenderSystem::Draw(IRenderer* renderer)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DirectXRenderer* dxRenderer = (DirectXRenderer*)renderer;
	BindTexture(dxRenderer);
	context->IASetVertexBuffers(0, 1, dxRenderer->shaderProgram.vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(dxRenderer->shaderProgram.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->VSSetShader(dxRenderer->shaderProgram.vertexShader.Get(), 0, 0);
	context->PSSetShader(dxRenderer->shaderProgram.pixelShader.Get(), 0, 0);
	context->DrawIndexed(
		dxRenderer->numIndices,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

}

void DirectXRenderSystem::DrawWireframe(IRenderer* renderer)
{
}

void DirectXRenderSystem::UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera)
{
	DirectXRenderer* dxRenderer = (DirectXRenderer*)renderer;

	D3D11_MAPPED_SUBRESOURCE data = {};
	MatrixConstantBuffer cb;
	cb.view = camera.view;
	cb.projection = camera.projection;
	cb.world = meshTransform.worldMatrix;
	cb.colorTint = dxRenderer->vertexColor;

	context->Map(dxRenderer->shaderProgram.vertexConstBuffer.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	//memcpy(data.pData, &cb, sizeof(cb));
	memcpy(data.pData, &cb.colorTint, sizeof(cb.colorTint));
	memcpy((char*)data.pData + sizeof(cb.colorTint), &cb.world, sizeof(cb.world));
	memcpy((char*)data.pData + sizeof(cb.colorTint) + sizeof(cb.world), &cb.view, sizeof(cb.view));
	memcpy((char*)data.pData + sizeof(cb.colorTint) + sizeof(cb.world) + sizeof(cb.view), &cb.projection, sizeof(cb.projection));
	context->Unmap(dxRenderer->shaderProgram.vertexConstBuffer.constantBuffer.Get(), 0);
	context->VSSetConstantBuffers(0, 1, dxRenderer->shaderProgram.vertexConstBuffer.constantBuffer.GetAddressOf());

	if (dxRenderer->shaderProgram.shaderType & ShaderProgramProperties::Lit)
	{
		LightBufferData lightBufferData;
		DirectionalLight light;
		light.AmbientColor = glm::vec4(.1f, .0f, .2f, 1);
		light.DiffuseColor = glm::vec4(0, 0, 0, 1);
		light.Direction = glm::vec4(0, 0, 1, 0);
		lightBufferData.dirLight = light;
		lightBufferData.cameraPos = camera.view[3];
		lightBufferData.specularIntensity = 16;
		lightBufferData.pointLight.AmbientColor = glm::vec4(0, 0, 0, 1);
		lightBufferData.pointLight.DiffuseColor = glm::vec4(.8f, .0f, .2f, 1);
		lightBufferData.pointLight.Position = glm::vec4(0, 2, 0, 1);

		data = {};
		context->Map(dxRenderer->shaderProgram.pixelConstBuffer.constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
		memcpy(data.pData, &lightBufferData, sizeof(LightBufferData));
		context->Unmap(dxRenderer->shaderProgram.pixelConstBuffer.constantBuffer.Get(), 0);
		context->PSSetConstantBuffers(0, 1, dxRenderer->shaderProgram.pixelConstBuffer.constantBuffer.GetAddressOf());
	}

	context->PSSetSamplers(0, 1, &dxRenderer->shaderProgram.samplerState);
}

DirectXRenderSystem::DirectXRenderSystem(ID3D11Device* dev, ID3D11DeviceContext* ctx) : device(dev), context(ctx)
{
}

void DirectXRenderSystem::CreateTexture(PEImage& img)
{
	std::shared_ptr<DirectX11ImageGraphicsData> dx11ImageGraphicsData = std::make_shared<DirectX11ImageGraphicsData>();
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
	if ((dxRenderer->shaderProgram.shaderType & ShaderProgramProperties::Textured) == 0) return;
	dxRenderer->diffuseTexture = PEImage(imagePath);
	ImageSystem::CreateImage(dxRenderer->diffuseTexture);
	CreateTexture(dxRenderer->diffuseTexture);

	std::shared_ptr<DirectX11ImageGraphicsData> dx11ImageGraphicsData = std::dynamic_pointer_cast<DirectX11ImageGraphicsData>(dxRenderer->diffuseTexture.imageGraphicsData);
	context->PSSetShaderResources(0, 1, &dx11ImageGraphicsData->srv);
}

void DirectXRenderSystem::LoadTexture(PEImage& img)
{
	CreateTexture(img);
}

void DirectXRenderSystem::BindTexture(DirectXRenderer* renderer)
{
	if ((renderer->shaderProgram.shaderType & ShaderProgramProperties::Textured) == 0) return;
	std::shared_ptr<DirectX11ImageGraphicsData> dx11ImageGraphicsData = std::dynamic_pointer_cast<DirectX11ImageGraphicsData>(renderer->diffuseTexture.imageGraphicsData);
	context->PSSetShaderResources(0, 1, &dx11ImageGraphicsData->srv);
}

#endif