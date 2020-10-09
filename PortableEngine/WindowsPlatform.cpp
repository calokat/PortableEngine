#ifdef _WIN64
#include "WindowsInputSystem.h"
#include "WindowsPlatform.h"

WindowsPlatform* WindowsPlatform::staticThis = 0;

int WindowsPlatform::InitWindow()
{
	WNDCLASS wndClass = {}; // Zero out the memory
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw on horizontal or vertical movement/adjustment
	wndClass.lpfnWndProc = WindowsProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;						// Our app's handle
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// Default icon
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// Default arrow cursor
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "WinWindowClass";

	// Attempt to register the window class we've defined
	if (!RegisterClass(&wndClass))
	{
		// Get the most recent error
		DWORD error = GetLastError();

		// If the class exists, that's actually fine.  Otherwise,
		// we can't proceed with the next step.
		if (error != ERROR_CLASS_ALREADY_EXISTS)
			return HRESULT_FROM_WIN32(error);
	}

	// Adjust the width and height so the "client size" matches
// the width and height given (the inner-area of the window)
	RECT clientRect;
	SetRect(&clientRect, 0, 0, window->width, window->height);
	AdjustWindowRect(
		&clientRect,
		WS_OVERLAPPEDWINDOW,	// Has a title bar, border, min and max buttons, etc.
		false);					// No menu bar

		// Center the window to the screen
	RECT desktopRect;
	GetClientRect(GetDesktopWindow(), &desktopRect);
	int centeredX = (desktopRect.right / 2) - (clientRect.right / 2);
	int centeredY = (desktopRect.bottom / 2) - (clientRect.bottom / 2);

	hwnd = CreateWindow(
		wndClass.lpszClassName,
		"Portable Engine",
		WS_OVERLAPPEDWINDOW,
		centeredX,
		centeredY,
		clientRect.right - clientRect.left,	// Calculated width
		clientRect.bottom - clientRect.top,	// Calculated height
		0,			// No parent window
		0,			// No menu
		hInstance,	// The app's handle
		0);			// No other windows in our application
	window->windowHandle = (int)hwnd;
	// Ensure the window was created properly
	if (hwnd == NULL)
	{
		DWORD error = GetLastError();
		return HRESULT_FROM_WIN32(error);
	}

	// The window exists but is not visible yet
	// We need to tell Windows to show it, and how to show it
	ShowWindow(hwnd, SW_SHOW);

	hdc = GetDC(hwnd);
	window->deviceContext = (int)hdc;

	// Return an "everything is ok" HRESULT value
	return S_OK;

}

unsigned int WindowsPlatform::GetWindowWidth()
{
	return window->width;
}

unsigned int WindowsPlatform::GetWindowHeight()
{
	return window->height;
}

HWND WindowsPlatform::GetHwnd()
{
	return (HWND)window->windowHandle;
}

long WindowsPlatform::Run()
{
		MSG msg = {};
		//while (msg.message != WM_QUIT)
		//{
			// Determine if there is a message waiting
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					return 1;
				}
				// Translate and dispatch the message
				// to our custom WindowProc function
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			return 0;
			//else
			//{
			//	// Update timer and title bar (if necessary)
			//	UpdateTimer();
			//	if (titleBarStats)
			//		UpdateTitleBarStats();

			//	// The game loop
			//	Update(deltaTime, totalTime);
			//	Draw(deltaTime, totalTime);
			//}
		//}
}

std::string WindowsPlatform::GetAssetPath(std::string relativePath)
{
	return GetExePath() + "\\" + relativePath;
}

std::wstring WindowsPlatform::GetAssetPath_Wide(std::wstring relativePath)
{
	return GetExePath_Wide() + L"\\" + relativePath;
}

void* WindowsPlatform::GetWindowHandle()
{
	return &hwnd;
}

void* WindowsPlatform::GetDeviceContext()
{
	return &hdc;
}

glm::vec2 WindowsPlatform::GetCursorPosition()
{
	POINT mousePos = {};
	GetCursorPos(&mousePos);
	ScreenToClient(hwnd, &mousePos);
	return glm::vec2(mousePos.y, mousePos.x);
}

IInputSystem* WindowsPlatform::GetInputSystem()
{
	return inputSystem;
}

WindowsPlatform::WindowsPlatform(GameWindow* win)
{
	staticThis = this;
	window = win;
	inputSystem = new WindowsInputSystem();
	//hInstance = hinst;
	/*windowWidth = winWidth;
	windowHeight = winHeight;*/

}

WindowsPlatform::~WindowsPlatform()
{
	delete inputSystem;
}

LRESULT WindowsPlatform::WindowsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return staticThis->ProcessMessage(hWnd, uMsg, wParam, lParam);
}

LRESULT WindowsPlatform::ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Check the incoming message and handle any we care about
	switch (uMsg)
	{
		// This is the message that signifies the window closing
	case WM_DESTROY:
		PostQuitMessage(0); // Send a quit message to our own program
		return 0;

		// Prevent beeping when we "alt-enter" into fullscreen
	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);

		// Prevent the overall window from becoming too small
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

		// Sent when the window size changes
	case WM_SIZE:
		// Don't adjust anything when minimizing,
		// since we end up with a width/height of zero
		// and that doesn't play well with the GPU
		if (wParam == SIZE_MINIMIZED)
			return 0;

		// Save the new client area dimensions.
		window->width = LOWORD(lParam);
		window->height = HIWORD(lParam);

		// If DX is initialized, resize 
		// our required buffers

		//if (device)
		//	OnResize();

		return 0;

		// Is our focus state changing?
	case WM_SETFOCUS:	hasFocus = true;	return 0;
	case WM_KILLFOCUS:	hasFocus = false;	return 0;
	case WM_ACTIVATE:	hasFocus = (LOWORD(wParam) != WA_INACTIVE); return 0;
	}

	// Let Windows handle any messages we're not touching
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}

std::string WindowsPlatform::GetExePath()
{
	// Assume the path is just the "current directory" for now
	std::string path = ".\\";

	// Get the real, full path to this executable
	char currentDir[1024] = {};
	GetModuleFileName(0, currentDir, 1024);

	// Find the location of the last slash charaacter
	char* lastSlash = strrchr(currentDir, '\\');
	if (lastSlash)
	{
		// End the string at the last slash character, essentially
		// chopping off the exe's file name.  Remember, c-strings
		// are null-terminated, so putting a "zero" character in 
		// there simply denotes the end of the string.
		*lastSlash = 0;
		
		// Set the remainder as the path
		path = currentDir;
	}

	// Toss back whatever we've found
	return path;
}

std::wstring WindowsPlatform::GetExePath_Wide()
{
	// Grab the path as a standard string
	std::string path = GetExePath();

	// Convert to a wide string
	wchar_t widePath[1024] = {};
	mbstowcs_s(0, widePath, path.c_str(), 1024);

	// Create a wstring for it and return
	return std::wstring(widePath);
}
#endif