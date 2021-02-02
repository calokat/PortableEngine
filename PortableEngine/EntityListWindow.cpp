#include "EntityListWindow.h"
#include <imgui.h>
#include "GizmoSystem.h"
void EntityListWindow::Render(entt::basic_view<entt::entity, entt::exclude_t<>, Name, Transform> namedEntities)
{
	ImGui::Begin("Entity List");
	ImGui::SetWindowPos({ 0, 20 });
	ImGui::SetWindowSize({ 200, 780 });
	for (auto entity : namedEntities)
	{
		Name name = namedEntities.get<Name>(entity);
		if (ImGui::MenuItem(name.nameString.c_str()))
		{
			GizmoSystem::DeselectAll();
			Transform& newSelected = namedEntities.get<Transform>(entity);
			GizmoSystem::Select(entity);
		}
	}
	ImGui::End();
}
