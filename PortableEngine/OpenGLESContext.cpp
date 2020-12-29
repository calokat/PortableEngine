#ifdef __EMSCRIPTEN__
#include "OpenGLESContext.h"
#include <iostream>

	ESContext esContext;

int OpenGLESContext::GetContext(GameWindow* window)
{
	//esInitContext(&esContext);
	//GLboolean iffy = esCreateWindow ( &esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB | ES_WINDOW_DEPTH );
	//if (iffy)
	//{
	//	printf("WHY\n");
	//}
	//else
	//{
	//	printf("Aha!\n");
	//}

	GLuint flags = ES_WINDOW_RGB | ES_WINDOW_DEPTH;

	EGLint attribList[] =
	{
		EGL_RED_SIZE,       5,
		EGL_GREEN_SIZE,     6,
		EGL_BLUE_SIZE,      5,
		EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
		EGL_NONE
	};


	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE, EGL_NONE };

	// Get Display
	display = eglGetDisplay((EGLNativeDisplayType)platform->GetDeviceContext());
	if (display == EGL_NO_DISPLAY)
	{
		return EGL_FALSE;
	}

	if (!eglInitialize(display, &majorVersion, &minorVersion))
	{
		return EGL_FALSE;
	}

	if (!eglGetConfigs(display, NULL, 0, &numConfigs))
	{
		return EGL_FALSE;
	}

	if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs))
	{
		return EGL_FALSE;
	}

	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)platform->GetWindowHandle(), NULL);

	if (surface == EGL_NO_SURFACE)
	{
		return EGL_FALSE;
	}

	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT)
	{
		return EGL_FALSE;
	}

	if (!eglMakeCurrent(display, surface, surface, context))
	{
		return EGL_FALSE;
	}


	
	glEnable(GL_DEPTH_TEST);
	 EmscriptenWebGLContextAttributes attr;
	emscripten_webgl_init_context_attributes(&attr);
	return 0;
}

void OpenGLESContext::_SwapBuffers(int winHandle)
{
	eglSwapBuffers(eglDisplay, eglSurface);
}
OpenGLESContext::OpenGLESContext(IPlatform* plat) : platform(plat)
{
}
#endif