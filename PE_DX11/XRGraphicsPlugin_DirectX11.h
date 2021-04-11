#pragma once
#include "IXRGraphicsPlugin.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#define XR_USE_GRAPHICS_API_D3D11 1
#include <openxr/openxr_platform.h>
#include <windows.h>
#include <wrl/client.h>  // For Microsoft::WRL::ComPtr
#include "GameWindow.h"

class XRGraphicsPlugin_DirectX11 :
    public IXRGraphicsPlugin
{
public:
	XRGraphicsPlugin_DirectX11(Microsoft::WRL::ComPtr<ID3D11DeviceContext> ctx, Microsoft::WRL::ComPtr<ID3D11Device> dev, GameWindow* win);
	virtual std::vector<const char*> GetGraphicsExtensions();
	virtual void InitializeDeviceForXR(XrInstance instance, XrSystemId systemId);
	virtual const XrBaseInStructure* GetGraphicsBinding();
	virtual int64_t SelectColorSwapchainFormat(const std::vector<int64_t>& runtimeFormats) const;
	virtual std::vector<XrSwapchainImageBaseHeader*> AllocateSwapchainImageStructs(uint32_t capacity, const XrSwapchainCreateInfo& /*swapchainCreateInfo*/);
	virtual void RenderView(const XrCompositionLayerProjectionView& layerView, const XrSwapchainImageBaseHeader* swapchainImage,
		int64_t swapchainFormat, entt::registry& reg, IRenderSystem* renderSystem);
private:
	Microsoft::WRL::ComPtr<IDXGIAdapter1> GetAdapter(LUID adapterId);
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView(ID3D11Texture2D* colorTexture);
	XrGraphicsBindingD3D11KHR m_graphicsBinding{ XR_TYPE_GRAPHICS_BINDING_D3D11_KHR };
	std::list<std::vector<XrSwapchainImageD3D11KHR>> m_swapchainImageBuffers;
	GameWindow* window;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	std::map<ID3D11Texture2D*, Microsoft::WRL::ComPtr<ID3D11DepthStencilView>> m_colorToDepthMap;
};

