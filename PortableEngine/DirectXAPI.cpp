#include "DirectXAPI.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Vertex.h"
#include <iostream>
// ********** REQUIRED WITHOUT SIMPLE SHADER **********
// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")

// ********** REQUIRED WITHOUT SIMPLE SHADER **********

struct VertShaderExternalData
{
	glm::mat4 world;
	glm::mat4 view;
	glm::mat4 proj;
};


int DirectXAPI::Init()
{
	unsigned int deviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// If we're in debug mode in visual studio, we also
	// want to make a "Debug DirectX Device" to see some
	// errors and warnings in Visual Studio's output window
	// when things go wrong!
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create a description of how our swap
	// chain should work
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = window->width;
	swapDesc.BufferDesc.Height = window->height;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.Flags = 0;
	swapDesc.OutputWindow = (HWND)window->windowHandle;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Windowed = true;

	// Result variable for below function calls
	HRESULT hr = S_OK;

	// Attempt to initialize DirectX
	hr = D3D11CreateDeviceAndSwapChain(
		0,							// Video adapter (physical GPU) to use, or null for default
		D3D_DRIVER_TYPE_HARDWARE,	// We want to use the hardware (GPU)
		0,							// Used when doing software rendering
		deviceFlags,				// Any special options
		0,							// Optional array of possible verisons we want as fallbacks
		0,							// The number of fallbacks in the above param
		D3D11_SDK_VERSION,			// Current version of the SDK
		&swapDesc,					// Address of swap chain options
		swapChain.GetAddressOf(),	// Pointer to our Swap Chain pointer
		device.GetAddressOf(),		// Pointer to our Device pointer
		&dxFeatureLevel,			// This will hold the actual feature level the app will use
		context.GetAddressOf());	// Pointer to our Device Context pointer
	if (FAILED(hr)) return hr;

	// The above function created the back buffer render target
	// for us, but we need a reference to it
	ID3D11Texture2D* backBufferTexture = 0;
	swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&backBufferTexture);

	// Now that we have the texture, create a render target view
	// for the back buffer so we can render into it.  Then release
	// our local reference to the texture, since we have the view.
	if (backBufferTexture != 0)
	{
		device->CreateRenderTargetView(
			backBufferTexture,
			0,
			backBufferRTV.GetAddressOf());
		backBufferTexture->Release();
	}

	// Set up the description of the texture to use for the depth buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = window->width;
	depthStencilDesc.Height = window->height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	// Create the depth buffer and its view, then 
	// release our reference to the texture
	ID3D11Texture2D* depthBufferTexture = 0;
	device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	if (depthBufferTexture != 0)
	{
		device->CreateDepthStencilView(
			depthBufferTexture,
			0,
			depthStencilView.GetAddressOf());
		depthBufferTexture->Release();
	}

	// Bind the views to the pipeline, so rendering properly 
	// uses their underlying textures
	context->OMSetRenderTargets(
		1,
		backBufferRTV.GetAddressOf(),
		depthStencilView.Get());

	// Lastly, set up a viewport so we render into
	// to correct portion of the window
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)window->width;
	viewport.Height = (float)window->height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(inputLayout);
	// Return the "everything is ok" HRESULT value
	return S_OK;

}

DirectXAPI::DirectXAPI(GameWindow* win) : window(win)
{
	// Initialize fields
	vertexBuffer = 0;
	indexBuffer = 0;
	vs = 0;
	ps = 0;
	vsConstantBuffer = 0;
	inputLayout = 0;
	//testHelix = 0;
}

void DirectXAPI::ClearScreen()
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

void DirectXAPI::Draw()
{
	VertShaderExternalData vsData;

	vsData.world = worldMatrix;
	vsData.view = viewMatrix;
	vsData.proj = projectionMatrix;
	//vsData.colorTint = glm::vec4(1, 0, 0, 1);

	// Copy this data to the constant buffer we intend to use
	context->UpdateSubresource(
		vsConstantBuffer,	// Destination buffer
		0,					// Which sub-resource inside that buffer (the first one)
		0,					// Are we updating a box (a block) of memory? Nope
		&vsData,			// Where are we pulling the data from?  (Size is assumed to match the destination buffer!)
		0,					// Is this 2-dimensional data?  (Like a texture)  Nope
		0);					// Is this 3-dimensional data?  (Like a volume texture)  Nope

			// Activate this constant buffer, ensuring it is in the correct slot (register)
		//  - This should match what the shader expects!
	context->VSSetConstantBuffers(
		0,						// Which slot (register) to bind the buffer to?
		1,						// How many are we activating?  Can do more than one at a time, if we need
		&vsConstantBuffer);		// An array of constant buffers (or the address of one, same thing in C++)

	context->VSSetShader(vs, 0, 0);
	context->PSSetShader(ps, 0, 0);

	// Set buffers in the input assembler
//  - Do this ONCE PER OBJECT you're drawing, since each object might
//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		3,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices


	//swapChain->Present(0, 0);

	//// Due to the usage of a more sophisticated swap chain,
	//// the render target must be re-bound after every call to Present()
	//context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}

DirectXAPI::~DirectXAPI()
{
	// Release any (and all!) DirectX objects
// we've made in the Game class
	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }

	// ********** REQUIRED WITHOUT SIMPLE SHADER **********

	// Release shaders, buffers and other resources
	if (vs) { vs->Release(); }
	if (ps) { ps->Release(); }
	if (vsConstantBuffer) { vsConstantBuffer->Release(); }
	if (inputLayout) { inputLayout->Release(); }

	//if (testHelix) delete testHelix;
	// ********** REQUIRED WITHOUT SIMPLE SHADER **********

}

