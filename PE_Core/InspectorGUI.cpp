#include "InspectorGUI.h"
#include <imgui.h>
#include "TransformSystem.h"
#include "OpenGLImageGraphicsData.h"
#ifdef _WIN64
#include "DirectX11ImageGraphicsData.h"
#endif
void ComponentGUI(Transform& t)
{
	//TransformSystem::CalculatePosition(t);
	if (ImGui::DragFloat3("Position", glm::value_ptr(t.position), .01f))
	{
		//t.position = potentialNewPos;
		TransformSystem::CalculateWorldMatrix(&t);
	}
	//TransformSystem::CalculateEulerAngles(t);
	if (ImGui::DragFloat3("Rotation", glm::value_ptr(t.rotation), .01f))
	{
		TransformSystem::SetOrientation(t.rotation, &t);
		TransformSystem::CalculateWorldMatrix(&t);
	}
	//TransformSystem::CalculateScale(t);
	if (ImGui::DragFloat3("Scale", glm::value_ptr(t.scale), .01f))
	{
		TransformSystem::CalculateWorldMatrix(&t);
	}
	//t.rotation = TransformSystem::CalculateEulerAngles(t);
	//ImGui::DragFloat3("Rotation", glm::value_ptr(t.rotation));
}

void ComponentGUI(GLRenderer& r)
{
	ImGui::ColorPicker4("Renderer Color", glm::value_ptr(r.vertexColor));
	ImGui::Image((void*)(std::dynamic_pointer_cast<OpenGLImageGraphicsData>(r.diffuseTexture.imageGraphicsData))->texture, ImVec2(100, 100));
}

void ComponentGUI(Rotator& r)
{
	ImGui::DragFloat3("Delta Rotation", glm::value_ptr(r.deltaRot), .01f);
	//r.deltaRot.x = glm::radians(r.deltaRot.x);
	//r.deltaRot.y = glm::radians(r.deltaRot.y);
	//r.deltaRot.z = glm::radians(r.deltaRot.z);
}
#ifdef _WIN64
void ComponentGUI(DirectXRenderer& dxr)
{
	ImGui::ColorPicker4("Renderer Color", glm::value_ptr(dxr.vertexColor));
	ImGui::Image((void*)(std::dynamic_pointer_cast<DirectX11ImageGraphicsData>(dxr.diffuseTexture.imageGraphicsData))->srv, ImVec2(100, 100));
}
#endif
