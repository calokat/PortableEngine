#ifdef __EMSCRIPTEN__
#include "EmscriptenInputSystem.h"

glm::vec2 EmscriptenInputSystem::cursorPos = glm::vec2(0, 0);

std::function<void()> EmscriptenInputSystem::rightClickFunction;

glm::vec2 EmscriptenInputSystem::prevCursorPos = glm::vec2(0, 0);

EmscriptenInputSystem::EmscriptenInputSystem()
{
	emscripten_set_mousedown_callback("canvas.emscripten", nullptr, true, EmscriptenInputSystem::MouseClickCallback);
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
EM_BOOL EmscriptenInputSystem::MouseCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	prevCursorPos = cursorPos;
	cursorPos = glm::vec2(mouseEvent->targetX, mouseEvent->targetY);
	return true;
}
EM_BOOL EmscriptenInputSystem::MouseClickCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	if (mouseEvent->button == 2)
	{
		rightClickFunction();
	}
	return true;
}
#endif