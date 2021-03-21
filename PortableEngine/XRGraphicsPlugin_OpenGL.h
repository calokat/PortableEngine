#pragma once
#include "IXRGraphicsPlugin.h"
class XRGraphicsPlugin_OpenGL :
    public IXRGraphicsPlugin
{
    const char* GetGraphicsExtensionName();
};

