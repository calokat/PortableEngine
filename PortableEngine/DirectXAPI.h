#pragma once
#include "IGraphicsAPI.h"
#include "WindowsPlatform.h"
#include "GameWindow.h"
#include <d3d11.h>
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
private:

	D3D_FEATURE_LEVEL		dxFeatureLevel;
	Microsoft::WRL::ComPtr<IDXGISwapChain>		swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>		device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	context;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBufferRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

};

