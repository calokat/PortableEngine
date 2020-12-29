#ifdef __EMSCRIPTEN__
#pragma once
#include "IInputSystem.h"
#include <emscripten/html5.h>
#include <map>
class EmscriptenInputSystem :
	public IInputSystem
{
public:
	EmscriptenInputSystem();
	void GetKeyPressed();
	glm::vec2 GetCursorPosition();
	glm::vec2 GetPreviousCursorPosition();
	glm::vec2 GetDeltaCursorPosition();
	bool IsKeyPressed(KeyboardCode kc);
	bool WasKeyPressed(KeyboardCode kc);
	bool IsMouseButtonPressed(MouseButton mb);
	bool WasMouseButtonPressed(MouseButton mb);
private:
	static EM_BOOL MouseCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData);
	static EM_BOOL MouseDownCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData);
	static EM_BOOL MouseUpCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData);
	static EM_BOOL KeyDownCallback(int eventType, const EmscriptenKeyboardEvent* kbEvent, void* userData);
	static EM_BOOL KeyUpCallback(int eventType, const EmscriptenKeyboardEvent* kbEvent, void* userData);
	static InputData current;
	static InputData previous;
	static glm::vec2 deltaCursorPos;
};
#endif
