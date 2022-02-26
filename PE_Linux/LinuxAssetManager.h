#pragma once
#include "IAssetManager.h"
class LinuxAssetManager : public IAssetManager 
{
public:
	std::string GetAssetPath(std::string relativePath);
	std::wstring GetAssetPath_Wide(std::wstring relativePath);
};
