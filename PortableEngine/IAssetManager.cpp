#include "IAssetManager.h"
#include "ImageSystem.h"
void IAssetManager::LoadDefaultThumbnails(IRenderSystem* renderSystem)
{
	assetThumbnail = PEImage(GetAssetPath("../../Assets/Images/asset.png"));
	ImageSystem::CreateImage(assetThumbnail);
	renderSystem->LoadTexture(assetThumbnail);

	directoryThumbnail = PEImage(GetAssetPath("../../Assets/Images/directory.png"));
	ImageSystem::CreateImage(directoryThumbnail);
	renderSystem->LoadTexture(directoryThumbnail);
}

const PEImage& IAssetManager::GetDefaultAssetImage()
{
	return assetThumbnail;
}

const PEImage& IAssetManager::GetDirectoryImage()
{
	return directoryThumbnail;
}

IAssetManager::~IAssetManager()
{
	for (auto it = assets.begin(); it != assets.end(); ++it)
	{
		delete* it;
	}
}
