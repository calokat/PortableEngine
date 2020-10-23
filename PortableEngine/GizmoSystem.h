#pragma once
#include "Camera.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "Mesh.h"
#include <vector>
class GizmoSystem
{
private:
	// TODO: Allow for multiple things to be selected
	static std::vector<Transform*> selected;
public:
	static void DrawGizmo(Camera camera);
	static void Select(Transform* toSelect);
	static void Deselect(Transform* toDeselect);
	static void DeselectAll();
	static ImGuizmo::OPERATION op;
};

