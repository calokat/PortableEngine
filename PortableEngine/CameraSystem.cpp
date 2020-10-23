#include "CameraSystem.h"
#include "TransformSystem.h"
void CameraSystem::CalculateViewMatrix(Camera& camera, Transform camTransform)
{
	glm::vec3 forward = TransformSystem::CalculateForward(&camTransform);
	camera.view = glm::lookAtRH(camTransform.position, camTransform.position + forward, glm::vec3(0, 1, 0));
}

void CameraSystem::CalculateProjectionMatrix(Camera& camera, float aspect)
{
		camera.projection = glm::perspective(camera.fieldOfView, aspect, camera.nearPlaneDistance, camera.farPlaneDistance);
}
