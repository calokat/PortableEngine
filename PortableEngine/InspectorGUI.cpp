#include "InspectorGUI.h"
#include <imgui.h>
#include "TransformSystem.h"
void ComponentGUI(Transform& t)
{
	TransformSystem::CalculatePosition(t);
	if (ImGui::DragFloat3("Position", glm::value_ptr(t.position), .1f))
	{
		//t.position = potentialNewPos;
		TransformSystem::CalculateWorldMatrix(&t);
	}
	TransformSystem::CalculateEulerAngles(t);
	if (ImGui::DragFloat3("Rotation", glm::value_ptr(t.rotation), .1f))
	{
		TransformSystem::CalculateWorldMatrix(&t);
	}
	TransformSystem::CalculateScale(t);
	if (ImGui::DragFloat3("Scale", glm::value_ptr(t.scale), .1f))
	{
		TransformSystem::CalculateWorldMatrix(&t);
	}
	//t.rotation = TransformSystem::CalculateEulerAngles(t);
	//ImGui::DragFloat3("Rotation", glm::value_ptr(t.rotation));
}