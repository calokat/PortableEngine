#pragma once
#include "PEImage.h"
class IRenderer
{
public:
	virtual ~IRenderer() {};
	//virtual IRenderer& operator=(IRenderer&& other) noexcept 
	//{
	//	if (this != &other)
	//	{
	//		this->diffuseTexture = std::move(other.diffuseTexture);
	//	}
	//	return *this;
	//};
	//PEImage diffuseTexture;
};