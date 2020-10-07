#ifdef _WIN64
#include <Windows.h>
#include "WindowsPlatform.h"
#include "DirectXAPI.h"
#endif
#ifdef __EMSCRIPTEN__
#include "EmscriptenPlatform.h"
#endif
#include "OpenGLAPI.h"
#include "PortableGame.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Camera.h"
#include <entt.hpp>
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

IPlatform* plat;
IGraphicsAPI* graph;
entt::registry registry;
void Loop()
{
	graph->Draw();
}

int main(int argc, char* argv[])
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
	 //long hr = S_OK;

	// Attempt to create the window for our program, and
	// exit early if something failed
	//hr = dxGame.InitWindow();
	//hr = glGame.InitWindow();
	//if((hr < 0)) return hr;

	// Attempt to initialize DirectX, and exit
	// early if something failed
	//hr = dxGame.InitDirectX();
	//hr = glGame.InitGL();
	//if(FAILED(hr)) return hr;

	// Begin the message and game loop, and then return
	// whatever we get back once the game loop is over
	//return dxGame.Run();
	//return glGame.Run();
	GameWindow* window = new GameWindow(0, 0, 800, 600);
	Camera cam = Camera(glm::vec3(0, 0, -3), (float)window->width / window->height);
	auto entity = registry.create();



#ifdef _WIN64
	plat = new WindowsPlatform(window);
#elif defined __EMSCRIPTEN__
	plat = new EmscriptenPlatform();
#endif
#ifdef _WIN64
	graph = new OpenGLAPI(window, plat);
#elif defined __EMSCRIPTEN__
	graph = new OpenGLAPI(window);
#endif
	plat->InitWindow();
	graph->Init();
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(Loop, 0, 1);
#else

	//GLCore glCore;
	//glewExperimental = true;
	//GLenum  glewStatus = glewInit();
	//if (glewStatus != GLEW_OK)
	//{
	//	printf("GLEW initialization failed");
	//	return -1;
	//}
	Mesh& mesh = registry.emplace<Mesh>(entity, plat->GetAssetPath("../../Assets/Models/torus.obj").c_str());
	Renderer& renderer = registry.emplace<Renderer>(entity, plat, &cam);
	renderer.LoadMesh(mesh.GetRawVertices());
	
	//game.Run();
	while (plat->Run() == 0)
	{



		auto view = registry.view<Mesh, Renderer>();
		graph->ClearScreen();
		for (auto renderable : view)
		{
			//Mesh& entMesh = view.get<Mesh>(renderable);
			//Renderer& entRenderer = view.get<Renderer>(renderable);
			//renderer.LoadMesh(mesh.GetRawVertices());
			renderer.Draw();
			//graph->Draw();
		}
		graph->_SwapBuffers();
		//graph->ClearScreen();
		
		
		
		//graph->Draw();
		//graph->ClearScreen();
	}
	//glCore.InitWindow();
	//glCore.InitGL();
	//return glCore.Run();
#endif
	//registry.destroy(entity);
	delete window;
	delete plat;
	delete graph;
	return 0;
}
