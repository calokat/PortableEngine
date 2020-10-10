#ifdef __EMSCRIPTEN__
#include "EmscriptenInputSystem.h"
#include <iostream>
glm::vec2 EmscriptenInputSystem::cursorPos = glm::vec2(0, 0);

std::function<void()> EmscriptenInputSystem::rightClickFunction;

glm::vec2 EmscriptenInputSystem::prevCursorPos = glm::vec2(0, 0);

EmscriptenInputSystem::EmscriptenInputSystem()
{
	emscripten_set_mousemove_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseCallback);
	emscripten_set_click_callback("canvas.emscripten", nullptr, false, EmscriptenInputSystem::MouseClickCallback);
}

void EmscriptenInputSystem::GetKeyPressed()
{
}
void EmscriptenInputSystem::RegisterKeyPressFunction(char, std::function<void()>)
{
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
	printf("Mouse button: %i\n", mouseEvent->buttons);
	if (mouseEvent->buttons == 2)
	{
		rightClickFunction();
	}
	return true;
}
EM_BOOL EmscriptenInputSystem::MouseClickCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	printf("Mouse button: %i\n", mouseEvent->buttons);
	return true;
}
#endif