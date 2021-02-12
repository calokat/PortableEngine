#pragma once
#include "IAssetManager.h"
#include <unordered_map>
#include <filesystem>
#include <Windows.h>

class WindowsAssetManager :
	public IAssetManager
{
public:
	std::string GetAssetPath(std::string relativePath);
	std::wstring GetAssetPath_Wide(std::wstring relativePath);
	void LoadAssetsFromCurrentDirectory(IRenderSystem* renderSystem);
	bool WatchCurrentDirectoryForChanges();
private:
	std::string GetExePath();
	std::wstring GetExePath_Wide();
	//std::unordered_map<std::string, std::filesystem::file_time_type> pathToLastWrite;
	DWORD dwWaitStatus;
	HANDLE dwChangeHandles[2];
};

