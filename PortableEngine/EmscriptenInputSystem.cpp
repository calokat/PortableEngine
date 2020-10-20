#ifdef __EMSCRIPTEN__
#include "EmscriptenInputSystem.h"
#include <iostream>
#include <imgui.h>
glm::vec2 EmscriptenInputSystem::cursorPos = glm::vec2(0, 0);

std::function<void()> EmscriptenInputSystem::rightClickFunction;

glm::vec2 EmscriptenInputSystem::prevCursorPos = glm::vec2(0, 0);

std::map<char, std::function<void()>> EmscriptenInputSystem::keyToFunction;

EmscriptenInputSystem::EmscriptenInputSystem()
{
	emscripten_set_keydown_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::KeyDownCallback);
	emscripten_set_mousemove_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseCallback);
	emscripten_set_mousedown_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseClickCallback);
	emscripten_set_mouseup_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseUpCallback);
}

void EmscriptenInputSystem::GetKeyPressed()
{
}
void EmscriptenInputSystem::RegisterKeyPressFunction(char key, std::function<void()> func)
{
	keyToFunction.emplace(key, func);
}
void EmscriptenInputSystem::RegisterRightMouseFunction(std::function<void()> rcFunc)
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