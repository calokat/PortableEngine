#ifdef __EMSCRIPTEN__
#include "EmscriptenPlatform.h"
#include "EmscriptenInputSystem.h"
#include <GLES3/gl3.h>
//#include "imgui_impl_emscripten.h"
#include "EmscriptenAssetManager.h"
#include <examples/imgui_impl_sdl.h>
#include <SDL/SDL_opengles2.h>

EmscriptenPlatform::EmscriptenPlatform(GameWindow* win) : window(win)
{
	inputSystem = new EmscriptenInputSystem();
	assetManager = new EmscriptenAssetManager();
	//ImGui_ImplEmscripten_Init(window, inputSystem);
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return;
	}
}

EmscriptenPlatform::~EmscriptenPlatform()
{
	delete inputSystem;
	delete assetManager;
}

int EmscriptenPlatform::InitWindow()
{
	//Window root;
	//XSetWindowAttributes swa;
	//XSetWindowAttributes  xattr;
	//Atom wm_state;
	//XWMHints hints;
	//XEvent xev;
	//EGLConfig ecfg;
	//EGLint num_config;
	//Window win;

	///*
	// * X11 native display initialization
	// */

	//x_display = XOpenDisplay(NULL);
	//if (x_display == NULL)
	//{
	//	return EGL_FALSE;
	//}

	//root = DefaultRootWindow(x_display);

	//swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;
	//win = XCreateWindow(
	//	x_display, root,
	//	0, 0, window->width, window->height, 0,
	//	CopyFromParent, InputOutput,
	//	CopyFromParent, CWEventMask,
	//	&swa);

	//xattr.override_redirect = 0;
	//XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

	//hints.input = 1;
	//hints.flags = InputHint;
	//XSetWMHints(x_display, win, &hints);

	//// make the window visible on the screen
	//XMapWindow(x_display, win);
	//XStoreName(x_display, win, "EmscriptenGL");

	//// get identifiers for the provided atom name strings
	//wm_state = XInternAtom(x_display, "_NET_WM_STATE", 0);

	//memset(&xev, 0, sizeof(xev));
	//xev.type = ClientMessage;
	//xev.xclient.window = win;
	//xev.xclient.message_type = wm_state;
	//xev.xclient.format = 32;
	//xev.xclient.data.l[0] = 1;
	//xev.xclient.data.l[1] = 0;
	//XSendEvent(
	//	x_display,
	//	DefaultRootWindow(x_display),
	//	0,
	//	SubstructureNotifyMask,
	//	&xev);

	//hWnd = (EGLNativeWindowType)win;
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS);
	g_Window = SDL_CreateWindow("Dear ImGui Emscripten example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window->width, window->height, window_flags);

	SDL_RaiseWindow(g_Window);
	return 0;
}

long EmscriptenPlatform::Run()
{
	return 0;
}

std::string EmscriptenPlatform::GetAssetPath(std::string relativePath)
{
	return relativePath;
}

std::wstring EmscriptenPlatform::GetAssetPath_Wide(std::wstring relativePath)
{
	return relativePath;
}
void* EmscriptenPlatform::GetWindowHandle()
{
	return g_Window;
}
void* EmscriptenPlatform::GetDeviceContext()
{
	return g_Window;
}
IInputSystem* EmscriptenPlatform::GetInputSystem()
{
	return inputSystem;
}
void EmscriptenPlatform::NewGuiFrame()
{
	ImGui_ImplSDL2_NewFrame(g_Window);
}

IAssetManager* EmscriptenPlatform::GetAssetManager()
{
	return assetManager;
}
#endif