#include "InspectorGUI.h"
#include <imgui.h>
#include "TransformSystem.h"
#include "OpenGLImageGraphicsData.h"
#ifdef _WIN64
#include "DirectX11ImageGraphicsData.h"
#endif
void ComponentGUI(const Name& n)
{
	ImGui::Text(n.nameString.c_str());
}
void ComponentGUI(Transform& t)
{
	//TransformSystem::CalculatePosition(t);
	if (ImGui::DragFloat3("Position", glm::value_ptr(t.position), .01f))
	{
		//t.position = potentialNewPos;
		TransformSystem::CalculateWorldMatrix(&t);
	}
	//TransformSystem::CalculateEulerAngles(t);
	glm::vec3 degrees = glm::degrees(t.rotation);
	if (ImGui::DragFloat3("Rotation", glm::value_ptr(degrees)))
	{
		t.rotation = glm::radians(degrees);
		TransformSystem::SetOrientation(t.rotation, &t);
		//TransformSystem::CalculateWorldMatrix(&t);
	}
	//TransformSystem::CalculateScale(t);
	if (ImGui::DragFloat3("Scale", glm::value_ptr(t.scale), .01f))
	{
		TransformSystem::CalculateWorldMatrix(&t);
	}
	glm::vec3 globalRot = glm::degrees(glm::eulerAngles(t.orientation));
	ImGui::Text("Global Rotation:");
	ImGui::Text("X: %f", globalRot.x);
	ImGui::Text("Y: %f", globalRot.y);
	ImGui::Text("Z: %f", globalRot.z);
	//t.rotation = TransformSystem::CalculateEulerAngles(t);
	//ImGui::DragFloat3("Rotation", glm::value_ptr(t.rotation));
}

void ComponentGUI(GLRenderer& r)
{
	ImGui::ColorPicker4("Renderer Color", glm::value_ptr(r.vertexColor));
	if (r.shaderProgram.propertyFlags & ShaderProgramProperties::Textured)
	{
		char buf[128];
		memcpy(buf, r.diffuseTexture.path.c_str(), r.diffuseTexture.path.length());
		buf[r.diffuseTexture.path.length()] = '\0';
		ImGui::Text("Path: ");
		ImGui::SameLine();
		if (ImGui::InputText(" ", buf, 128))
		{
			r.diffuseTexture.path = buf;
			r.diffuseTexture.pathChanged = true;
		}
		ImGui::Image((void*)(std::dynamic_pointer_cast<OpenGLImageGraphicsData>(r.diffuseTexture.imageGraphicsData))->texture, ImVec2(100, 100));
	}
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
	if (dxr.shaderProgram.shaderType & ShaderProgramProperties::Textured)
	{
		ImGui::Image((void*)(std::dynamic_pointer_cast<DirectX11ImageGraphicsData>(dxr.diffuseTexture.imageGraphicsData))->srv, ImVec2(100, 100));
	}
}
#endif
