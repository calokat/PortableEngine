#pragma once
class IOpenGLContext
{
public:
	virtual int GetContext() = 0;
	virtual void _SwapBuffers(int winHandle) = 0;
};

