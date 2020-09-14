#include "WinOpenGLContext.h"
#include <iostream>

int WinOpenGLContext::GetContext(GameWindow* window)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int formatID = ChoosePixelFormat((HDC)window->deviceContext, &pfd);
	SetPixelFormat((HDC)window->deviceContext, formatID, &pfd);

	HGLRC rc = wglCreateContext((HDC)window->deviceContext);
	wglMakeCurrent((HDC)window->deviceContext, rc);

	// Thanks to https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/
	//const int pixelAttribs[] = {
	//WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
	//WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
	//WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
	//WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
	//WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
	//WGL_COLOR_BITS_ARB, 32,
	//WGL_ALPHA_BITS_ARB, 8,
	//WGL_DEPTH_BITS_ARB, 24,
	//WGL_STENCIL_BITS_ARB, 8,
	//WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
	//WGL_SAMPLES_ARB, 4,
	//0
	//};

	//int pixelFormatID; UINT numFormats;
	//bool status = wglChoosePixelFormatARB((HDC)window->deviceContext, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);

	//if (status == false || numFormats == 0)
	//{
	//	printf("wglChoosePixelFormatARB() failed.");
	//	return 1;
	//}

	//PIXELFORMATDESCRIPTOR PFD;
	//DescribePixelFormat((HDC)window->deviceContext, pixelFormatID, sizeof(PFD), &PFD);
	//SetPixelFormat((HDC)window->deviceContext, pixelFormatID, &PFD);

	//const int major_min = 4, minor_min = 5;
	//int  contextAttribs[] = {
	//	WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
	//	WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
	//	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	//	0
	//};
	//HGLRC RC;
	//RC = wglCreateContextAttribsARB((HDC)window->deviceContext, 0, contextAttribs);
	//if (RC == NULL) {
	//	printf("wglCreateContextAttribsARB() failed.");
	//	return 1;
	//}

	//wglMakeCurrent((HDC)window->deviceContext, RC);


	glewExperimental = true;
	GLenum  glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
	{
		printf("GLEW initialization failed");
		return -1;
	}

}

void WinOpenGLContext::_SwapBuffers(int winHandle)
{
	SwapBuffers((HDC)winHandle);
}