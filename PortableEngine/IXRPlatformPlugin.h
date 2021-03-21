#pragma once
#include <vector>
class IXRPlatformPlugin
{
public:
	virtual std::vector<const char*> GetPlatformExtensions() = 0;
};