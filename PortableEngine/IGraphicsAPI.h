#pragma once
class IGraphicsAPI
{
public:
	virtual int Init() = 0;
	virtual void ClearScreen() = 0;
	virtual void Draw() = 0;
	virtual void _SwapBuffers() = 0;
	virtual ~IGraphicsAPI() {};
};