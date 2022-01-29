#ifdef _WIN64
#pragma once
#include "IAssetManager.h"
#include <unordered_map>
#include <filesystem>
#include <Windows.h>

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

#endif