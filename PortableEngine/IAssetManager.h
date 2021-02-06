#pragma once
#include <string>
#include "IRenderSystem.h"
#include "PEAsset.h"
#include <vector>
class IAssetManager
{
public:
	virtual std::string GetAssetPath(std::string relativePath) = 0;
	virtual std::wstring GetAssetPath_Wide(std::wstring relativePath) = 0;
	void LoadDefaultThumbnails(IRenderSystem* renderSystem);
	const PEImage& GetDefaultAssetImage();
	const PEImage& GetDirectoryImage();
	virtual void LoadAssetsFromCurrentDirectory(std::string dirPath, IRenderSystem* renderSystem) = 0;
	const char* currentAssetPath = "C:/Users/Caleb/Documents/Github/PE_Projects/demo/*";
	std::vector<PEAsset*> assets;
	virtual ~IAssetManager();
protected:
	PEImage assetThumbnail;
	PEImage directoryThumbnail;
};