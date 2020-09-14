#pragma once
class IOpenGLContext
{
public:
	virtual int GetContext() = 0;
	virtual void SwapBuffers(int winHandle) = 0;
};

