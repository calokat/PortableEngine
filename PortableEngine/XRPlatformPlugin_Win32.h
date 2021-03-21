#pragma once
#include "IXRPlatformPlugin.h"
class XRPlatformPlugin_Win32 :
    public IXRPlatformPlugin
{
public:
    std::vector<const char*> GetPlatformExtensions();
    void* GetInstanceCreateExtension();

};

