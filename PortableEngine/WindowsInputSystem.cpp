#ifdef _WIN64
#include "WindowsInputSystem.h"
#include <ctype.h>
WindowsInputSystem::WindowsInputSystem(HWND h) : hwnd(h)
{
}
void WindowsInputSystem::GetKeyPressed()
{
	POINT mousePos = {};
	GetCursorPos(&mousePos);
	ScreenToClient(hwnd, &mousePos);
	current.cursorPos = glm::vec2(mousePos.x, mousePos.y);

	current.keys[KeyCode::A] = GetAsyncKeyState('A') & 0x8000;
	current.keys[KeyCode::B] = GetAsyncKeyState('B') & 0x8000;
	current.keys[KeyCode::C] = GetAsyncKeyState('C') & 0x8000;
	current.keys[KeyCode::D] = GetAsyncKeyState('D') & 0x8000;
	current.keys[KeyCode::E] = GetAsyncKeyState('E') & 0x8000;
	current.keys[KeyCode::F] = GetAsyncKeyState('F') & 0x8000;
	current.keys[KeyCode::G] = GetAsyncKeyState('G') & 0x8000;
	current.keys[KeyCode::H] = GetAsyncKeyState('H') & 0x8000;
	current.keys[KeyCode::I] = GetAsyncKeyState('I') & 0x8000;
	current.keys[KeyCode::J] = GetAsyncKeyState('J') & 0x8000;
	current.keys[KeyCode::K] = GetAsyncKeyState('K') & 0x8000;
	current.keys[KeyCode::L] = GetAsyncKeyState('L') & 0x8000;
	current.keys[KeyCode::M] = GetAsyncKeyState('M') & 0x8000;
	current.keys[KeyCode::N] = GetAsyncKeyState('N') & 0x8000;
	current.keys[KeyCode::O] = GetAsyncKeyState('O') & 0x8000;
	current.keys[KeyCode::P] = GetAsyncKeyState('P') & 0x8000;
	current.keys[KeyCode::Q] = GetAsyncKeyState('Q') & 0x8000;
	current.keys[KeyCode::R] = GetAsyncKeyState('R') & 0x8000;
	current.keys[KeyCode::S] = GetAsyncKeyState('S') & 0x8000;
	current.keys[KeyCode::T] = GetAsyncKeyState('T') & 0x8000;
	current.keys[KeyCode::U] = GetAsyncKeyState('U') & 0x8000;
	current.keys[KeyCode::V] = GetAsyncKeyState('V') & 0x8000;
	current.keys[KeyCode::W] = GetAsyncKeyState('W') & 0x8000;
	current.keys[KeyCode::X] = GetAsyncKeyState('X') & 0x8000;
	current.keys[KeyCode::Y] = GetAsyncKeyState('Y') & 0x8000;
	current.keys[KeyCode::Z] = GetAsyncKeyState('Z') & 0x8000;
	current.keys[KeyCode::Num0] = GetAsyncKeyState('0') & 0x8000;
	current.keys[KeyCode::Num1] = GetAsyncKeyState('1') & 0x8000;
	current.keys[KeyCode::Num2] = GetAsyncKeyState('2') & 0x8000;
	current.keys[KeyCode::Num3] = GetAsyncKeyState('3') & 0x8000;
	current.keys[KeyCode::Num4] = GetAsyncKeyState('4') & 0x8000;
	current.keys[KeyCode::Num5] = GetAsyncKeyState('5') & 0x8000;
	current.keys[KeyCode::Num6] = GetAsyncKeyState('6') & 0x8000;
	current.keys[KeyCode::Num7] = GetAsyncKeyState('7') & 0x8000;
	current.keys[KeyCode::Num8] = GetAsyncKeyState('8') & 0x8000;
	current.keys[KeyCode::Num9] = GetAsyncKeyState('9') & 0x8000;
	current.keys[KeyCode::LCtrl] = GetAsyncKeyState(VK_LCONTROL) & 0x8000;
	current.keys[KeyCode::RCtrl] = GetAsyncKeyState(VK_RCONTROL) & 0x8000;
	current.keys[KeyCode::LShift] = GetAsyncKeyState(VK_LSHIFT) & 0x8000;
	current.keys[KeyCode::RShift] = GetAsyncKeyState(VK_RSHIFT) & 0x8000;
	current.keys[KeyCode::ForwardSlash] = GetAsyncKeyState('/') & 0x8000;
	current.keys[KeyCode::BackSlash] = GetAsyncKeyState('\\') & 0x8000;
	current.keys[KeyCode::Enter] = GetAsyncKeyState('\n') & 0x8000;
	current.keys[KeyCode::Escape] = GetAsyncKeyState(VK_ESCAPE) & 0x8000;
	
	current.mouseButtons[MouseButton::Left] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	current.mouseButtons[MouseButton::Middle] = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
	current.mouseButtons[MouseButton::Right] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;

	previous = current;
}

glm::vec2 WindowsInputSystem::GetCursorPosition()
{
	return current.cursorPos;
}

glm::vec2 WindowsInputSystem::GetPreviousCursorPosition()
{
	return previous.cursorPos;
}
bool WindowsInputSystem::IsKeyPressed(KeyCode kc)
{
	return current.keys[kc];
}
bool WindowsInputSystem::WasKeyPressed(KeyCode kc)
{
	return previous.keys[kc];
}
bool WindowsInputSystem::IsMouseButtonPressed(MouseButton mb)
{
	return current.mouseButtons[mb];
}
bool WindowsInputSystem::WasMouseButtonPressed(MouseButton mb)
{
	return previous.mouseButtons[mb];
}
#endif