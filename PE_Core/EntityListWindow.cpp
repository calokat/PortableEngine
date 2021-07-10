#include "EntityListWindow.h"
#include <imgui.h>
#include "GizmoSystem.h"
void EntityListWindow::Render(Scene<entt::entity> namedEntities, entt::basic_view<entt::entity, entt::exclude_t<>, Name> nameView)
{
	ImGui::Begin("Entity List");
	ImGui::SetWindowPos({ 0, 20 });
	ImGui::SetWindowSize({ 200, 780 });

	SetUpGuiTree(namedEntities, nameView);
	
	ImGui::End();
}

void EntityListWindow::SetUpGuiTree(Scene<entt::entity> entityTree, entt::basic_view<entt::entity, entt::exclude_t<>, Name> nameView)
{
	Name entityName = nameView.get<Name>(entityTree.data);
	if (entityTree.children.size() == 0)
	{
		if (ImGui::MenuItem(entityName.nameString.c_str()))
		{
			GizmoSystem::DeselectAll();
			GizmoSystem::Select(entityTree.data);
		}
	}
	else
	{
		if (ImGui::TreeNodeEx(entityName.nameString.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
		{
			if (ImGui::IsItemClicked())
			{
				GizmoSystem::DeselectAll();
				GizmoSystem::Select(entityTree.data);
			}
			for (auto child : entityTree.children)
			{
				SetUpGuiTree(child, nameView);
			}
			ImGui::TreePop();
		}
	}
}
