#include "imgui_impl_emscripten.h"
#include <imgui.h>
bool ImGui_ImplEmscripten_Init()
{
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
	io.BackendPlatformName = "imgui_impl_emscripten";
	
	return true;
}
