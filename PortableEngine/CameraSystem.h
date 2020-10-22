#pragma once
#include "Camera.h"
#include "Transform.h"
class CameraSystem
{
public:
	static void CalculateViewMatrix(Camera& camera);
	static void CalculateProjectionMatrix(Camera& camera, float aspect);
};

