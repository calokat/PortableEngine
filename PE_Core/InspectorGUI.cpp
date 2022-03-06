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
		for (auto texIt = r.textures.begin(); texIt != r.textures.end(); ++texIt)
		{
			char buf[128];
			memcpy(buf, texIt->second.path.c_str(), texIt->second.path.length());
			buf[texIt->second.path.length()] = '\0';
			switch (texIt->first)
			{
				case TextureType::DiffuseTexture:
					ImGui::Text("Diffuse - Path: ");
				break;
				case TextureType::NormalTexture:
					ImGui::Text("Normal - Path: ");
			}
			ImGui::SameLine();
			if (ImGui::InputText(buf, buf, 128))
			{
				texIt->second.path = buf;
				texIt->second.pathChanged = true;
			}
			OpenGLImageGraphicsData* imageGraphicsData = (OpenGLImageGraphicsData*)texIt->second.imageGraphicsData.get();
			ImGui::Image((ImTextureID)imageGraphicsData->texture, ImVec2(100, 100));
		}
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
		for (auto texIt = dxr.textures.begin(); texIt != dxr.textures.end(); ++texIt)
		{
			char buf[128];
			memcpy(buf, texIt->second.path.c_str(), texIt->second.path.length());
			buf[texIt->second.path.length()] = '\0';
			switch (texIt->first)
			{
			case TextureType::DiffuseTexture:
				ImGui::Text("Diffuse - Path: ");
				break;
			case TextureType::NormalTexture:
				ImGui::Text("Normal - Path: ");
			}
			ImGui::SameLine();
			if (ImGui::InputText(buf, buf, 128))
			{
				texIt->second.path = buf;
				texIt->second.pathChanged = true;
			}
			DirectX11ImageGraphicsData* imageGraphicsData = (DirectX11ImageGraphicsData*)texIt->second.imageGraphicsData.get();
			ImGui::Image((ImTextureID)imageGraphicsData->srv, ImVec2(100, 100));
		}
	}
}
#endif
