#pragma once
#include <entt/entt.hpp>
#include "IRenderSystem.h"
class IXRAPI
{
public:
    virtual void PollEvents() = 0;
    virtual bool IsSessionRunning() = 0;
    virtual void Frame(entt::registry& reg, IRenderSystem* renderSystem) = 0;
    virtual ~IXRAPI() {};
};

enum XRDeviceType {
    LeftHand,
    RightHand
};

struct XRDevice
{
    XRDeviceType type;
};