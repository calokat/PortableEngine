#include "AssetBrowserWindow.h"
#include "OpenGLImageGraphicsData.h"
#include <imgui.h>
#include <filesystem>
void AssetBrowserWindow::Render(IAssetManager* assetManager, IRenderSystem* renderSystem)
{
	ImGui::Begin("Asset Browser");
	ImGui::SetWindowPos({ 200, 400 });
	ImGui::SetWindowSize({ 400, 200 });

	if (ImGui::BeginTable("Assets", 5))
	{
		for (auto it = assetManager->assets.begin(); it != assetManager->assets.end(); ++it)
		{
			ImGui::TableNextColumn();
			PEAsset* asset = (*it);
			ImGui::BeginGroup();
			ImGui::Image(asset->thumbnail->imageGraphicsData->GetData(), ImVec2(50, 50));
			ImGui::TextWrapped("%s", asset->name.c_str());
			ImGui::EndGroup();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				if (asset->assetType == AssetType::Directory)
				{
					assetManager->currentAssetPath = asset->path;
					assetManager->LoadAssetsFromCurrentDirectory(renderSystem);
				}
				break;
			}
		}
		ImGui::EndTable();
	}
	ImGui::End();

	if (assetManager->WatchCurrentDirectoryForChanges())
	{
		assetManager->LoadAssetsFromCurrentDirectory(renderSystem);
	}
}
