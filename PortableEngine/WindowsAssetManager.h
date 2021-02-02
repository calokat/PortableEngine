#pragma once
#include "IAssetManager.h"
class WindowsAssetManager :
	public IAssetManager
{
public:
	std::string GetAssetPath(std::string relativePath);
	std::wstring GetAssetPath_Wide(std::wstring relativePath);
private:
	std::string GetExePath();
	std::wstring GetExePath_Wide();
};

