#pragma once
#include <vector>
class IXRGraphicsPlugin {
public:
	virtual std::vector<const char*> GetGraphicsExtensions() = 0;
};