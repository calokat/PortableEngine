#pragma once
#include "PEImage.h"

enum ShaderProgramProperties { Fully_3D = 1, Colored = 2, Textured = 4, Lit = 8, Normal = 16 };

enum ShaderType
{
	Unlit_Color = ShaderProgramProperties::Colored | ShaderProgramProperties::Fully_3D,
	Unlit_Textured = ShaderProgramProperties::Fully_3D | ShaderProgramProperties::Textured | ShaderProgramProperties::Colored,
	Lit_Color = ShaderProgramProperties::Fully_3D | ShaderProgramProperties::Colored | ShaderProgramProperties::Lit,
	Lit_Textured = ShaderProgramProperties::Fully_3D | ShaderProgramProperties::Colored | ShaderProgramProperties::Lit | ShaderProgramProperties::Textured,
	Lit_Textured_Normal = ShaderProgramProperties::Fully_3D | ShaderProgramProperties::Colored | ShaderProgramProperties::Lit | ShaderProgramProperties::Textured | ShaderProgramProperties::Normal
};

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