#include "InspectorWindow.h"
#include "GizmoSystem.h"
#include "InspectorGUI.h"
void InspectorWindow::Render(entt::registry& registry)
{
	ImGui::Begin("Inspector");
	ImGui::SetWindowPos({ 600, 20 });
	ImGui::SetWindowSize({ 200, 780 });
	entt::entity selected = GizmoSystem::GetSelectedEntity();
	if (selected != entt::null)
	{
		Transform* t = registry.try_get<Transform>(selected);
		if (t)
		{
			ComponentGUI(*t);
		}
		GLRenderer* glr = registry.try_get<GLRenderer>(selected);
		if (glr)
		{
			ComponentGUI(*glr);
		}
#ifdef _WIN64
		DirectXRenderer* r = registry.try_get<DirectXRenderer>(selected);
		if (r)
		{
			ComponentGUI(*r);
		}
#endif
	}
	ImGui::End();
}
