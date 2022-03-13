#include "RendererMenu.h"
#include <imgui.h>
void RendererMenu::Render(entt::registry& registry, entt::entity selected, IRenderSystem* renderSystem)
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Renderer"))
	{
		if (ImGui::MenuItem("Unlit Color"))
		{
			renderSystem->CreateRenderer(registry, selected, ShaderType::Unlit_Color);
		}
		if (ImGui::MenuItem("Lit Color"))
		{
			renderSystem->CreateRenderer(registry, selected, ShaderType::Lit_Color);
		}
		if (ImGui::MenuItem("Unlit Textured"))
		{
			renderSystem->CreateRenderer(registry, selected, ShaderType::Unlit_Textured);
		}
		if (ImGui::MenuItem("Lit Textured"))
		{
			renderSystem->CreateRenderer(registry, selected, ShaderType::Lit_Textured);
		}
		if (ImGui::MenuItem("Lit Textured with Normal"))
		{
			renderSystem->CreateRenderer(registry, selected, ShaderType::Lit_Textured_Normal);
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

}
