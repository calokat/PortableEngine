#include "AssetBrowserWindow.h"
#include <Windows.h>
#include "OpenGLImageGraphicsData.h"
#include <imgui.h>
#include <filesystem>
void AssetBrowserWindow::Render(IAssetManager* assetManager, IRenderSystem* renderSystem)
{
	ImGui::Begin("Asset Browser");
	ImGui::SetWindowPos({ 200, 400 });
	ImGui::SetWindowSize({ 400, 200 });

	for (auto it = assetManager->assets.begin(); it != assetManager->assets.end(); ++it)
	{
		PEAsset* asset = (*it);
		ImGui::BeginGroup();
		ImGui::Image(asset->thumbnail->imageGraphicsData->GetData(), ImVec2(50, 50));
		ImGui::Text("%s", asset->name.c_str());
		ImGui::EndGroup();
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			if (asset->assetType == AssetType::Directory)
			{
				assetManager->currentAssetPath = std::filesystem::path(asset->path).string();
				assetManager->currentAssetPath.push_back('/');
				assetManager->currentAssetPath.push_back('*');
				assetManager->LoadAssetsFromCurrentDirectory(renderSystem);
			}
			break;
		}
		ImGui::SameLine();

	}
	ImGui::End();
}
