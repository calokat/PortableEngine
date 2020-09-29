#pragma once
#include "IGraphicsAPI.h"
#include "WindowsPlatform.h"
#include "GameWindow.h"
#include <d3d11.h>
#include "glm/glm.hpp"
//#include "Mesh.h"
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
class DirectXAPI :
	public IGraphicsAPI
{
public:
	int Init();
	GameWindow* window;
	DirectXAPI(GameWindow*);
	void ClearScreen();
	void Draw();
	~DirectXAPI();
	void _SwapBuffers();
private:
	void LoadShaders();
	void CreateMatrices();
	void CreateBasicGeometry();
	D3D_FEATURE_LEVEL		dxFeatureLevel;
	Microsoft::WRL::ComPtr<IDXGISwapChain>		swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>		device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	context;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBufferRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// ********** REQUIRED WITHOUT SIMPLE SHADER **********
	ID3D11Buffer* vsConstantBuffer;
	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D11InputLayout* inputLayout;
	// ********** REQUIRED WITHOUT SIMPLE SHADER **********

	// The matrices to go from model space to screen space
	glm::mat4 worldMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	//Mesh* testHelix;

};

