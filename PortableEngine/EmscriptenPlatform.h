#pragma once
#include "IPlatform.h"
class EmscriptenPlatform :
	public IPlatform
{
public:
	int InitWindow();
	long Run();
	std::string GetAssetPath(std::string relativePath);
	std::wstring GetAssetPath_Wide(std::wstring relativePath);
};

