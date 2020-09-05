#include "Game.h"
#include "Vertex.h"
// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include "BufferStructs.h"
#include "WICTextureLoader.h"
// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------

Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
	blue = nullptr;
	camera = nullptr;
	sphere = nullptr;
	green = nullptr;
	helix = nullptr;
	light = {};
	light2 = {};
	light3 = {};
	light4 = {};
	pixelShader = nullptr;
	red = nullptr;
	white = nullptr;
	sampler = nullptr;
	srv = nullptr;
	srv2 = nullptr;
	vertexShader = nullptr;
	normalVertexShader = nullptr;
	normalPixelShader = nullptr;
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object
	//delete triangle;
	//delete square;
	//delete trapezoid;
	//delete cube;
	delete sphere;
	delete helix;
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		delete *it;
	}
	delete red;
	delete green;
	delete blue;
	delete white;
	delete camera;
	delete vertexShader;
	delete pixelShader;
	delete normalVertexShader;
	delete normalPixelShader;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	LoadTextures();
	red = new Material(XMFLOAT4(1, 0, 0, 1), pixelShader, vertexShader, 1, srv, nullptr, sampler);
	blue = new Material(XMFLOAT4(0, 0, 1, 1), normalPixelShader, normalVertexShader, 1, srv, normalSrv, sampler);
	green = new Material(XMFLOAT4(0, 1, 0, 1), normalPixelShader, normalVertexShader, 1, srv2, normalSrv2, sampler);
	white = new Material(XMFLOAT4(1, 1, 1, 1), normalPixelShader, normalVertexShader, 1, srv, normalSrv, sampler);
	CreateBasicGeometry();
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Get size as the next multiple of 16 
	unsigned int size =	sizeof(VertexShaderExternalData);
	size = (size + 15) / 16 * 16;

	// Describe the constant buffer
	D3D11_BUFFER_DESC cbDesc = {}; // Sets struct to all zeros
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth = size; // Must be a multiple of 16
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;


	camera = new Camera(glm::vec3(0, 0, -.5f), 1280.0f / 720);

	// For some reason, calling UpdateProjectionMatrix() works here, but not in the Camera constructor
	camera->UpdateProjectionMatrix((float)this->width / this->height);

	light = { XMFLOAT3(.1f, .1f, .1f), 0.0f, XMFLOAT3(1, 1, 1), 0.0f, XMFLOAT3(0, 1, 0) };
	light2 = { XMFLOAT3(.1f, .1f, .1f), 0.0f, XMFLOAT3(.2f, .5f, .9f), 0.0f, XMFLOAT3(1, 0, 0) };
	light3 = { XMFLOAT3(.1f, .1f, .1f), 0.0f, XMFLOAT3(.0f, .9f, .0f), 0.0f, XMFLOAT3(0, 0, -1) };
	light4 = { XMFLOAT3(.1f, .1f, .1f), 0.0f, XMFLOAT3(1, 1, 1), 0.0f, XMFLOAT3(0, -1, 0) };
	pixelShader->SetData("light", &light, sizeof(PointLight));
	pixelShader->SetData("light2", &light2, sizeof(PointLight));
	pixelShader->SetData("light3", &light3, sizeof(PointLight));
	pixelShader->SetData("dirLight", &light4, sizeof(DirectionalLight));
	pixelShader->CopyAllBufferData();

	normalPixelShader->SetData("light", &light, sizeof(PointLight));
	normalPixelShader->SetData("light2", &light2, sizeof(PointLight));
	normalPixelShader->SetData("light3", &light3, sizeof(PointLight));
	normalPixelShader->SetData("dirLight", &light4, sizeof(DirectionalLight));
	normalPixelShader->CopyAllBufferData();

	//camera->GetTransform().SetRotation(XMFLOAT3(3.14f, 0, 0));
	//camera->GetTransform().MoveRelative(XMFLOAT3(0, 0, -3));
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"VertexShader.cso").c_str());
	pixelShader = new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"PixelShader.cso").c_str());
	normalVertexShader = new SimpleVertexShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"NormalMapVS.cso").c_str());
	normalPixelShader = new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"NormalMapPS.cso").c_str());
}

