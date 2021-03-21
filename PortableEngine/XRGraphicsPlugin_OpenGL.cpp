#include "XRGraphicsPlugin_OpenGL.h"
#include <GL/glew.h>
XRGraphicsPlugin_OpenGL::XRGraphicsPlugin_OpenGL(GameWindow* win, IOpenGLContext* ctx) : window(win), glContext(ctx)
{
}
std::vector<const char*> XRGraphicsPlugin_OpenGL::GetGraphicsExtensions()
{
    return { "XR_KHR_opengl_enable" };
}

void XRGraphicsPlugin_OpenGL::InitializeDeviceForXR(XrInstance instance, XrSystemId systemId)
{
    PFN_xrGetOpenGLGraphicsRequirementsKHR pfnGetOpenGLGraphicsRequirementsKHR = nullptr;
    xrGetInstanceProcAddr(instance, "xrGetOpenGLGraphicsRequirementsKHR",
        reinterpret_cast<PFN_xrVoidFunction*>(&pfnGetOpenGLGraphicsRequirementsKHR));

    XrGraphicsRequirementsOpenGLKHR graphicsRequirements{ XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR };
    pfnGetOpenGLGraphicsRequirementsKHR(instance, systemId, &graphicsRequirements);

    GLint major = 0;
    GLint minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    const XrVersion desiredApiVersion = XR_MAKE_VERSION(major, minor, 0);
    if (graphicsRequirements.minApiVersionSupported > desiredApiVersion) {
        printf("Runtime does not support desired Graphics API and/or version");
    }

    m_graphicsBinding.hDC = (HDC)window->deviceContext;
    m_graphicsBinding.hGLRC = (HGLRC)glContext->GetContext(window);

}

const XrBaseInStructure* XRGraphicsPlugin_OpenGL::GetGraphicsBinding()
{
    return reinterpret_cast<const XrBaseInStructure*>(&m_graphicsBinding);
}

int64_t XRGraphicsPlugin_OpenGL::SelectColorSwapchainFormat(const std::vector<int64_t>& runtimeFormats) const
{
    // List of supported color swapchain formats.
    constexpr int64_t SupportedColorSwapchainFormats[] = {
        GL_RGB10_A2,
        GL_RGBA16F,
        // The two below should only be used as a fallback, as they are linear color formats without enough bits for color
        // depth, thus leading to banding.
        GL_RGBA8,
        GL_RGBA8_SNORM,
    };

    auto swapchainFormatIt =
        std::find_first_of(runtimeFormats.begin(), runtimeFormats.end(), std::begin(SupportedColorSwapchainFormats),
            std::end(SupportedColorSwapchainFormats));
    if (swapchainFormatIt == runtimeFormats.end()) {
        printf("No runtime swapchain format supported for color swapchain");
    }

    return *swapchainFormatIt;
}

std::vector<XrSwapchainImageBaseHeader*> XRGraphicsPlugin_OpenGL::AllocateSwapchainImageStructs(uint32_t capacity, const XrSwapchainCreateInfo&)
{
    // Allocate and initialize the buffer of image structs (must be sequential in memory for xrEnumerateSwapchainImages).
// Return back an array of pointers to each swapchain image struct so the consumer doesn't need to know the type/size.
    std::vector<XrSwapchainImageOpenGLKHR> swapchainImageBuffer(capacity);
    std::vector<XrSwapchainImageBaseHeader*> swapchainImageBase;
    for (XrSwapchainImageOpenGLKHR& image : swapchainImageBuffer) {
        image.type = XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR;
        swapchainImageBase.push_back(reinterpret_cast<XrSwapchainImageBaseHeader*>(&image));
    }

    // Keep the buffer alive by moving it into the list of buffers.
    m_swapchainImageBuffers.push_back(std::move(swapchainImageBuffer));

    return swapchainImageBase;

}
