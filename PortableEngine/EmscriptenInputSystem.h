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
	void RegisterKeyPressFunction(char, std::function<void()>);
	void RegisterRightMouseFunction(std::function<void()>);
	glm::vec2 GetCursorPosition();
	glm::vec2 GetPreviousCursorPosition();
	bool IsKeyPressed(KeyboardCode kc);
	bool WasKeyPressed(KeyboardCode kc);
	bool IsMouseButtonPressed(MouseButton mb);
	bool WasMouseButtonPressed(MouseButton mb);
private:
	static EM_BOOL MouseCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData);
	static EM_BOOL MouseClickCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData);
	static EM_BOOL MouseUpCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData);
	static EM_BOOL KeyDownCallback(int eventType, const EmscriptenKeyboardEvent* kbEvent, void* userData);
	static glm::vec2 cursorPos;
	static glm::vec2 prevCursorPos;
	static std::function<void()> rightClickFunction;
	static std::map<char, std::function<void()>> keyToFunction;
};
#endif
