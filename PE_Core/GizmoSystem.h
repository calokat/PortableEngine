#pragma once
#include "Camera.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "Mesh.h"
#include <vector>
#include <entt.hpp>
#include "IInputSystem.h"
class GizmoSystem
{
private:
	// TODO: Allow for multiple things to be selected
	static std::vector<entt::entity> selected;
	static ImGuizmo::OPERATION op;
public:
	static void DrawGizmo(Camera camera, entt::basic_view<entt::entity, entt::exclude_t<>, Transform> transformView);
	static void Select(entt::entity toSelect);
	static void Deselect(entt::entity toDeselect);
	static void DeselectAll();
	static entt::entity GetSelectedEntity();
	static void UpdateGizmo(IInputSystem* inputSystem);
};

