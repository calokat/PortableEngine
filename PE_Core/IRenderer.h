#pragma once
#include "PEImage.h"

enum ShaderProgramProperties { Fully_3D = 1, Colored = 2, Textured = 4, Lit = 8, Normal = 16 };

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