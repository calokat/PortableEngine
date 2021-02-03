#include "AssetBrowserWindow.h"
#include <Windows.h>
#include "OpenGLImageGraphicsData.h"
#include <imgui.h>
void AssetBrowserWindow::Render(IAssetManager* assetManager)
{
	const char* projectPath = "C:/Users/Caleb/Documents/Github/PE_Projects/demo/*";
	ImGui::Begin("Asset Browser");
	ImGui::SetWindowPos({ 200, 400 });
	ImGui::SetWindowSize({ 400, 200 });
	WIN32_FIND_DATA findResult;
	HANDLE fileHandle = FindFirstFile(projectPath, &findResult);
	FindNextFile(fileHandle, &findResult);
	while (FindNextFile(fileHandle, &findResult))
	{
		ImGui::BeginGroup();
		ImGui::Image(assetManager->GetDirectoryImage().imageGraphicsData->GetData(), ImVec2(50, 50));
		ImGui::Text("%s", findResult.cFileName);
		ImGui::EndGroup();
		ImGui::SameLine();
	}
	ImGui::End();
}
