#pragma once
#include "IAssetManager.h"
#include "IInputSystem.h"
class AssetBrowserWindow
{
public:
	void Render(IAssetManager* assetManager, IRenderSystem* renderSystem);
};

