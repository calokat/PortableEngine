#include "WindowHeader.h"
#include <imgui.h>
#include "MeshLoaderSystem.h"
#include "TransformSystem.h"
#include "misc_components.h"
#include "AABBSystem.h"
void WindowHeader::Render(entt::registry& registry, IAssetManager* assetManager, IRenderSystem* renderSystem)
{
	ImGui::BeginMainMenuBar();
	ImGui::EndMainMenuBar();
}