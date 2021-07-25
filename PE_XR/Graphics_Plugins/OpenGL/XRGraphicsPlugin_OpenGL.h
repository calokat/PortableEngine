#pragma once
#include "../PE_XR/IXRGraphicsPlugin.h"
#ifdef _WIN32
#include <Windows.h>
#endif
#undef XR_USE_GRAPHICS_API_D3D11
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#define XR_USE_GRAPHICS_API_D3D11 1
#include "GameWindow.h"
#include "IOpenGLContext.h"
#include "OpenGLAPI.h"
#include <algorithm>
#include <list>
#include <map>
#include <GL/glew.h>
class XRGraphicsPlugin_OpenGL :
    public IXRGraphicsPlugin
{
public:
    XRGraphicsPlugin_OpenGL(OpenGLAPI* glApi, GameWindow* win);
    XRGraphicsPlugin_OpenGL(GameWindow* win, IOpenGLContext* ctx);
    std::vector<const char*> GetGraphicsExtensions();
    void InitializeDeviceForXR(XrInstance instance, XrSystemId systemId);
    const XrBaseInStructure* GetGraphicsBinding();
    int64_t SelectColorSwapchainFormat(const std::vector<int64_t>& runtimeFormats) const;
    std::vector<XrSwapchainImageBaseHeader*> AllocateSwapchainImageStructs(uint32_t capacity, const XrSwapchainCreateInfo& /*swapchainCreateInfo*/);
    void RenderView(const XrCompositionLayerProjectionView& layerView, const XrSwapchainImageBaseHeader* swapchainImage,
        int64_t swapchainFormat, entt::registry& reg, IRenderSystem* renderSystem, Camera viewCam);
private:
    uint32_t GetDepthTexture(uint32_t colorTexture);
#ifdef _WIN32
    XrGraphicsBindingOpenGLWin32KHR m_graphicsBinding{ XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR };
#endif
    GameWindow* window;
    IOpenGLContext* glContext;
    std::list<std::vector<XrSwapchainImageOpenGLKHR>> m_swapchainImageBuffers;
    GLuint m_swapchainFramebuffer{ 0 };
    std::map<uint32_t, uint32_t> m_colorToDepthMap;
};

