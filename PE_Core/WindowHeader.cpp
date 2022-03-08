#include "WindowHeader.h"
#include <imgui.h>
#include "MeshLoaderSystem.h"
#include "TransformSystem.h"
#include "misc_components.h"
#include "AABBSystem.h"
const char* WindowHeader::Render()
{
	ImGui::BeginMainMenuBar();
	char* res = nullptr;
	if (ImGui::BeginMenu("Import Mesh"))
	{
		if (ImGui::MenuItem("Cube"))
		{
			res = "../../Assets/Models/cube.obj";
		}
		if (ImGui::MenuItem("Sphere"))
		{
			res = "../../Assets/Models/sphere.obj";
		}
		if (ImGui::MenuItem("Cone"))
		{
			res = "../../Assets/Models/cone.obj";
		}
		if (ImGui::MenuItem("Torus"))
		{
			res = "../../Assets/Models/torus.obj";
		}
		if (ImGui::MenuItem("Helix"))
		{
			res = "../../Assets/Models/helix.obj";
		}
		if (ImGui::MenuItem("Cylinder"))
		{
			res = "../../Assets/Models/cylinder.obj";
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	return res;
}
