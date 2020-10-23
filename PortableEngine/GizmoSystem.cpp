#include "GizmoSystem.h"

std::vector<Transform*> GizmoSystem::selected;
ImGuizmo::OPERATION GizmoSystem::op;
void GizmoSystem::DrawGizmo(Camera camera)
{
	if (selected.empty())
	{
		return;
	}
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(glm::value_ptr(camera.view), glm::value_ptr(camera.projection), op, (op == ImGuizmo::SCALE) ? ImGuizmo::LOCAL : ImGuizmo::WORLD, glm::value_ptr(selected[0]->worldMatrix));

}

void GizmoSystem::Select(Transform* toSelect)
{
	selected.push_back(toSelect);
}

void GizmoSystem::Deselect(Transform* toDeselect)
{
	for (auto it = selected.begin(); it != selected.end(); ++it)
	{
		if (*it == toDeselect)
		{
			selected.erase(it);
			return;
		}
	}
}

void GizmoSystem::DeselectAll()
{
	selected.clear();
}
