#pragma once
#include "IPlatform.h"
#include "IGraphicsAPI.h"
#include "openxr/openxr.h"
#include "openxr/openxr_platform.h"

class XRAPI
{
public:
	XRAPI(IPlatform* plat, IGraphicsAPI* graph);
private:
	IPlatform* platform;
	IGraphicsAPI* graphics;
	XrInstance m_instance;
	XrResult Init();
	XrResult CreateXRInstance();
};

