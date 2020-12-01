#include "EngineCameraControllerSystem.h"
#include "TransformSystem.h"
void EngineCameraControllerSystem::ControlCamera(IInputSystem* inputSystem, Transform& camTransform)
{
	float cameraMoveSpeed = .05f;
	if (inputSystem->IsMouseButtonPressed(MouseButton::Right))
	{
		LookCamera(inputSystem, camTransform);
	}
	if (inputSystem->IsKeyPressed(KeyCode::W))
	{
		MoveCameraRelative(camTransform, glm::vec3(0, 0, 1) * cameraMoveSpeed);
	}
	if (inputSystem->IsKeyPressed(KeyCode::S))
	{
		MoveCameraRelative(camTransform, glm::vec3(0, 0, -1) * cameraMoveSpeed);
	}
	if (inputSystem->IsKeyPressed(KeyCode::A))
	{
		MoveCameraRelative(camTransform, glm::vec3(1, 0, 0) * cameraMoveSpeed);
	}
	if (inputSystem->IsKeyPressed(KeyCode::D))
	{
		MoveCameraRelative(camTransform, glm::vec3(-1, 0, 0) * cameraMoveSpeed);
	}
	if (inputSystem->IsKeyPressed(KeyCode::Q))
	{
		MoveCameraRelative(camTransform, glm::vec3(0, -1, 0) * cameraMoveSpeed);
	}
	if (inputSystem->IsKeyPressed(KeyCode::E))
	{
		MoveCameraRelative(camTransform, glm::vec3(0, 1, 0) * cameraMoveSpeed);
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
		//cam.GetTransform()->Rotate(glm::vec3(newCamRotX * .005f, -delta.x * .005f, 0));
		TransformSystem::Rotate(glm::vec3(newCamRotX * .005f, -delta.x * .005f, 0), &camTransform);
	}
	else
	{
		//cam.GetTransform()->Rotate(glm::vec3(delta.y * .005f, -delta.x * .005f, 0));
		TransformSystem::Rotate(glm::vec3(delta.y * .005f, -delta.x * .005f, 0), &camTransform);
	}
}

void EngineCameraControllerSystem::MoveCameraRelative(Transform& camTransform, glm::vec3 dir)
{
	TransformSystem::MoveRelative(dir, &camTransform);
}

