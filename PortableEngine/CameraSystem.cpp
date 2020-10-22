#include "CameraSystem.h"
#include "TransformSystem.h"
void CameraSystem::CalculateViewMatrix(Camera& camera)
{
	glm::vec3 forward = TransformSystem::CalculateForward(camera.transform);
	camera.view = glm::lookAtRH(camera.transform->position, camera.transform->position + forward, glm::vec3(0, 1, 0));
}

void CameraSystem::CalculateProjectionMatrix(Camera& camera, float aspect)
{
		camera.projection = glm::perspective(camera.fieldOfView, aspect, camera.nearPlaneDistance, camera.farPlaneDistance);
}
