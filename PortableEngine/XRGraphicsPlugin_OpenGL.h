#pragma once
#include "IXRGraphicsPlugin.h"
class XRGraphicsPlugin_OpenGL :
    public IXRGraphicsPlugin
{
    std::vector<const char*> GetGraphicsExtensions();
};

