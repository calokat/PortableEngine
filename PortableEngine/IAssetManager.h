#pragma once
#include <string>
#include "IRenderSystem.h"

class IAssetManager
{
public:
	virtual std::string GetAssetPath(std::string relativePath) = 0;
	virtual std::wstring GetAssetPath_Wide(std::wstring relativePath) = 0;
	void LoadDefaultThumbnails(IRenderSystem* renderSystem);
	const PEImage& GetDefaultAssetImage();
	const PEImage& GetDirectoryImage();
protected:
	PEImage assetThumbnail;
	PEImage directoryThumbnail;
};