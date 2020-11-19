#pragma once
#include "Camera.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "Mesh.h"
#include <vector>
#include <entt.hpp>
class GizmoSystem
{
private:
	// TODO: Allow for multiple things to be selected
	static std::vector<entt::entity> selected;
public:
	static void DrawGizmo(Camera camera, entt::basic_view<entt::entity, entt::exclude_t<>, Transform> transformView);
	static void Select(entt::entity toSelect);
	static void Deselect(entt::entity toDeselect);
	static void DeselectAll();
	static ImGuizmo::OPERATION op;
	static entt::entity GetSelectedEntity();
};

