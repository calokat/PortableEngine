#pragma once
#include <string>
#include <glm/glm.hpp>
struct Name
{
public:
	std::string nameString;
};

struct Rotator
{
	glm::vec3 deltaRot;
};
