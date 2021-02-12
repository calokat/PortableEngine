#pragma once
#include "IAssetManager.h"
class EmscriptenAssetManager : public IAssetManager 
{
	std::string GetAssetPath(std::string relativePath);
	std::wstring GetAssetPath_Wide(std::wstring relativePath);
	void LoadAssetsFromCurrentDirectory(IRenderSystem* renderSystem){};
	bool WatchCurrentDirectoryForChanges() {return false;};
};