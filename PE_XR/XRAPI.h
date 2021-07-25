#pragma once
#include "IXRAPI.h"
#include "IPlatform.h"
#include "IGraphicsAPI.h"
#include "xr_defines.h"
#include "openxr/openxr.h"
#include "openxr/xr_linear.h"
#include "GameWindow.h"
#include <entt.hpp>
#include "IRenderSystem.h"
#include "misc_components.h"
#include <array>
#include "../PE_XR/IXRGraphicsPlugin.h"
#include "../PE_XR/IXRPlatformPlugin.h"
struct Swapchain {
    XrSwapchain handle;
    int32_t width;
    int32_t height;
};

enum Side {
    LEFT,
    RIGHT
};

struct InputState {
    XrActionSet actionSet{ XR_NULL_HANDLE };
    XrAction grabAction{ XR_NULL_HANDLE };
    XrAction poseAction{ XR_NULL_HANDLE };
    XrAction vibrateAction{ XR_NULL_HANDLE };
    XrAction quitAction{ XR_NULL_HANDLE };
    std::array<XrPath, 2> handSubactionPath;
    std::array<XrSpace, 2> handSpace;
    std::array<float, 2> handScale = { {1.0f, 1.0f} };
    std::array<XrBool32, 2> handActive;
};



class XRAPI
    : public IXRAPI
{
public:
	XRAPI(IPlatform* plat, IGraphicsAPI* graph, GameWindow* window, Options options);
    void PollEvents(/*bool* exitRenderLoop, bool* requestRestart*/);
    bool IsSessionRunning();
    void Frame(entt::registry& reg, IRenderSystem* renderSystem);
private:
    void UpdateDevices(XrTime predictedTime, entt::basic_view<entt::entity, entt::exclude_t<>, XRDevice, Transform> xrDevices, entt::basic_view<entt::entity, entt::exclude_t<>, Camera, Transform> cameraTransformView);
	IPlatform* platform;
	IGraphicsAPI* graphics;
    void InitializeActions();
    void PollActions();
    XrFrameState BeginFrame();
    void RenderFrame(entt::registry& reg, IRenderSystem* renderSystem, XrFrameState frameState);
    bool LocateViews(XrTime predictedDisplayTime);
    void CalculateCameraViews(const Transform& primaryCamTransform);
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

    std::vector<Camera> viewCams;
    GraphicsAPI graphicsAPI;
    InputState inputState;

    IXRPlatformPlugin* platformPlugin;
    IXRGraphicsPlugin* graphicsPlugin;
};

