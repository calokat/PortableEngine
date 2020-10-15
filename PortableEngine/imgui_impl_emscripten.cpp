#include "imgui_impl_emscripten.h"
#include <imgui.h>
#include <emscripten.h>
double g_Time;
GameWindow* window;
IInputSystem* input;

void ImGui_ImplEmscripten_UpdateMousePos()
{
	ImGuiIO& io = ImGui::GetIO();
	glm::vec2 mousePos = input->GetCursorPosition();
	io.MousePos = { mousePos.x, mousePos.y };
}


bool ImGui_ImplEmscripten_Init(GameWindow* win, IInputSystem* in)
{
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
	io.BackendPlatformName = "imgui_impl_emscripten";
	window = win;
	input = in;

	return true;
}

void ImGui_ImplEmscripten_Shutdown()
{
}

void ImGui_ImplEmscripten_NewFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");
	double now = emscripten_get_now();
	io.DeltaTime = now - g_Time;
	g_Time = now;

	io.DisplaySize = ImVec2((float)(window->width), (float)(window->height));

	ImGui_ImplEmscripten_UpdateMousePos();
}

