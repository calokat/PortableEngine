#pragma once
#include <string>
#include "IRenderSystem.h"
#include "PEAsset.h"
#include <vector>
#include <memory>
class IAssetManager
{
public:
	virtual std::string GetAssetPath(std::string relativePath) = 0;
	virtual std::wstring GetAssetPath_Wide(std::wstring relativePath) = 0;
	virtual ~IAssetManager() {};
};