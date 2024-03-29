#include "InspectorWindow.h"
#include "GizmoSystem.h"
#include "InspectorGUI.h"
void InspectorWindow::Render(entt::registry& registry)
{
	ImGui::Begin("Inspector");
	entt::entity selected = GizmoSystem::GetSelectedEntity();
	if (selected != entt::null)
	{
		Name* n = registry.try_get<Name>(selected);
		if (n)
		{
			ComponentGUI(*n);
		}
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
		PointLight* pl = registry.try_get<PointLight>(selected);
		if (pl)
		{
			ComponentGUI(*pl);
		}
		DirectionalLight* dl = registry.try_get<DirectionalLight>(selected);
		if (dl)
		{
			ComponentGUI(*dl);
		}
		SpotLight* sl = registry.try_get<SpotLight>(selected);
		if (sl)
		{
			ComponentGUI(*sl);
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
