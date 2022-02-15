#pragma once
#include "Camera.h"
#include <imgui.h>
#include "Mesh.h"
#include <vector>
#include <entt/entt.hpp>
#include "IInputSystem.h"
#include "GameWindow.h"
class GizmoSystem
{
private:
	// TODO: Allow for multiple things to be selected
	static std::vector<entt::entity> selected;
	//static ImGuizmo::OPERATION op;
public:
	static void DrawGizmo(Camera camera, entt::basic_view<entt::entity, entt::get_t<Transform>, entt::exclude_t<>, void> transformView, const GameWindow& gameWindow);
	static void Select(entt::entity toSelect);
	static void Deselect(entt::entity toDeselect);
	static void DeselectAll();
	static entt::entity GetSelectedEntity();
	static void UpdateGizmo(IInputSystem* inputSystem);
};

