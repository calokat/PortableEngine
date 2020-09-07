#pragma once
class IGraphicsAPI
{
public:
	virtual int Init() = 0;
	virtual void ClearScreen() = 0;
	virtual void Draw() = 0;
};