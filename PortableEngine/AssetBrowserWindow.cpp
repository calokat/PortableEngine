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

	for (auto it = assetManager->assets.begin(); it != assetManager->assets.end(); ++it)
	{
		PEAsset* asset = (*it);
		ImGui::BeginGroup();
		if (asset->thumbnail == nullptr)
		{
			if (asset->assetType == AssetType::File)
			{
				ImGui::Image(assetManager->GetDefaultAssetImage().imageGraphicsData->GetData(), ImVec2(50, 50));
			}
			else if (asset->assetType == AssetType::Directory)
			{
				ImGui::Image(assetManager->GetDirectoryImage().imageGraphicsData->GetData(), ImVec2(50, 50));
			}
		}
		else
		{
			ImGui::Image(asset->thumbnail->imageGraphicsData->GetData(), ImVec2(50, 50));
		}
		ImGui::Text("%s", asset->name.c_str());
		ImGui::EndGroup();
		ImGui::SameLine();

	}
	ImGui::End();
}
