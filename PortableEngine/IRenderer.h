#pragma once
#include "PEImage.h"
class IRenderer
{
public:
	virtual ~IRenderer() {};
	PEImage diffuseTexture;
};