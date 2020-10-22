#pragma once
#include "Transform.h"
#include "glm/glm.hpp"
class TransformSystem
{
public:
	static void MoveRelative(glm::vec3 unrotated, Transform*);
	static void Rotate(glm::vec3 rotation, Transform*);
	static void CalculateWorldMatrix(Transform* transform);
	static glm::vec3 CalculateForward(Transform* transform);
};

