 #ifdef __EMSCRIPTEN__
#include "EmscriptenAssetManager.h"
#include <filesystem>
#include "ImageSystem.h"

EmscriptenAssetManager::EmscriptenAssetManager()
{
    currentAssetPath = "../../Assets/Images/";
}

std::string EmscriptenAssetManager::GetAssetPath(std::string relativePath)
{
    return relativePath;
}

std::wstring EmscriptenAssetManager::GetAssetPath_Wide(std::wstring relativePath)
{
    return relativePath;
}

void EmscriptenAssetManager::LoadAssetsFromCurrentDirectory(IRenderSystem* renderSystem)
{
    UnloadAssets();
	assets.clear();
    auto dirIt = std::filesystem::directory_iterator(std::filesystem::path(currentAssetPath));
    bool foo = true;
    while (dirIt->exists())
    {
        PEAsset* asset = new PEAsset;
        asset->name = dirIt->path().filename();
        asset->path = dirIt->path();

        if (dirIt->path().extension().string() == ".png")
        {
            PEImage thumbnail = PEImage(dirIt->path().string());
            ImageSystem::CreateImage(thumbnail);
            renderSystem->LoadTexture(thumbnail);

            asset->thumbnail = std::make_shared<PEImage>(thumbnail);
            asset->assetType = AssetType::Image;
        }
        else if (dirIt->is_regular_file())
        {
            asset->assetType = AssetType::File;
            asset->thumbnail = this->assetThumbnail;
        }
        else if (dirIt->is_directory())
        {
            asset->assetType = AssetType::Directory;
            asset->thumbnail = this->directoryThumbnail;
        }
        else
        {
            printf("Something wrong in EmscriptenAssetManager\n");
        }
        assets.push_back(asset);
        dirIt++;
    }
}
 #endif