void Game::LoadTextures()
{
	HRESULT result = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Images/rock.png").c_str(), nullptr, &srv);
	HRESULT result2 = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Images/cushion.png").c_str(), nullptr, &srv2);
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Images/Normal/rock_normals.png").c_str(), nullptr, &normalSrv);
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Images/Normal/cushion_normals.png").c_str(), nullptr, &normalSrv2);
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT res3 = device->CreateSamplerState(&samplerDesc, &sampler);
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 orange = XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f);
	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	// - Note: Since we don't have a camera or really any concept of
	//    a "3d world" yet, we're simply describing positions within the
	//    bounds of how the rasterizer sees our screen: [-1 to +1] on X and Y
	// - This means (0,0) is at the very center of the screen.
	// - These are known as "Normalized Device Coordinates" or "Homogeneous 
	//    Screen Coords", which are ways to describe a position without
	//    knowing the exact size (in pixels) of the image/window/etc.  
	// - Long story short: Resizing the window also resizes the triangle,
	//    since we're describing the triangle in terms of the window itself
	XMFLOAT3 normal = XMFLOAT3(0, 0, -1);
	XMFLOAT2 uv = XMFLOAT2(0, 0);
	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.15f, +0.0f), normal, uv},
		{ XMFLOAT3(+0.15f, -0.15f, +0.0f), normal, uv },
		{ XMFLOAT3(-0.15f, -0.15f, +0.0f), normal, uv },
	};

	Vertex squareVertices[] = {
		{XMFLOAT3(0.2f, 0.2f, 0.0f), normal, uv},
		{XMFLOAT3(0.2f, -0.2f, 0), normal, uv},
		{XMFLOAT3(0.4f, 0.2f, 0), normal, uv},
		{XMFLOAT3(0.4f, -0.2f, 0), normal, uv},
	};

	Vertex trapezoidVertices[] = {
	{XMFLOAT3(0.2f, 0.4f, 0.0f), normal, uv},
	{XMFLOAT3(-0.6f, -0.0f, 0), normal, uv},
	{XMFLOAT3(0.4f, 0.4f, 0), normal, uv},
	{XMFLOAT3(1.0f, -0.0f, 0), normal, uv},
	};

	Vertex cubeVerts[] = {
		{XMFLOAT3(3, 3, 3), normal, uv},
		{XMFLOAT3(-3, 3, 3), normal, uv},
		{XMFLOAT3(-3, -3, 3), normal, uv},
		{XMFLOAT3(3, -3, 3), normal, uv},
		{XMFLOAT3(3, 3, -3), normal, uv},
		{XMFLOAT3(-3, 3, -3), normal, uv},
		{XMFLOAT3(-3, -3, -3), normal, uv},
		{XMFLOAT3(3, -3, -3), normal, uv}
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	unsigned int indices[] = { 0, 1, 2 };

	unsigned int squareIndices[] = { 0, 2, 3, 3, 1, 0 };

	unsigned int cubeIndices[] = { 0, 2, 1, 2, 3, 0, 6, 5, 4, 4, 6, 7};

	//triangle = new Mesh(vertices, 3, indices, 3, device);

	//square = new Mesh(squareVertices, 4, squareIndices, 6, device);

	//trapezoid = new Mesh(trapezoidVertices, 4, squareIndices, 6, device);

	//cube = new Mesh(cubeVerts, 8, cubeIndices, 12, device);

	sphere = new Mesh(GetFullPathTo("../../Assets/Models/sphere.obj").c_str(), device);
	helix = new Mesh(GetFullPathTo("../../Assets/Models/helix.obj").c_str(), device);

	Entity* testEntity = new Entity(helix, this->red);
	//testEntity->GetTransform()->MoveAbsolute(XMFLOAT3(0, 0, 1));
	entities.push_back(testEntity);
	Entity* entity2 = new Entity(sphere, this->white);
	//entity2->GetTransform()->MoveAbsolute(XMFLOAT3(-1, 0, -1));
	entities.push_back(entity2);
	Entity* squareEnt = new Entity(sphere, this->white);
	//squareEnt->GetTransform()->MoveAbsolute(XMFLOAT3(0, 0, 2));
	entities.push_back(squareEnt);
	Entity* triangleEnt = new Entity(sphere, this->white);
	//triangleEnt->GetTransform()->MoveAbsolute(XMFLOAT3(0, 0, 1.5f));
	entities.push_back(triangleEnt);
	Entity* triangleEnt2 = new Entity(sphere, this->green);
	//triangleEnt2->GetTransform()->MoveAbsolute(XMFLOAT3(0, 0, -1.5f));
	entities.push_back(triangleEnt2);
	//Entity* cubeMaybe = new Entity(cube);
	//entities.push_back(cubeMaybe);
	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	
	//D3D11_BUFFER_DESC vbd;
	//vbd.Usage = D3D11_USAGE_IMMUTABLE;
	//vbd.ByteWidth = sizeof(Vertex) * 3;       // 3 = number of vertices in the buffer
	//vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	//vbd.CPUAccessFlags = 0;
	//vbd.MiscFlags = 0;
	//vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	
	//D3D11_SUBRESOURCE_DATA initialVertexData;
	//initialVertexData.pSysMem = vertices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	
	//device->CreateBuffer(&vbd, &initialVertexData, vertexBuffer.GetAddressOf());



	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.

	//D3D11_BUFFER_DESC ibd;
	//ibd.Usage = D3D11_USAGE_IMMUTABLE;
	//ibd.ByteWidth = sizeof(int) * 3;         // 3 = number of indices in the buffer
	//ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	//ibd.CPUAccessFlags = 0;
	//ibd.MiscFlags = 0;
	//ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	
	//D3D11_SUBRESOURCE_DATA initialIndexData;
	//initialIndexData.pSysMem = indices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN

	//device->CreateBuffer(&ibd, &initialIndexData, indexBuffer.GetAddressOf());
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	if (camera != 0)
	{
		camera->UpdateProjectionMatrix((float)this->width / this->height);
	}
	DXCore::OnResize();
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	entities[0]->GetTransform()->Scale(glm::vec3(sin(totalTime) * deltaTime, sin(totalTime) * deltaTime, sin(totalTime) * deltaTime));
	//entities[2]->SetColorTint(1, cos(totalTime), tan(totalTime));
	entities[2]->GetTransform()->RotateAroundPoint(glm::vec3(0, -.4f, 0), totalTime);
	entities[2]->GetTransform()->SetRotation(glm::vec3(0, 0, totalTime));
	entities[3]->GetTransform()->SetPosition(glm::vec3(cos(totalTime), -sin(totalTime), 0));
	entities[4]->GetTransform()->SetPosition(glm::vec3(0, -sin(totalTime), 0));
	entities[4]->GetTransform()->Rotate(glm::vec3(0, 0, deltaTime));
	camera->Update(deltaTime, this->hWnd);
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
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


	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	//context->VSSetShader(vertexShader.Get(), 0, 0);
	//context->PSSetShader(pixelShader.Get(), 0, 0);


	// Ensure the pipeline knows how to interpret the data (numbers)
	// from the vertex buffer.  
	// - If all of your 3D models use the exact same vertex layout,
	//    this could simply be done once in Init()
	// - However, this isn't always the case (but might be for this course)


	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	//  - for this demo, this step *could* simply be done once during Init(),
	//    but I'm doing it here because it's often done multiple times per frame
	//    in a larger application/game
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		(*it)->Draw(context, this->camera);
	}
