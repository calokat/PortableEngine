#pragma once
#include <functional>
#include <glm/glm.hpp>

enum KeyboardCode {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, LCtrl, RCtrl, LShift, RShift, ForwardSlash, BackSlash, Enter, Esc};
enum MouseButton {Left, Right, Middle};
struct InputData
{
	bool keys[44];
	bool mouseButtons[3];
	glm::vec2 cursorPos;
};

class IInputSystem
{
public:
	virtual void GetKeyPressed() = 0;
	virtual glm::vec2 GetCursorPosition() = 0;
	virtual glm::vec2 GetPreviousCursorPosition() = 0;
	virtual glm::vec2 GetDeltaCursorPosition() = 0;
	virtual bool IsKeyPressed(KeyboardCode kc) = 0;
	virtual bool WasKeyPressed(KeyboardCode kc) = 0;
	virtual bool IsMouseButtonPressed(MouseButton mb) = 0;
	virtual bool WasMouseButtonPressed(MouseButton mb) = 0;
	virtual ~IInputSystem() {};
protected:
	// InputData current;
	// InputData previous;
};