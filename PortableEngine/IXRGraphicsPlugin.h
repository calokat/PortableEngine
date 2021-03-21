#pragma once
#include <vector>
#include "xr_defines.h"
class IXRGraphicsPlugin {
public:
	virtual std::vector<const char*> GetGraphicsExtensions() = 0;
};