#include "GizmoSystem.h"

std::vector<entt::entity> GizmoSystem::selected;
ImGuizmo::OPERATION GizmoSystem::op;
void GizmoSystem::DrawGizmo(Camera camera, entt::basic_view<entt::entity, entt::exclude_t<>, Transform> transformView, const GameWindow& gameWindow)
{
	if (selected.empty())
	{
		return;
	}
	ImGuizmo::SetRect(0, 0, gameWindow.width, gameWindow.height);
	// assume only one thing can be selected at this time
	for (auto entity : transformView)
	{
		if (entity == selected[0])
		{
			Transform& t = transformView.get<Transform>(entity);
			ImGuizmo::Manipulate(glm::value_ptr(camera.view), glm::value_ptr(camera.projection), op, (op == ImGuizmo::SCALE) ? ImGuizmo::LOCAL : ImGuizmo::WORLD, glm::value_ptr(t.worldMatrix));
		}
	}

}

void GizmoSystem::Select(entt::entity toSelect)
{
	selected.push_back(toSelect);
}

void GizmoSystem::Deselect(entt::entity toDeselect)
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

entt::entity GizmoSystem::GetSelectedEntity()
{
	if (!selected.empty())
	{
		return selected[0];
	}
	return entt::null;
}

void GizmoSystem::UpdateGizmo(IInputSystem* inputSystem)
{
	if (inputSystem->IsKeyPressed(KeyboardCode::W))
	{
		op = ImGuizmo::TRANSLATE;
	}
	if (inputSystem->IsKeyPressed(KeyboardCode::E))
	{
		op = ImGuizmo::ROTATE;
	}
	if (inputSystem->IsKeyPressed(KeyboardCode::R))
	{
		op = ImGuizmo::SCALE;
	}
}
