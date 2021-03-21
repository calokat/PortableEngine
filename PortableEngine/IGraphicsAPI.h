#pragma once
#include "IXRGraphicsPlugin.h"
class IGraphicsAPI
{
public:
	virtual int Init() = 0;
	virtual void ClearScreen() = 0;
	virtual void Draw() = 0;
	virtual void _SwapBuffers() = 0;
	virtual void NewGuiFrame() = 0;
	virtual void DrawGui() = 0;
	virtual void OnResize() = 0;
	virtual ~IGraphicsAPI() {};
	virtual IXRGraphicsPlugin* GetXRGraphicsPlugin() = 0;
};