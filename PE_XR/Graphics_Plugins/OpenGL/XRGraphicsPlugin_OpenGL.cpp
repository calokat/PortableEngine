#include "XRGraphicsPlugin_OpenGL.h"
#include <GL/glew.h>
#include <assert.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include "GLRenderer.h"
#include "Transform.h"
#include "GizmoSystem.h"
#include "CameraSystem.h"
#include "TransformSystem.h"
#include "LightsSystem.h"
XRGraphicsPlugin_OpenGL::XRGraphicsPlugin_OpenGL(OpenGLAPI* glApi, GameWindow* win) : window(win), glContext(glApi->GetOpenGLContext())
{
}

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

    // this was originally in InitializeResources() from hello_xr, but I think I only need this part of it
    glGenFramebuffers(1, &m_swapchainFramebuffer);
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

void XRGraphicsPlugin_OpenGL::RenderView(const XrCompositionLayerProjectionView& layerView, const XrSwapchainImageBaseHeader* swapchainImage, int64_t swapchainFormat, entt::registry& reg, IRenderSystem* renderSystem, Camera viewCam)
{
    assert(layerView.subImage.imageArrayIndex == 0);  // Texture arrays not supported.
    //UNUSED_PARM(swapchainFormat);                    // Not used in this function for now.

    glBindFramebuffer(GL_FRAMEBUFFER, m_swapchainFramebuffer);

    const uint32_t colorTexture = reinterpret_cast<const XrSwapchainImageOpenGLKHR*>(swapchainImage)->image;

    glViewport(static_cast<GLint>(layerView.subImage.imageRect.offset.x),
        static_cast<GLint>(layerView.subImage.imageRect.offset.y),
        static_cast<GLsizei>(layerView.subImage.imageRect.extent.width),
        static_cast<GLsizei>(layerView.subImage.imageRect.extent.height));

    //glFrontFace(GL_CW);
    //glCullFace(GL_BACK);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    const uint32_t depthTexture = GetDepthTexture(colorTexture);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    // Clear swapchain and depth buffer.
    //glClearColor(DarkSlateGray[0], DarkSlateGray[1], DarkSlateGray[2], DarkSlateGray[3]);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Set shaders and uniform variables.
    //glUseProgram(m_program);

    DirectionalLight dirLights[MAX_DIR_LIGHTS];
    PointLight pointLights[MAX_POINT_LIGHTS];
    LightsSystem::ExtractLightsFromRegistry(reg, dirLights, pointLights);
    auto renderableView = reg.view<GLRenderer, Transform>();
    for (auto renderable : renderableView)
    {
        GLRenderer& renderer = reg.get<GLRenderer>(renderable);
        Mesh& mesh = reg.get<Mesh>(renderable);
        Transform& meshTransform = reg.get<Transform>(renderable);
        //renderSystem->LoadMesh(&renderer, mesh);
        renderSystem->BindRenderer(&renderer);
        renderSystem->UpdateRenderer(&renderer, meshTransform, viewCam, dirLights, pointLights);
        renderSystem->Draw(&renderer);
    }

    //ImGui::Render();
    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //// Render each cube
    //for (const Cube& cube : cubes) {
    //    // Compute the model-view-projection transform and set it..
    //    XrMatrix4x4f model;
    //    XrMatrix4x4f_CreateTranslationRotationScale(&model, &cube.Pose.position, &cube.Pose.orientation, &cube.Scale);
    //    XrMatrix4x4f mvp;
    //    XrMatrix4x4f_Multiply(&mvp, &vp, &model);
    //    glUniformMatrix4fv(m_modelViewProjectionUniformLocation, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&mvp));

    //    // Draw the cube.
    //    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ArraySize(Geometry::c_cubeIndices)), GL_UNSIGNED_SHORT, nullptr);
    //}

    // Swap our window every other eye for RenderDoc
    //static int everyOther = 0;
    //if ((everyOther++ & 1) != 0) {
    //    glContext->_SwapBuffers(-1);
    //}
}

uint32_t XRGraphicsPlugin_OpenGL::GetDepthTexture(uint32_t colorTexture)
{
    // If a depth-stencil view has already been created for this back-buffer, use it.
    auto depthBufferIt = m_colorToDepthMap.find(colorTexture);
    if (depthBufferIt != m_colorToDepthMap.end()) {
        return depthBufferIt->second;
    }

    // This back-buffer has no corresponding depth-stencil texture, so create one with matching dimensions.
    GLint width;
    GLint height;
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    uint32_t depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    m_colorToDepthMap.insert(std::make_pair(colorTexture, depthTexture));
    return depthTexture;
}
