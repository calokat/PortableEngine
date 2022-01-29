 #ifdef __EMSCRIPTEN__
#pragma once
#include "IAssetManager.h"
class EmscriptenAssetManager : public IAssetManager 
{
public:
	std::string GetAssetPath(std::string relativePath);
	std::wstring GetAssetPath_Wide(std::wstring relativePath);
};
 #endif