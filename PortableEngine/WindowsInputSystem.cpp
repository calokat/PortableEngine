#include "WindowsInputSystem.h"
void WindowsInputSystem::GetKeyPressed()
{
	for (std::_Tree_const_iterator it = keyToFunction.begin(); it != keyToFunction.end(); ++it)
	{
		if (GetAsyncKeyState(it->first) & 0x8000)
		{
			it->second();
		}
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		rightClickFunction();
	}
}

void WindowsInputSystem::RegisterKeyPressFunction(char key, std::function<void()> function)
{
	keyToFunction.emplace(key, function);
}

void WindowsInputSystem::RegisterRightMouseFunction(std::function<void()> rcFunc)
{
	rightClickFunction = rcFunc;
}
