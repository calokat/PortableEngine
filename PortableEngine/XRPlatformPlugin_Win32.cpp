#include "XRPlatformPlugin_Win32.h"

std::vector<const char*> XRPlatformPlugin_Win32::GetPlatformExtensions()
{
    return std::vector<const char*>();
}

void* XRPlatformPlugin_Win32::GetInstanceCreateExtension()
{
    return nullptr;
}
