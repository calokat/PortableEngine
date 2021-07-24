#pragma once
#include <vector>
#include "xr_defines.h"
#include "openxr/openxr.h"
#include <entt.hpp>
#include "IRenderSystem.h"

class IXRGraphicsPlugin {
public:
	virtual std::vector<const char*> GetGraphicsExtensions() = 0;
	virtual void InitializeDeviceForXR(XrInstance instance, XrSystemId systemId) = 0;
	virtual const XrBaseInStructure* GetGraphicsBinding() = 0;
	virtual int64_t SelectColorSwapchainFormat(const std::vector<int64_t>& runtimeFormats) const = 0;
	virtual std::vector<XrSwapchainImageBaseHeader*> AllocateSwapchainImageStructs(uint32_t capacity, const XrSwapchainCreateInfo& /*swapchainCreateInfo*/) = 0;
	virtual void RenderView(const XrCompositionLayerProjectionView& layerView, const XrSwapchainImageBaseHeader* swapchainImage,
		int64_t swapchainFormat, entt::registry& reg, IRenderSystem* renderSystem, Camera viewCam) = 0;
};