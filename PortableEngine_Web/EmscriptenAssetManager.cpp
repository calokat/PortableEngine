 #ifdef __EMSCRIPTEN__
#include "EmscriptenAssetManager.h"
#include <filesystem>
#include "ImageSystem.h"

std::string EmscriptenAssetManager::GetAssetPath(std::string relativePath)
{
    return relativePath;
}

std::wstring EmscriptenAssetManager::GetAssetPath_Wide(std::wstring relativePath)
{
    return relativePath;
}
 #endif