#include "EntityListWindow.h"
#include <imgui.h>
#include "GizmoSystem.h"
void EntityListWindow::Render(Relationship& rootRel, entt::basic_view<entt::entity, entt::exclude_t<>, Name, Relationship> nameView)
{
	ImGui::Begin("Entity List");
	ImGui::SetWindowPos({ 0, 20 });
	ImGui::SetWindowSize({ 200, 780 });
	// the root entity likely does not have a name, start with it's children
	for (auto it = rootRel.children.begin(); it != rootRel.children.end(); ++it)
	{
		SetUpGuiTree(it->second, nameView);
	}

	ImGui::End();
}

void EntityListWindow::SetUpGuiTree(entt::entity parent, entt::basic_view<entt::entity, entt::exclude_t<>, Name, Relationship> nameView)
{
	Name entityName = nameView.get<Name>(parent);
	Relationship& parentRel = nameView.get<Relationship>(parent);
	if (parentRel.children.size() == 0)
	{
		if (ImGui::MenuItem(entityName.nameString.c_str()))
		{
			GizmoSystem::DeselectAll();
			GizmoSystem::Select(parent);
		}
	}
	else
	{
		if (ImGui::TreeNodeEx(entityName.nameString.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
		{
			if (ImGui::IsItemClicked())
			{
				GizmoSystem::DeselectAll();
				GizmoSystem::Select(parent);
			}
			for (auto it = parentRel.children.begin(); it != parentRel.children.end(); ++it)
			{
				SetUpGuiTree(it->second, nameView);
			}
			ImGui::TreePop();
		}
	}
}
