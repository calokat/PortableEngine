#ifdef _WIN64
#include "WindowsInputSystem.h"
#include <ctype.h>
WindowsInputSystem::WindowsInputSystem(HWND h) : hwnd(h)
{
	cursor = GetCursor();
}
void WindowsInputSystem::GetKeyPressed()
{
	previous = current;

	POINT mousePos = {};
	GetCursorPos(&mousePos);
	//ScreenToClient(hwnd, &mousePos);
	current.cursorPos = glm::vec2(mousePos.x, mousePos.y);

	current.keys[KeyboardCode::A] = GetAsyncKeyState('A') & 0x8000;
	current.keys[KeyboardCode::B] = GetAsyncKeyState('B') & 0x8000;
	current.keys[KeyboardCode::C] = GetAsyncKeyState('C') & 0x8000;
	current.keys[KeyboardCode::D] = GetAsyncKeyState('D') & 0x8000;
	current.keys[KeyboardCode::E] = GetAsyncKeyState('E') & 0x8000;
	current.keys[KeyboardCode::F] = GetAsyncKeyState('F') & 0x8000;
	current.keys[KeyboardCode::G] = GetAsyncKeyState('G') & 0x8000;
	current.keys[KeyboardCode::H] = GetAsyncKeyState('H') & 0x8000;
	current.keys[KeyboardCode::I] = GetAsyncKeyState('I') & 0x8000;
	current.keys[KeyboardCode::J] = GetAsyncKeyState('J') & 0x8000;
	current.keys[KeyboardCode::K] = GetAsyncKeyState('K') & 0x8000;
	current.keys[KeyboardCode::L] = GetAsyncKeyState('L') & 0x8000;
	current.keys[KeyboardCode::M] = GetAsyncKeyState('M') & 0x8000;
	current.keys[KeyboardCode::N] = GetAsyncKeyState('N') & 0x8000;
	current.keys[KeyboardCode::O] = GetAsyncKeyState('O') & 0x8000;
	current.keys[KeyboardCode::P] = GetAsyncKeyState('P') & 0x8000;
	current.keys[KeyboardCode::Q] = GetAsyncKeyState('Q') & 0x8000;
	current.keys[KeyboardCode::R] = GetAsyncKeyState('R') & 0x8000;
	current.keys[KeyboardCode::S] = GetAsyncKeyState('S') & 0x8000;
	current.keys[KeyboardCode::T] = GetAsyncKeyState('T') & 0x8000;
	current.keys[KeyboardCode::U] = GetAsyncKeyState('U') & 0x8000;
	current.keys[KeyboardCode::V] = GetAsyncKeyState('V') & 0x8000;
	current.keys[KeyboardCode::W] = GetAsyncKeyState('W') & 0x8000;
	current.keys[KeyboardCode::X] = GetAsyncKeyState('X') & 0x8000;
	current.keys[KeyboardCode::Y] = GetAsyncKeyState('Y') & 0x8000;
	current.keys[KeyboardCode::Z] = GetAsyncKeyState('Z') & 0x8000;
	current.keys[KeyboardCode::Num0] = GetAsyncKeyState('0') & 0x8000;
	current.keys[KeyboardCode::Num1] = GetAsyncKeyState('1') & 0x8000;
	current.keys[KeyboardCode::Num2] = GetAsyncKeyState('2') & 0x8000;
	current.keys[KeyboardCode::Num3] = GetAsyncKeyState('3') & 0x8000;
	current.keys[KeyboardCode::Num4] = GetAsyncKeyState('4') & 0x8000;
	current.keys[KeyboardCode::Num5] = GetAsyncKeyState('5') & 0x8000;
	current.keys[KeyboardCode::Num6] = GetAsyncKeyState('6') & 0x8000;
	current.keys[KeyboardCode::Num7] = GetAsyncKeyState('7') & 0x8000;
	current.keys[KeyboardCode::Num8] = GetAsyncKeyState('8') & 0x8000;
	current.keys[KeyboardCode::Num9] = GetAsyncKeyState('9') & 0x8000;
	current.keys[KeyboardCode::LCtrl] = GetAsyncKeyState(VK_LCONTROL) & 0x8000;
	current.keys[KeyboardCode::RCtrl] = GetAsyncKeyState(VK_RCONTROL) & 0x8000;
	current.keys[KeyboardCode::LShift] = GetAsyncKeyState(VK_LSHIFT) & 0x8000;
	current.keys[KeyboardCode::RShift] = GetAsyncKeyState(VK_RSHIFT) & 0x8000;
	current.keys[KeyboardCode::ForwardSlash] = GetAsyncKeyState('/') & 0x8000;
	current.keys[KeyboardCode::BackSlash] = GetAsyncKeyState('\\') & 0x8000;
	current.keys[KeyboardCode::Enter] = GetAsyncKeyState('\n') & 0x8000;
	current.keys[KeyboardCode::Esc] = GetAsyncKeyState(VK_ESCAPE) & 0x8000;
	
	current.mouseButtons[MouseButton::Left] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	current.mouseButtons[MouseButton::Middle] = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
	current.mouseButtons[MouseButton::Right] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
	if (current.mouseButtons[MouseButton::Right])
	{
		RECT winRect;
		GetWindowRect(hwnd, &winRect);

		if (current.cursorPos.x > winRect.right)
		{
			SetCursorPos(winRect.left, mousePos.y);
			previous.cursorPos = { winRect.left, mousePos.y };
			current.cursorPos = { winRect.left, mousePos.y };
		}
		if (current.cursorPos.x < winRect.left)
		{
			SetCursorPos(winRect.right, mousePos.y);
			previous.cursorPos = { winRect.right, mousePos.y };
			current.cursorPos = { winRect.right, mousePos.y };
		}
		if (current.cursorPos.y > winRect.bottom)
		{
			SetCursorPos(mousePos.x, winRect.top);
			previous.cursorPos = { mousePos.x, winRect.top };
			current.cursorPos = { mousePos.x, winRect.top };
		}
		if (current.cursorPos.y < winRect.top)
		{
			SetCursorPos(mousePos.x, winRect.bottom);
			previous.cursorPos = { mousePos.x, winRect.bottom };
			current.cursorPos = { mousePos.x, winRect.bottom };
		}

	}
	else
	{
	}
}

glm::vec2 WindowsInputSystem::GetCursorPosition()
{
	return current.cursorPos;
}

glm::vec2 WindowsInputSystem::GetPreviousCursorPosition()
{
	return previous.cursorPos;
}
glm::vec2 WindowsInputSystem::GetDeltaCursorPosition()
{
	return current.cursorPos - previous.cursorPos;
}
bool WindowsInputSystem::IsKeyPressed(KeyboardCode kc)
{
	return current.keys[kc];
}
bool WindowsInputSystem::WasKeyPressed(KeyboardCode kc)
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
bool WindowsInputSystem::WasMouseButtonClicked(MouseButton mb)
{
	return current.mouseButtons[MouseButton::Left] && !previous.mouseButtons[MouseButton::Left];
}
#endif