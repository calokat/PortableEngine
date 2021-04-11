#pragma once
#include <entt.hpp>
#include "IRenderSystem.h"
class IXRAPI
{
public:
    virtual void PollEvents() = 0;
    virtual bool IsSessionRunning() = 0;
    virtual void RenderFrame(entt::registry& reg, IRenderSystem* renderSystem) = 0;
    virtual ~IXRAPI() {};
};

