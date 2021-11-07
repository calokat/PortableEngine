//#ifdef __EMSCRIPTEN__
#pragma once
#include "IXRAPI.h"
#include "IGraphicsAPI.h"
#include <GLES3/gl3.h>
//#include <GL/glew.h>

struct WebXR_Eyebuffer {
    GLuint frameBuffer;
    GLuint colorBuffer;
    GLuint depthStencilBuffer;
};

void WebXR_Eyebuffer_Init(WebXR_Eyebuffer&);
void WebXR_Eyebuffer_AllocateRenderbuffers(WebXR_Eyebuffer buffer, unsigned int w, unsigned int h);
void WebXR_Eyebuffer_BindRenderBuffersToFrameBuffer(WebXR_Eyebuffer buffer);

class WebXRAPI :
    public IXRAPI
{
public:
    WebXRAPI(IGraphicsAPI* graph, IRenderSystem* rs);
    void PollEvents();
    bool IsSessionRunning();
    void Frame(entt::registry& reg, IRenderSystem* renderSystem);
    void RenderEye(entt::registry& reg, IRenderSystem* renderSystem, Camera& viewCam, WebXR_Eyebuffer& eyeBuffer, unsigned int index);
    static WebXRAPI* staticThis;
    IGraphicsAPI* graphics;
    IRenderSystem* renderSystem;
    //GLuint viewTextures[2];
    WebXR_Eyebuffer eyeBuffers[2];
    int eyeWidth, eyeHeight;
    bool isSessionRunning = false;
    Camera viewCameras[2];
};
//#endif