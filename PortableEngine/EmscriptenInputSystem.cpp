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
	previous = current;
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
	return false;
}
EM_BOOL EmscriptenInputSystem::MouseClickCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	ImGuiIO& io = ImGui::GetIO();
	if ((mouseEvent->buttons & 2) == 2)
	{
		io.MouseDown[2] = true;
		current.mouseButtons[MouseButton::Right] = true;
		emscripten_request_pointerlock("canvas.emscripten", true);
	}
	if ((mouseEvent->button) == 0)
	{
		io.MouseDown[0] = true;
		current.mouseButtons[MouseButton::Left] = true;
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
	current.keys[KeyboardCode::A] = strcmp(kbEvent->key, "a") == 0;
	current.keys[KeyboardCode::B] = strcmp(kbEvent->key, "b") == 0;
	current.keys[KeyboardCode::C] = strcmp(kbEvent->key, "c") == 0;
	current.keys[KeyboardCode::D] = strcmp(kbEvent->key, "d") == 0;
	current.keys[KeyboardCode::E] = strcmp(kbEvent->key, "e") == 0;
	current.keys[KeyboardCode::F] = strcmp(kbEvent->key, "f") == 0;
	current.keys[KeyboardCode::G] = strcmp(kbEvent->key, "g") == 0;
	current.keys[KeyboardCode::H] = strcmp(kbEvent->key, "h") == 0;
	current.keys[KeyboardCode::I] = strcmp(kbEvent->key, "i") == 0;
	current.keys[KeyboardCode::J] = strcmp(kbEvent->key, "j") == 0;
	current.keys[KeyboardCode::K] = strcmp(kbEvent->key, "k") == 0;
	current.keys[KeyboardCode::L] = strcmp(kbEvent->key, "l") == 0;
	current.keys[KeyboardCode::M] = strcmp(kbEvent->key, "m") == 0;
	current.keys[KeyboardCode::N] = strcmp(kbEvent->key, "n") == 0;
	current.keys[KeyboardCode::O] = strcmp(kbEvent->key, "o") == 0;
	current.keys[KeyboardCode::P] = strcmp(kbEvent->key, "p") == 0;
	current.keys[KeyboardCode::Q] = strcmp(kbEvent->key, "q") == 0;
	current.keys[KeyboardCode::R] = strcmp(kbEvent->key, "r") == 0;	
	current.keys[KeyboardCode::S] = strcmp(kbEvent->key, "s") == 0;
	current.keys[KeyboardCode::T] = strcmp(kbEvent->key, "t") == 0;
	current.keys[KeyboardCode::U] = strcmp(kbEvent->key, "u") == 0;
	current.keys[KeyboardCode::V] = strcmp(kbEvent->key, "v") == 0;
	current.keys[KeyboardCode::W] = strcmp(kbEvent->key, "w") == 0;
	current.keys[KeyboardCode::X] = strcmp(kbEvent->key, "x") == 0;
	current.keys[KeyboardCode::Y] = strcmp(kbEvent->key, "y") == 0;	
	current.keys[KeyboardCode::Z] = strcmp(kbEvent->key, "z") == 0;
	current.keys[KeyboardCode::Num0] = strcmp(kbEvent->key, "0") == 0;
	current.keys[KeyboardCode::Num1] = strcmp(kbEvent->key, "1") == 0;
	current.keys[KeyboardCode::Num2] = strcmp(kbEvent->key, "2") == 0;
	current.keys[KeyboardCode::Num3] = strcmp(kbEvent->key, "3") == 0;
	current.keys[KeyboardCode::Num4] = strcmp(kbEvent->key, "4") == 0;
	current.keys[KeyboardCode::Num5] = strcmp(kbEvent->key, "5") == 0;
	current.keys[KeyboardCode::Num6] = strcmp(kbEvent->key, "6") == 0;
	current.keys[KeyboardCode::Num7] = strcmp(kbEvent->key, "7") == 0;
	current.keys[KeyboardCode::Num8] = strcmp(kbEvent->key, "8") == 0;
	current.keys[KeyboardCode::Num9] = strcmp(kbEvent->key, "9") == 0;
	current.keys[KeyboardCode::LCtrl] = strcmp(kbEvent->key, "Control") == 0;
	current.keys[KeyboardCode::RCtrl] = strcmp(kbEvent->key, "Control") == 0;
	current.keys[KeyboardCode::LShift] = strcmp(kbEvent->key, "Shift") == 0;
	current.keys[KeyboardCode::RShift] = strcmp(kbEvent->key, "Shift") == 0;
	current.keys[KeyboardCode::ForwardSlash] = strcmp(kbEvent->key, "/") == 0;
	current.keys[KeyboardCode::BackSlash] = strcmp(kbEvent->key, "\\") == 0;
	current.keys[KeyboardCode::Enter] = strcmp(kbEvent->key, "Enter") == 0;
	current.keys[KeyboardCode::Esc] = strcmp(kbEvent->key, "Escape") == 0;
	return false;
}

