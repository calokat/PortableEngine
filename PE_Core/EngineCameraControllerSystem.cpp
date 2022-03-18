#include "EngineCameraControllerSystem.h"
#include "TransformSystem.h"
#include <glm/gtx/euler_angles.hpp>
void EngineCameraControllerSystem::ControlCamera(IInputSystem* inputSystem, Transform& camTransform)
{
	float cameraMoveSpeed = .05f;
	if (inputSystem->IsMouseButtonPressed(MouseButton::Right) || inputSystem->IsKeyPressed(KeyboardCode::LShift) || inputSystem->IsKeyPressed(KeyboardCode::RShift))
	{
		LookCamera(inputSystem, camTransform);
		if (inputSystem->IsKeyPressed(KeyboardCode::W))
		{
			MoveCameraRelative(camTransform, glm::vec3(0, 0, 1) * cameraMoveSpeed);
		}
		if (inputSystem->IsKeyPressed(KeyboardCode::S))
		{
			MoveCameraRelative(camTransform, glm::vec3(0, 0, -1) * cameraMoveSpeed);
		}
		if (inputSystem->IsKeyPressed(KeyboardCode::A))
		{
			MoveCameraRelative(camTransform, glm::vec3(-1, 0, 0) * cameraMoveSpeed);
		}
		if (inputSystem->IsKeyPressed(KeyboardCode::D))
		{
			MoveCameraRelative(camTransform, glm::vec3(1, 0, 0) * cameraMoveSpeed);
		}
		if (inputSystem->IsKeyPressed(KeyboardCode::Q))
		{
			MoveCameraRelative(camTransform, glm::vec3(0, -1, 0) * cameraMoveSpeed);
		}
		if (inputSystem->IsKeyPressed(KeyboardCode::E))
		{
			MoveCameraRelative(camTransform, glm::vec3(0, 1, 0) * cameraMoveSpeed);
		}
	}
}

void EngineCameraControllerSystem::LookCamera(IInputSystem* inputSystem, Transform& camTransform)
{
	//auto camView = registry.view<Camera>();
	//auto [camera, camTransform] = registry.get<Camera, Transform>(camView[0]);
	glm::vec2 delta = inputSystem->GetCursorPosition() - inputSystem->GetPreviousCursorPosition();
	float camRotX = camTransform.rotation.x;
	bool tooFarUp = camRotX > 3.f / 2;
	bool tooFarDown = camRotX < -3.f / 2;
	// checks to see if camera is in danger of gimbal lock
	if (tooFarUp || tooFarDown)
	{
		float newCamRotX;
		// we allow the input to rotate the camera on the x axis
		// if it is "too far down" and going up or "too far up" 
		// and going down. Otherwise zero out the x axis input
		if ((tooFarUp && delta.y < 0) || (tooFarDown && delta.y > 0))
		{
			newCamRotX = delta.y;
		}
		else
		{
			newCamRotX = 0;
		}
		camTransform.rotation += glm::vec3(newCamRotX * .005f, -delta.x * .005f, 0);
		camTransform.rotation.z = 0;
		glm::quat xQuat = glm::angleAxis(camTransform.rotation.x, glm::vec3(1, 0, 0));
		glm::quat yQuat = glm::angleAxis(-camTransform.rotation.y, glm::vec3(0, 1, 0));
		camTransform.orientation = yQuat * xQuat;
		TransformSystem::CalculateWorldMatrix(&camTransform);
		//camTransform.rotation += glm::vec3(newCamRotX * .005f, -delta.x * .005f, 0);
		//camTransform.rotation.z = 0;
		//glm::quat xQuat = glm::angleAxis(camTransform.rotation.x, glm::vec3(1, 0, 0));
		//glm::quat yQuat = glm::angleAxis(camTransform.rotation.y, glm::vec3(0, 1, 0));
		//camTransform.orientation = xQuat * yQuat;
		//TransformSystem::CalculateWorldMatrix(&camTransform);
		//camTransform.orientation = glm::quat(camTransform.rotation);
		//TransformSystem::CalculateWorldMatrix(&camTransform);
		//cam.GetTransform()->Rotate(glm::vec3(newCamRotX * .005f, -delta.x * .005f, 0));
		//TransformSystem::Rotate(glm::vec3(newCamRotX * .005f, delta.x * .005f, 0), &camTransform);
		//glm::quat rotQuat(glm::vec3(newCamRotX * .005f, delta.x * .005f, 0));
		//camTransform.orientation = rotQuat * camTransform.orientation;
		//glm::vec3 rotWithNoZ;
		//glm::extractEulerAngleXYZ(glm::mat4(camTransform.orientation), rotWithNoZ.x, rotWithNoZ.y, rotWithNoZ.z);
		//rotWithNoZ.z = 0;
		//camTransform.orientation = glm::quat(rotWithNoZ);
		//TransformSystem::CalculateWorldMatrix(&camTransform);
	}
	else
	{
		camTransform.rotation += glm::vec3(delta.y * .005f, -delta.x * .005f, 0);
		camTransform.rotation.z = 0;
		glm::quat xQuat = glm::angleAxis(camTransform.rotation.x, glm::vec3(1, 0, 0));
		glm::quat yQuat = glm::angleAxis(-camTransform.rotation.y, glm::vec3(0, 1, 0));
		camTransform.orientation = yQuat * xQuat;
		TransformSystem::CalculateWorldMatrix(&camTransform);
		//camTransform.orientation = glm::quat(camTransform.rotation);
		//TransformSystem::CalculateWorldMatrix(&camTransform);
		//cam.GetTransform()->Rotate(glm::vec3(delta.y * .005f, -delta.x * .005f, 0));
		//TransformSystem::Rotate(glm::vec3(delta.y * .005f, delta.x * .005f, 0), &camTransform);
		//glm::quat rotQuat(glm::vec3(delta.y * .005f, delta.x * .005f, 0));
		//camTransform.orientation = rotQuat * camTransform.orientation;
		//glm::vec3 rotWithNoZ;
		//glm::extractEulerAngleXYZ(glm::mat4(camTransform.orientation), rotWithNoZ.x, rotWithNoZ.y, rotWithNoZ.z);
		//rotWithNoZ.z = 0;
		//camTransform.orientation = glm::quat(rotWithNoZ);
		//TransformSystem::CalculateWorldMatrix(&camTransform);
	}
}

void EngineCameraControllerSystem::MoveCameraRelative(Transform& camTransform, glm::vec3 dir)
{
	TransformSystem::MoveRelative(dir, &camTransform);
}

