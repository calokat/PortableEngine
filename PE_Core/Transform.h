#pragma once
//#include <DirectXMath.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
class Transform
{
	public:
		glm::mat4 worldMatrix;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		Transform();
		//void SetPosition(glm::vec3 newPos);
		//void SetRotation(glm::vec3 newRot);
		//void SetScale(glm::vec3 newScale);
		//glm::vec3 GetPosition();
		//glm::vec3 GetRotation();
		//glm::vec3 GetScale();
		//glm::mat4 GetWorldMatrix();
		//void MoveAbsolute(glm::vec3);
		//void MoveRelative(glm::vec3);
		//void Rotate(glm::vec3);
		//void Scale(glm::vec3);
		//void RotateAroundPoint(glm::vec3 point, float angle);
		//glm::vec3 GetForward();
		Transform& operator=(Transform&& other);
		Transform& operator=(Transform& other);
		Transform(Transform&& other) noexcept;
		Transform(Transform& other);
		bool operator==(Transform& other);
};