void DirectXAPI::LoadShaders()
{
	// ********** REQUIRED WITHOUT SIMPLE SHADER **********
	// ********** REQUIRED WITHOUT SIMPLE SHADER **********

	// Blob for reading raw data
	// - This is a simplified way of handling raw data
	ID3DBlob* shaderBlob;

	// Read our compiled vertex shader code into a blob
	// - Essentially just "open the file and plop its contents here"
	D3DReadFileToBlob(L"VertexShader.cso", &shaderBlob);

	// Create a vertex shader from the information
	// we read into the blob above
	// - A blob can give a pointer to its contents, and knows its own size
	device->CreateVertexShader(
		shaderBlob->GetBufferPointer(), // Get a pointer to the blob's contents
		shaderBlob->GetBufferSize(),	// How big is that data?
		0,								// No classes in this shader
		&vs);							// The address of the ID3D11VertexShader*

	// Create an input layout that describes the vertex format
	// used by the vertex shader we're using
	//  - This is used by the pipeline to know how to interpret the raw data
	//     sitting inside a vertex buffer
	//  - Doing this NOW because it requires a vertex shader's byte code to verify against!
	//  - Luckily, we already have that loaded (the blob above)
	D3D11_INPUT_ELEMENT_DESC inputElements[5] = {};

	// Set up the first element - a position, which is 3 float values
	inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // Most formats are described as color channels, really it just means "Three 32-bit floats"
	inputElements[0].SemanticName = "POSITION";				// This is "POSITTION" - needs to match the semantics in our vertex shader input!
	inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // How far into the vertex is this?  Assume it's after the previous element

	inputElements[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // Most formats are described as color channels, really it just means "Three 32-bit floats"
	inputElements[1].SemanticName = "NORMAL";				// This is "POSITTION" - needs to match the semantics in our vertex shader input!
	inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // How far into the vertex is this?  Assume it's after the previous element

	inputElements[2].Format = DXGI_FORMAT_R32G32_FLOAT;  // Most formats are described as color channels, really it just means "Three 32-bit floats"
	inputElements[2].SemanticName = "TEXCOORD";				// This is "POSITTION" - needs to match the semantics in our vertex shader input!
	inputElements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // How far into the vertex is this?  Assume it's after the previous element

	inputElements[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // Most formats are described as color channels, really it just means "Three 32-bit floats"
	inputElements[3].SemanticName = "TANGENT";				// This is "POSITTION" - needs to match the semantics in our vertex shader input!
	inputElements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // How far into the vertex is this?  Assume it's after the previous element

	// Set up the second element - a color, which is 4 more float values
	inputElements[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	// 4x 32-bit floats
	inputElements[4].SemanticName = "COLOR";					// Match our vertex shader input!
	inputElements[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// After the previous element

	// Create the input layout, verifying our description against actual shader code
	device->CreateInputLayout(inputElements, 5, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);

	// Read and create the pixel shader
	//  - Reusing the same blob here, since we're done with the vert shader code
	D3DReadFileToBlob(L"PixelShader.cso", &shaderBlob);
	device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &ps);


	// Set up a constant buffer for the vertex shader variables
	//  - We need one of these for EACH constant buffer defined in EACH shader
	//  - (You can technically reuse them between shaders if they're the same size and contain the same data)
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// Intend to bind this as a constant buffer
	cbd.ByteWidth = sizeof(glm::mat4) * 3;		// Gotta match the total size of the cbuffer defined in the shader!!!
	cbd.Usage = D3D11_USAGE_DEFAULT;			// Standard usage
	cbd.CPUAccessFlags = 0;						// Not reading from the CPU-side
	cbd.MiscFlags = 0;							// Nothing 
	cbd.StructureByteStride = 0;				// Nothing

	device->CreateBuffer(&cbd, 0, &vsConstantBuffer);

	// ********** REQUIRED WITHOUT SIMPLE SHADER **********

}

void DirectXAPI::CreateMatrices()
{
	// Set up world matrix
// - In an actual game, each object will need one of these and they should
//    update when/if the object moves (every frame)
// - You'll notice a "transpose" happening below, which is redundant for
//    an identity matrix.  This is just to show that HLSL expects a different
//    matrix (column major vs row major) than the DirectX Math library
	worldMatrix = glm::mat4(1.0f);
	//XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	glm::vec3 pos = glm::vec3(0, 0, -5);
	glm::vec3 dir = glm::vec3(0, 0, 1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	viewMatrix = glm::transpose(glm::lookAtLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up));     // "Up" direction in 3D space (prevents roll)
	//XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	projectionMatrix = glm::transpose(glm::perspectiveFovLH(
		45.f,		// Field of View Angle
		(float)window->width , 
		(float)window->height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f));					// Far clip plane distance
	//XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

void DirectXAPI::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 green = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 blue = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex vertices[] =
	{
		{ glm::vec3(+0.0f, +1.0f, +0.0f), glm::vec3(0), glm::vec2(0), glm::vec3(0)},
		{ glm::vec3(+1.5f, -1.0f, +0.0f), glm::vec3(0), glm::vec2(0), glm::vec3(0) },
		{ glm::vec3(-1.5f, -1.0f, +0.0f), glm::vec3(0), glm::vec2(0), glm::vec3(0) },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	int indices[] = { 0, 1, 2 };


	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 3;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);



	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * 3;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);
}

void DirectXAPI::_SwapBuffers()
{
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}