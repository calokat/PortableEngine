#pragma once
#include "IXRAPI.h"
#include "IPlatform.h"
#include "IGraphicsAPI.h"
#include "xr_defines.h"
#include "openxr/openxr.h"
#include "openxr/openxr_platform.h"
#include "openxr/xr_linear.h"
#include "GameWindow.h"
#include <entt.hpp>
#include "IRenderSystem.h"

struct Swapchain {
    XrSwapchain handle;
    int32_t width;
    int32_t height;
};


class XRAPI
    : public IXRAPI
{
public:
	XRAPI(IPlatform* plat, IGraphicsAPI* graph, GameWindow* window);
    void PollEvents(/*bool* exitRenderLoop, bool* requestRestart*/);
    bool IsSessionRunning();
    void RenderFrame(entt::registry& reg, IRenderSystem* renderSystem);
private:
	IPlatform* platform;
	IGraphicsAPI* graphics;
    XrFrameState BeginFrame();
	XrResult Init();
	XrResult CreateXRInstance();
	void InitializeXRSystem();
	void InitializeXRSession();
	void CreateVisualizedSpaces();
	void CreateSwapchains(GameWindow* window);
    const XrEventDataBaseHeader* TryReadNextEvent();
    void HandleSessionStateChangedEvent(const XrEventDataSessionStateChanged& stateChangedEvent/*, bool* exitRenderLoop,
        bool* requestRestart*/);
    bool RenderLayer(XrTime predictedDisplayTime, std::vector<XrCompositionLayerProjectionView>& projectionLayerViews,
        XrCompositionLayerProjection& layer, entt::registry& reg, IRenderSystem* renderSystem);
    XrInstance m_instance{ XR_NULL_HANDLE };
    XrSession m_session{ XR_NULL_HANDLE };
    XrSpace m_appSpace{ XR_NULL_HANDLE };
    XrFormFactor m_formFactor{ XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY };
    XrViewConfigurationType m_viewConfigType{ XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO };
    XrEnvironmentBlendMode m_environmentBlendMode{ XR_ENVIRONMENT_BLEND_MODE_OPAQUE };
    XrSystemId m_systemId{ XR_NULL_SYSTEM_ID };

    std::vector<XrViewConfigurationView> m_configViews;
    std::vector<Swapchain> m_swapchains;
    std::map<XrSwapchain, std::vector<XrSwapchainImageBaseHeader*>> m_swapchainImages;
    std::vector<XrView> m_views;
    int64_t m_colorSwapchainFormat{ -1 };

    std::vector<XrSpace> m_visualizedSpaces;

    // Application's current lifecycle state according to the runtime
    XrSessionState m_sessionState{ XR_SESSION_STATE_UNKNOWN };
    bool m_sessionRunning{ false };

    XrEventDataBuffer m_eventDataBuffer;
};

