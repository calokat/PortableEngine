#include "WindowsAssetManager.h"
#include <Windows.h>
#include <filesystem>
#include "ImageSystem.h"

std::string WindowsAssetManager::GetAssetPath(std::string relativePath)
{
	return GetExePath() + "\\" + relativePath;
}

std::wstring WindowsAssetManager::GetAssetPath_Wide(std::wstring relativePath)
{
	return GetExePath_Wide() + L"\\" + relativePath;
}

void WindowsAssetManager::LoadAssetsFromCurrentDirectory(IRenderSystem* renderSystem)
{
	std::string realPath = currentAssetPath;
	assets.clear();
	WIN32_FIND_DATA findResult;
	HANDLE fileHandle = FindFirstFile(realPath.c_str(), &findResult);
	FindNextFile(fileHandle, &findResult);
	while (FindNextFile(fileHandle, &findResult))
	{
		std::filesystem::path assetPath = realPath.substr(0, realPath.length() - 1).append(findResult.cFileName);
		PEImage assetThumbnail;
		if (assetPath.has_extension())
		{
			if (assetPath.extension() == ".png")
			{
				assetThumbnail = PEImage(assetPath.string());
				ImageSystem::CreateImage(assetThumbnail);
				renderSystem->LoadTexture(assetThumbnail);
				PEAsset* asset = new PEAsset;
				asset->assetType = AssetType::Image;
				asset->name = findResult.cFileName;
				asset->path = assetPath.string();
				asset->thumbnail = std::make_shared<PEImage>(assetThumbnail);
				//PEAsset asset = { assetThumbnail, assetPath.string(), AssetType::Image, findResult.cFileName };
				assets.push_back(asset);
			}
			else
			{
				PEAsset* asset = new PEAsset;
				asset->assetType = AssetType::File;
				asset->name = std::string(findResult.cFileName);
				asset->path = assetPath.string();
				asset->thumbnail = this->assetThumbnail;
				//asset->thumbnail = new PEImage(this->assetThumbnail.path);
				//ImageSystem::CreateImage(*asset->thumbnail);
				//renderSystem->LoadTexture(*asset->thumbnail);
				//PEAsset asset = { this->assetThumbnail, assetPath.string(), AssetType::File, findResult.cFileName };
				assets.push_back(asset);
			}
		}
		else
		{
			PEAsset* asset = new PEAsset;
			asset->assetType = AssetType::Directory;
			asset->name = std::string(findResult.cFileName);
			asset->path = assetPath.string();
			asset->thumbnail = this->directoryThumbnail;
			//asset->thumbnail = new PEImage(this->directoryThumbnail.path);
			//ImageSystem::CreateImage(*asset->thumbnail);
			//renderSystem->LoadTexture(*asset->thumbnail);
			//PEAsset asset = { this->assetThumbnail, assetPath.string(), AssetType::File, findResult.cFileName };
			assets.push_back(asset);
		}

		//ImGui::BeginGroup();
		//ImGui::Image(assetManager->GetDirectoryImage().imageGraphicsData->GetData(), ImVec2(50, 50));
		//ImGui::Text("%s", findResult.cFileName);
		//ImGui::EndGroup();
		//ImGui::SameLine();

	}
}

std::string WindowsAssetManager::GetExePath()
{
	// Assume the path is just the "current directory" for now
	std::string path = ".\\";

	// Get the real, full path to this executable
	char currentDir[1024] = {};
	GetModuleFileName(0, currentDir, 1024);

	// Find the location of the last slash charaacter
	char* lastSlash = strrchr(currentDir, '\\');
	if (lastSlash)
	{
		// End the string at the last slash character, essentially
		// chopping off the exe's file name.  Remember, c-strings
		// are null-terminated, so putting a "zero" character in 
		// there simply denotes the end of the string.
		*lastSlash = 0;

		// Set the remainder as the path
		path = currentDir;
	}

	// Toss back whatever we've found
	return path;
}

std::wstring WindowsAssetManager::GetExePath_Wide()
{
	// Grab the path as a standard string
	std::string path = GetExePath();

	// Convert to a wide string
	wchar_t widePath[1024] = {};
	mbstowcs_s(0, widePath, path.c_str(), 1024);

	// Create a wstring for it and return
	return std::wstring(widePath);
}
