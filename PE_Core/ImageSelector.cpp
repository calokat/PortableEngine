#include "ImageSelector.h"
#include <imgui.h>
#include "Pathfinder.h"

bool ImageSelector::Render(PE::String& str, const char* textureType)
{
	bool result = true;
	if (ImGui::BeginMenu(textureType))
	{
		if (ImGui::MenuItem("Rock"))
		{
			str = PE::String("../Assets/Images/rock.png");
		}
		else if (ImGui::MenuItem("Cushion"))
		{
			str = PE::String("../Assets/Images/cushion.png");
		}
		else if (ImGui::MenuItem("Rock Normal Map"))
		{
			str = PE::String("../Assets/Images/Normal/rock_normals.png");
		}
		else if (ImGui::MenuItem("Cushion Normal Map"))
		{
			str = PE::String("../Assets/Images/Normal/cushion_normals.png");
		}
		else
		{
			result = false;
		}
		ImGui::EndMenu();
	}
	else
	{
		result = false;
	}
	return result;
}
