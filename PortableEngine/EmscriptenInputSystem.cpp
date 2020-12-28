#ifdef __EMSCRIPTEN__
#include "EmscriptenInputSystem.h"
#include <iostream>
#include <imgui.h>
InputData EmscriptenInputSystem::current;
InputData EmscriptenInputSystem::previous;
glm::vec2 EmscriptenInputSystem::deltaCursorPos;

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

glm::vec2 EmscriptenInputSystem::GetDeltaCursorPosition()
{
	// Hack: deltaCursorPos does not become (0, 0) when the mouse stops moving. When the mouse stops moving, MouseCallback is not called,
	// so deltaCursorPos is not assigned (0, 0) and it retains its non-zero value from the previous frame. This hack ensures that 
	// deltaCursorPos is (0, 0) when the mouse is not moving by manually zeroing it and letting that value be overridden in MouseCallback
	// when called.
	glm::vec2 returnValue = deltaCursorPos;
	deltaCursorPos = glm::vec2(0, 0);
	return returnValue;
}
EM_BOOL EmscriptenInputSystem::MouseCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	// previous = current;
	current.cursorPos = glm::vec2(mouseEvent->targetX, mouseEvent->targetY);
	deltaCursorPos = glm::vec2(mouseEvent->movementX, mouseEvent->movementY);
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
	if ((mouseEvent->buttons & 2) != 2)
	{
		io.MouseDown[2] = false;
		current.mouseButtons[MouseButton::Right] = false;
		emscripten_exit_pointerlock();
	}
	if ((mouseEvent->button) == 0)
	{
		io.MouseDown[0] = false;
		current.mouseButtons[MouseButton::Left] = false;
	}
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