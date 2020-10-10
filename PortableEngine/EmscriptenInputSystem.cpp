#ifdef __EMSCRIPTEN__
#include "EmscriptenInputSystem.h"

glm::vec2 EmscriptenInputSystem::cursorPos = glm::vec2(0, 0);

std::function<void()> EmscriptenInputSystem::rightClickFunction;

glm::vec2 EmscriptenInputSystem::prevCursorPos = glm::vec2(0, 0);

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
#endif