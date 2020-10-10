#ifdef __EMSCRIPTEN__
#include "EmscriptenPlatform.h"
#include <GLES3/gl3.h>

glm::vec2 EmscriptenPlatform::cursorPos = glm::vec2(0, 0);

EM_BOOL EmscriptenPlatform::MouseCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	cursorPos = glm::vec2(mouseEvent->targetX, mouseEvent->targetY);
	printf("%li, %li\n", mouseEvent->targetX, mouseEvent->targetY);
	//cursorPos = glm::vec2(mouseEvent->canvasX, mouseEvent->canvasY);
	return true;
}

EmscriptenPlatform::EmscriptenPlatform(GameWindow* win) : window(win)
{
}

int EmscriptenPlatform::InitWindow()
{
	Window root;
	XSetWindowAttributes swa;
	XSetWindowAttributes  xattr;
	Atom wm_state;
	XWMHints hints;
	XEvent xev;
	EGLConfig ecfg;
	EGLint num_config;
	Window win;

	/*
	 * X11 native display initialization
	 */

	x_display = XOpenDisplay(NULL);
	if (x_display == NULL)
	{
		return EGL_FALSE;
	}

	root = DefaultRootWindow(x_display);

	swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;
	win = XCreateWindow(
		x_display, root,
		0, 0, window->width, window->height, 0,
		CopyFromParent, InputOutput,
		CopyFromParent, CWEventMask,
		&swa);

	xattr.override_redirect = 0;
	XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

	hints.input = 1;
	hints.flags = InputHint;
	XSetWMHints(x_display, win, &hints);

	// make the window visible on the screen
	XMapWindow(x_display, win);
	XStoreName(x_display, win, "EmscriptenGL");

	// get identifiers for the provided atom name strings
	wm_state = XInternAtom(x_display, "_NET_WM_STATE", 0);

	memset(&xev, 0, sizeof(xev));
	xev.type = ClientMessage;
	xev.xclient.window = win;
	xev.xclient.message_type = wm_state;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = 1;
	xev.xclient.data.l[1] = 0;
	XSendEvent(
		x_display,
		DefaultRootWindow(x_display),
		0,
		SubstructureNotifyMask,
		&xev);

	hWnd = (EGLNativeWindowType)win;


	// register mouse move callback
	// With help from https://stackoverflow.com/questions/26331628/reference-to-non-static-member-function-must-be-called
	EM_BOOL (*mouseMoveRef)(int, const EmscriptenMouseEvent*, void*);
	mouseMoveRef = EmscriptenPlatform::MouseCallback;
	printf("About to register mouse move\n");
	emscripten_set_mousemove_callback("canvas.emscripten", this, true, mouseMoveRef);
	return EGL_TRUE;
	//return 0;
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
	return &hWnd;
}
void* EmscriptenPlatform::GetDeviceContext()
{
	return x_display;
}
glm::vec2 EmscriptenPlatform::GetCursorPosition()
{
	return cursorPos;
}
IInputSystem* EmscriptenPlatform::GetInputSystem()
{
	return inputSystem;
}
#endif