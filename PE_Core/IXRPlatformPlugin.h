#pragma once
#include <vector>
#include "xr_defines.h"
class IXRPlatformPlugin
{
public:
	virtual std::vector<const char*> GetPlatformExtensions() = 0;
	virtual void* GetInstanceCreateExtension() = 0;
};