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

bool EmscriptenInputSystem::WasKeyPressed(KeyboardCode kc)
{
	return previous.keys[kc];
}

bool EmscriptenInputSystem::IsMouseButtonPressed(MouseButton mb)
{
	return current.mouseButtons[mb];
}

bool EmscriptenInputSystem::WasMouseButtonPressed(MouseButton mb)
{
	return previous.mouseButtons[mb];
}

glm::vec2 EmscriptenInputSystem::GetCursorPosition()
{
	return current.cursorPos;
}
glm::vec2 EmscriptenInputSystem::GetPreviousCursorPosition()
{
	return previous.cursorPos;
}
EM_BOOL EmscriptenInputSystem::MouseCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	// previous = current;
	current.cursorPos = glm::vec2(mouseEvent->targetX, mouseEvent->targetY);
	printf("Is RMB Down? %i\n", current.mouseButtons[MouseButton::Right]);
	return false;
}
EM_BOOL EmscriptenInputSystem::MouseClickCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	ImGuiIO& io = ImGui::GetIO();
	if ((mouseEvent->buttons & 2) == 2)
	{
		io.MouseDown[2] = true;
		emscripten_request_pointerlock("canvas.emscripten", true);
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
		emscripten_exit_pointerlock();
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