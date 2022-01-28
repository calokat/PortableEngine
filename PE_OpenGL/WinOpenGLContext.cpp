#ifdef _WIN64
#include "WinOpenGLContext.h"
#include <iostream>

void* WinOpenGLContext::GetContext(GameWindow* window)
{
	if (m_hrc != nullptr)
	{
		return m_hrc;
	}
	//PIXELFORMATDESCRIPTOR pfd =
	//{
	//	sizeof(PIXELFORMATDESCRIPTOR),
	//	1,
	//	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
	//	PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
	//	32,                   // Colordepth of the framebuffer.
	//	0, 0, 0, 0, 0, 0,
	//	0,
	//	0,
	//	0,
	//	0, 0, 0, 0,
	//	24,                   // Number of bits for the depthbuffer
	//	8,                    // Number of bits for the stencilbuffer
	//	0,                    // Number of Aux buffers in the framebuffer.
	//	PFD_MAIN_PLANE,
	//	0,
	//	0, 0, 0
	//};

	//int formatID = ChoosePixelFormat((HDC)window->deviceContext, &pfd);
	//BOOL res = SetPixelFormat((HDC)window->deviceContext, formatID, &pfd);

	//HGLRC rc = wglCreateContext((HDC)window->deviceContext);
	//res = wglMakeCurrent((HDC)window->deviceContext, rc);



















	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(*(HDC*)platform->GetDeviceContext(), &pfd);

	SetPixelFormat(*(HDC*)platform->GetDeviceContext(), nPixelFormat, &pfd);

	HGLRC tempContext = wglCreateContext(*(HDC*)platform->GetDeviceContext());
	wglMakeCurrent(*(HDC*)platform->GetDeviceContext(), tempContext);

	glewExperimental = true;
	GLenum  glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
	{
		throw "GLEW initialization failed";
	}

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	m_hrc = wglCreateContextAttribsARB(*(HDC*)platform->GetDeviceContext(), 0, attribs);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempContext);
	wglMakeCurrent(*(HDC*)platform->GetDeviceContext(), m_hrc);

	if (!m_hrc) throw "Unable to get a GL context";


	return m_hrc;
}

void WinOpenGLContext::_SwapBuffers(int winHandle)
{
	//if (SwapBuffers(*(HDC*)platform->GetDeviceContext()))
	//	printf("Whoopee!\n");
	//else
	//	printf("Boo-hoo\n");
	SwapBuffers(*(HDC*)platform->GetDeviceContext());
}
WinOpenGLContext::WinOpenGLContext(IPlatform* plat) : platform(plat)
{
	m_hrc = nullptr;
}
#endif