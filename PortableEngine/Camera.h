#pragma once
#include "Transform.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
//#include <d3d11.h>
//#include <math.h>
class Camera
{
private:
	Transform* transform;
	glm::mat4 view;
	glm::mat4 projection;
	float fieldOfView;
	float aspectRatio;
	float nearPlaneDistance;
	float farPlaneDistance;
	float movementSpeed;
	float lookSpeed;
	glm::vec2 prevMousePosition;
	void UpdateViewMatrix();
public:
	Camera();
	Camera(glm::vec3 initialPos, float aspectRatio);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	void UpdateProjectionMatrix(float aspect);
	//void Update(float dt, HWND windowHandle);
	Transform* GetTransform();
	~Camera();
};

