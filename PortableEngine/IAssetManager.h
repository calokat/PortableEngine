#pragma once
#include <string>
class IAssetManager
{
public:
	virtual std::string GetAssetPath(std::string relativePath) = 0;
	virtual std::wstring GetAssetPath_Wide(std::wstring relativePath) = 0;
};