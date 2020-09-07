
#include <Windows.h>
#include "Game.h"
#include "GLCore.h"
#include "WindowsPlatform.h"
#include "DirectXAPI.h"
#include "OpenGLAPI.h"
#include "PortableGame.h"
// --------------------------------------------------------
// Entry point for a graphical (non-console) Windows application
// --------------------------------------------------------
//int WINAPI WinMain(
//	_In_ HINSTANCE hInstance,		// The handle to this app's instance
//	_In_opt_ HINSTANCE hPrevInstance,	// A handle to the previous instance of the app (always NULL)
//	_In_ LPSTR lpCmdLine,			// Command line params
//	_In_ int nCmdShow)				// How the window should be shown (we ignore this)
//{

//HRESULT Run()
//{
//	MSG msg = {};
//	while (msg.message != WM_QUIT)
//	{
//		// Determine if there is a message waiting
//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			// Translate and dispatch the message
//			// to our custom WindowProc function
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		//else
//		//{
//		//	// Update timer and title bar (if necessary)
//		//	UpdateTimer();
//		//	if (titleBarStats)
//		//		UpdateTitleBarStats();
//
//		//	// The game loop
//		//	Update(deltaTime, totalTime);
//		//	Draw(deltaTime, totalTime);
//		//}
//	}
//
//	// We'll end up here once we get a WM_QUIT message,
//	// which usually comes from the user closing the window
//	return (HRESULT)msg.wParam;
//
//}

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Enable memory leak detection as a quick and dirty
	// way of determining if we forgot to clean something up
	//  - You may want to use something more advanced, like Visual Leak Detector
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Create the Game object using
	// the app handle we got from WinMain
	//Game dxGame(GetModuleHandle(NULL));
	//GLCore glGame;
	// Result variable for function calls below
	HRESULT hr = S_OK;

	// Attempt to create the window for our program, and
	// exit early if something failed
	//hr = dxGame.InitWindow();
	//hr = glGame.InitWindow();
	if(FAILED(hr)) return hr;

	// Attempt to initialize DirectX, and exit
	// early if something failed
	//hr = dxGame.InitDirectX();
	//hr = glGame.InitGL();
	if(FAILED(hr)) return hr;

	// Begin the message and game loop, and then return
	// whatever we get back once the game loop is over
	//return dxGame.Run();
	//return glGame.Run();
	GameWindow* window = new GameWindow(0, 0, 800, 600);
	WindowsPlatform win = WindowsPlatform(window);
	//DirectXAPI dx = DirectXAPI(window);
	OpenGLAPI gl = OpenGLAPI(window);
	PortableGame game = PortableGame(&win, &gl, window);
	game.Start();
	return game.Run();
}
