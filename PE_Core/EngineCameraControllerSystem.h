#pragma once
#include "IInputSystem.h"
#include "Transform.h"
class EngineCameraControllerSystem
{
public:
	static void ControlCamera(IInputSystem* inputSystem, Transform& camTransform);
private:
	static void LookCamera(IInputSystem* inputSystem, Transform& camTransform);
	static void MoveCameraRelative(Transform& camTransform, glm::vec3 dir);
};
