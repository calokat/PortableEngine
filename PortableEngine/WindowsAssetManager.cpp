#include "WindowsAssetManager.h"
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
	std::string fakePath = currentAssetPath;
	dwChangeHandles[0] = FindFirstChangeNotification(
		fakePath.c_str(),              // directory to watch 
		FALSE,                         // do not watch subtree 
		FILE_NOTIFY_CHANGE_FILE_NAME); // watch file name changes 

	dwChangeHandles[1] = FindFirstChangeNotification(
		fakePath.c_str(),      // directory to watch 
		TRUE,                          // watch the subtree 
		FILE_NOTIFY_CHANGE_DIR_NAME);  // watch dir name changes 

	//pathToLastWrite[currentAssetPath] = std::filesystem::last_write_time(currentAssetPath);
	std::string realPath = currentAssetPath + "/*";
	UnloadAssets();
	assets.clear();
	WIN32_FIND_DATA findResult;
	HANDLE fileHandle = FindFirstFile((realPath).c_str(), &findResult);
	FindNextFile(fileHandle, &findResult);
	// get rid of the wildcard at the end
	realPath.pop_back();
	while (FindNextFile(fileHandle, &findResult))
	{
		std::filesystem::path assetPath = realPath + findResult.cFileName;
		PEImage assetThumbnail;
		if (!std::filesystem::is_directory(assetPath))
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

bool WindowsAssetManager::WatchCurrentDirectoryForChanges()
{
	dwWaitStatus = WaitForMultipleObjects(2, dwChangeHandles,
		FALSE, 20);

	switch (dwWaitStatus)
	{
		case WAIT_OBJECT_0:
		case WAIT_OBJECT_0 + 1:
			FindCloseChangeNotification(dwChangeHandles[0]);
			FindCloseChangeNotification(dwChangeHandles[1]);
			return true;
			break;
		default:
			return false;
			break;
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
