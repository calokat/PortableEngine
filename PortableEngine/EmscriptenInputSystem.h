#ifdef __EMSCRIPTEN__
#pragma once
#include "IInputSystem.h"
#include <emscripten/html5.h>
class EmscriptenInputSystem :
	public IInputSystem
{
public:
	void GetKeyPressed();
	void RegisterKeyPressFunction(char, std::function<void()>);
	void RegisterRightMouseFunction(std::function<void()>);
	glm::vec2 GetCursorPosition();
private:
	static EM_BOOL MouseCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData);
	static glm::vec2 cursorPos;
	static glm::vec2 prevCursorPos;
	static std::function<void()> rightClickFunction;
};
#endif
