#include "OpenGLESContext.h"
#include <iostream>

	ESContext esContext;

int OpenGLESContext::GetContext(GameWindow* window)
{
	esInitContext(&esContext);
	GLboolean iffy = esCreateWindow ( &esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB | ES_WINDOW_DEPTH );
	if (iffy)
	{
		printf("WHY\n");
	}
	else
	{
		printf("Aha!\n");
	}
	
	glEnable(GL_DEPTH_TEST);
	 EmscriptenWebGLContextAttributes attr;
	emscripten_webgl_init_context_attributes(&attr);
	return 0;
}

void OpenGLESContext::SwapBuffers(int winHandle)
{
	eglSwapBuffers(esContext.eglDisplay, esContext.eglSurface);
}
