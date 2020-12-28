#ifdef __EMSCRIPTEN__
#include "EmscriptenInputSystem.h"
#include <iostream>
#include <imgui.h>
InputData EmscriptenInputSystem::current;
InputData EmscriptenInputSystem::previous;
EmscriptenInputSystem::EmscriptenInputSystem()
{
	emscripten_set_keydown_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::KeyDownCallback);
	emscripten_set_keyup_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::KeyUpCallback);
	emscripten_set_mousemove_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseCallback);
	emscripten_set_mousedown_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseClickCallback);
	emscripten_set_mouseup_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseUpCallback);
}

void EmscriptenInputSystem::GetKeyPressed()
{
}
bool EmscriptenInputSystem::IsKeyPressed(KeyboardCode kc)
{
	return current.keys[kc];
}
{
	rightClickFunction = rcFunc;
}
glm::vec2 EmscriptenInputSystem::GetCursorPosition()
{
	return cursorPos;
}
glm::vec2 EmscriptenInputSystem::GetPreviousCursorPosition()
{
	return prevCursorPos;
}
EM_BOOL EmscriptenInputSystem::MouseCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	prevCursorPos = cursorPos;
	cursorPos = glm::vec2(mouseEvent->targetX, mouseEvent->targetY);
	if (mouseEvent->buttons == 2)
	{
		rightClickFunction();
	}
	ImGuiIO& io = ImGui::GetIO();
	return false;
}
EM_BOOL EmscriptenInputSystem::MouseClickCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	ImGuiIO& io = ImGui::GetIO();
	if ((mouseEvent->buttons & 2) == 2)
	{
		io.MouseDown[2] = true;
	}
	if ((mouseEvent->button) == 0)
	{
		io.MouseDown[0] = true;
	}
	return false;
}
EM_BOOL EmscriptenInputSystem::MouseUpCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[0] = false;
	io.MouseDown[2] = false;
	return false;
}
EM_BOOL EmscriptenInputSystem::KeyDownCallback(int eventType, const EmscriptenKeyboardEvent* kbEvent, void* userData)
{
	if (keyToFunction[kbEvent->key[0]])
	{
		keyToFunction[kbEvent->key[0]]();
	}
	return false;
}
#endif