#pragma once
#include "IXRAPI.h"
#include <entt.hpp>
#include "IRenderSystem.h"
#include <openxr/openxr.h>
#include "IPlatform.h"
#include "IGraphicsAPI.h"
#include "GameWindow.h"
class MockXRAPI :
    public IXRAPI
{
public:
    MockXRAPI();
    void PollEvents(/*bool* exitRenderLoop, bool* requestRestart*/);
    bool IsSessionRunning();
    void RenderFrame(entt::registry& reg, IRenderSystem* renderSystem);
};