EM_BOOL EmscriptenInputSystem::KeyUpCallback(int eventType, const EmscriptenKeyboardEvent* kbEvent, void* userData)
{
	current.keys[KeyboardCode::A] = (current.keys[KeyboardCode::A] && strcmp(kbEvent->key, "a") != 0);
	current.keys[KeyboardCode::B] = (current.keys[KeyboardCode::B] && strcmp(kbEvent->key, "b") != 0);
	current.keys[KeyboardCode::C] = (current.keys[KeyboardCode::C] && strcmp(kbEvent->key, "c") != 0);
	current.keys[KeyboardCode::D] = (current.keys[KeyboardCode::D] && strcmp(kbEvent->key, "d") != 0);
	current.keys[KeyboardCode::E] = (current.keys[KeyboardCode::E] && strcmp(kbEvent->key, "e") != 0);
	current.keys[KeyboardCode::F] = (current.keys[KeyboardCode::F] && strcmp(kbEvent->key, "f") != 0);
	current.keys[KeyboardCode::G] = (current.keys[KeyboardCode::G] && strcmp(kbEvent->key, "g") != 0);
	current.keys[KeyboardCode::H] = (current.keys[KeyboardCode::H] && strcmp(kbEvent->key, "h") != 0);
	current.keys[KeyboardCode::I] = (current.keys[KeyboardCode::I] && strcmp(kbEvent->key, "i") != 0);
	current.keys[KeyboardCode::J] = (current.keys[KeyboardCode::J] && strcmp(kbEvent->key, "j") != 0);
	current.keys[KeyboardCode::K] = (current.keys[KeyboardCode::K] && strcmp(kbEvent->key, "k") != 0);
	current.keys[KeyboardCode::L] = (current.keys[KeyboardCode::L] && strcmp(kbEvent->key, "l") != 0);
	current.keys[KeyboardCode::M] = (current.keys[KeyboardCode::M] && strcmp(kbEvent->key, "m") != 0);
	current.keys[KeyboardCode::N] = (current.keys[KeyboardCode::N] && strcmp(kbEvent->key, "n") != 0);
	current.keys[KeyboardCode::O] = (current.keys[KeyboardCode::O] && strcmp(kbEvent->key, "o") != 0);
	current.keys[KeyboardCode::P] = (current.keys[KeyboardCode::P] && strcmp(kbEvent->key, "p") != 0);
	current.keys[KeyboardCode::Q] = (current.keys[KeyboardCode::Q] && strcmp(kbEvent->key, "q") != 0);
	current.keys[KeyboardCode::R] = (current.keys[KeyboardCode::R] && strcmp(kbEvent->key, "r") != 0);	
	current.keys[KeyboardCode::S] = (current.keys[KeyboardCode::S] && strcmp(kbEvent->key, "s") != 0);
	current.keys[KeyboardCode::T] = (current.keys[KeyboardCode::T] && strcmp(kbEvent->key, "t") != 0);
	current.keys[KeyboardCode::U] = (current.keys[KeyboardCode::U] && strcmp(kbEvent->key, "u") != 0);
	current.keys[KeyboardCode::V] = (current.keys[KeyboardCode::V] && strcmp(kbEvent->key, "v") != 0);
	current.keys[KeyboardCode::W] = (current.keys[KeyboardCode::W] && strcmp(kbEvent->key, "w") != 0);
	current.keys[KeyboardCode::X] = (current.keys[KeyboardCode::X] && strcmp(kbEvent->key, "x") != 0);
	current.keys[KeyboardCode::Y] = (current.keys[KeyboardCode::Y] && strcmp(kbEvent->key, "y") != 0);	
	current.keys[KeyboardCode::Z] = (current.keys[KeyboardCode::Z] && strcmp(kbEvent->key, "z") != 0);
	current.keys[KeyboardCode::Num0] = (current.keys[KeyboardCode::Num0] && strcmp(kbEvent->key, "0") != 0);
	current.keys[KeyboardCode::Num1] = (current.keys[KeyboardCode::Num1] && strcmp(kbEvent->key, "1") != 0);
	current.keys[KeyboardCode::Num2] = (current.keys[KeyboardCode::Num2] && strcmp(kbEvent->key, "2") != 0);
	current.keys[KeyboardCode::Num3] = (current.keys[KeyboardCode::Num3] && strcmp(kbEvent->key, "3") != 0);
	current.keys[KeyboardCode::Num4] = (current.keys[KeyboardCode::Num4] && strcmp(kbEvent->key, "4") != 0);
	current.keys[KeyboardCode::Num5] = (current.keys[KeyboardCode::Num5] && strcmp(kbEvent->key, "5") != 0);
	current.keys[KeyboardCode::Num6] = (current.keys[KeyboardCode::Num6] && strcmp(kbEvent->key, "6") != 0);
	current.keys[KeyboardCode::Num7] = (current.keys[KeyboardCode::Num7] && strcmp(kbEvent->key, "7") != 0);
	current.keys[KeyboardCode::Num8] = (current.keys[KeyboardCode::Num8] && strcmp(kbEvent->key, "8") != 0);
	current.keys[KeyboardCode::Num9] = (current.keys[KeyboardCode::Num9] && strcmp(kbEvent->key, "9") != 0);
	current.keys[KeyboardCode::LCtrl] = (current.keys[KeyboardCode::LCtrl] && strcmp(kbEvent->key, "Control") != 0);
	current.keys[KeyboardCode::RCtrl] = (current.keys[KeyboardCode::RCtrl] && strcmp(kbEvent->key, "Control") != 0);
	current.keys[KeyboardCode::LShift] = (current.keys[KeyboardCode::LShift] && strcmp(kbEvent->key, "Shift") != 0);
	current.keys[KeyboardCode::RShift] = (current.keys[KeyboardCode::RShift] && strcmp(kbEvent->key, "Shift") != 0);
	current.keys[KeyboardCode::ForwardSlash] = (current.keys[KeyboardCode::ForwardSlash] && strcmp(kbEvent->key, "/") != 0);
	current.keys[KeyboardCode::BackSlash] = (current.keys[KeyboardCode::BackSlash] && strcmp(kbEvent->key, "\\") != 0);
	current.keys[KeyboardCode::Enter] = (current.keys[KeyboardCode::Enter] && strcmp(kbEvent->key, "Enter") != 0);
	current.keys[KeyboardCode::Esc] = (current.keys[KeyboardCode::Esc] && strcmp(kbEvent->key, "Escape") != 0);
	return false;
}
#endif