//---------------------------Begin commenting spree
	//UINT stride = sizeof(Vertex);
	//UINT offset = 0;

	//context->IASetVertexBuffers(0, 1, triangle->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	//context->IASetIndexBuffer(triangle->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	//VertexShaderExternalData vsData;
	//vsData.colorTint = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//XMStoreFloat4x4(&vsData.worldMatrix, XMMatrixIdentity());

	//D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	//context->Map(constantBufferVS, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	//memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	//context->Unmap(constantBufferVS, 0);

	//context->VSSetConstantBuffers(
	//		0,	// Which slot (register) to bind the buffer to?
	//		1,	// How many are	we activating ? Can do multiple at once
	//		&constantBufferVS);
	//// Array of buffers (or the address of one)
	//// Finally do the actual drawing
	////  - Do this ONCE PER OBJECT you intend to draw
	////  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	////  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	////     vertices in the currently set VERTEX BUFFER
	//context->DrawIndexed(
	//	triangle->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
	//	0,     // Offset to the first index we want to use
	//	0);    // Offset to add to each index when looking up vertices

	//context->IASetVertexBuffers(0, 1, square->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	//context->IASetIndexBuffer(square->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);


	//context->DrawIndexed(
	//	square->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
	//	0,     // Offset to the first index we want to use
	//	0);    // Offset to add to each index when looking up vertices

	//context->IASetVertexBuffers(0, 1, trapezoid->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	//context->IASetIndexBuffer(trapezoid->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	//context->DrawIndexed(
	//	trapezoid->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
	//	0,     // Offset to the first index we want to use
	//	0);    // Offset to add to each index when looking up vertices
//----------------------------------- End Commenting spree

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}