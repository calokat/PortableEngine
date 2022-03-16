#include "InspectorGUI.h"
#include <imgui.h>
#include "TransformSystem.h"
#include "ImageSelector.h"
#include "Pathfinder.h"
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
			//size_t pathLength = strlen(texIt->second.path.GetData());
			//memcpy(buf, texIt->second.path.GetData(), pathLength);
			//buf[pathLength] = '\0';
			char* textureTypeString = "Engine error";
			switch (texIt->first)
			{
				case TextureType::DiffuseTexture:
					textureTypeString = "Select Diffuse type";
				break;
				case TextureType::NormalTexture:
					textureTypeString = "Select Normal type";
					break;
			}
			PE::String partialPath;
			if (ImageSelector().Render(partialPath, textureTypeString))
			{
				PE::String fullPath = partialPath;
				texIt->second.path = std::move(fullPath);
				texIt->second.pathChanged = true;
			}
			OpenGLImageGraphicsData* imageGraphicsData = (OpenGLImageGraphicsData*)texIt->second.imageGraphicsData.get();
			ImGui::Image((ImTextureID)imageGraphicsData->texture, ImVec2(100, 100));
			//PE::String partialPath = PE::String(ImageSelector().Render());
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
void ComponentGUI(PointLight& pl)
{
	ImGui::ColorPicker4("Ambient Color", glm::value_ptr(pl.AmbientColor));
	ImGui::ColorPicker4("Diffuse Color", glm::value_ptr(pl.DiffuseColor));
	ImGui::DragFloat("Intensity", &pl.intensity, .01f, 0);
}
void ComponentGUI(DirectionalLight& dl)
{
	ImGui::ColorPicker4("Ambient Color", glm::value_ptr(dl.AmbientColor));
	ImGui::ColorPicker4("Diffuse Color", glm::value_ptr(dl.DiffuseColor));
}

void ComponentGUI(SpotLight& sl)
{
	ImGui::ColorPicker4("Ambient Color", glm::value_ptr(sl.AmbientColor));
	ImGui::ColorPicker4("Diffuse Color", glm::value_ptr(sl.DiffuseColor));
	ImGui::DragFloat("Intensity", &sl.Intensity, .01f, 0);
	ImGui::DragFloat("Angle", &sl.Angle, .01f, 0);
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
			size_t pathLength = strlen(texIt->second.path.GetData());
			memcpy(buf, texIt->second.path.GetData(), pathLength);
			buf[pathLength] = '\0';
			char* textureTypeString = "Engine error";
			switch (texIt->first)
			{
			case TextureType::DiffuseTexture:
				textureTypeString = "Diffuse";
				break;
			case TextureType::NormalTexture:
				textureTypeString = "Normal";
				break;
			}
			PE::String partialPath;
			if (ImageSelector().Render(partialPath, textureTypeString))
			{
				PE::String fullPath = partialPath;
				texIt->second.path = std::move(fullPath);
				texIt->second.pathChanged = true;
			}
			DirectX11ImageGraphicsData* imageGraphicsData = (DirectX11ImageGraphicsData*)texIt->second.imageGraphicsData.get();
			ImGui::Image((ImTextureID)imageGraphicsData->srv, ImVec2(100, 100));
		}
	}
}
#endif
