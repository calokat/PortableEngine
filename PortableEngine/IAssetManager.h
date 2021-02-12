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
	void LoadDefaultThumbnails(IRenderSystem* renderSystem);
	const PEImage& GetDefaultAssetImage();
	const PEImage& GetDirectoryImage();
	virtual void LoadAssetsFromCurrentDirectory(IRenderSystem* renderSystem) = 0;
	std::string currentAssetPath = "C:/Users/Caleb/Documents/Github/PE_Projects/demo/*";
	std::vector<PEAsset*> assets;
	virtual ~IAssetManager();
	virtual bool WatchCurrentDirectoryForChanges() = 0;
protected:
	std::shared_ptr<PEImage> assetThumbnail;
	std::shared_ptr<PEImage> directoryThumbnail;
	void UnloadAssets();
};