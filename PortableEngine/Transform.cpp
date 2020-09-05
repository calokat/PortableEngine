#include "Transform.h"
#include <glm/gtx/euler_angles.hpp>
//using namespace DirectX;
Transform::Transform()
{
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	this->worldMatrix = glm::mat4(1.0f);
}

void Transform::SetPosition(glm::vec3 newPos)
{
	position = newPos;
}

void Transform::SetRotation(glm::vec3 newRot)
{
	rotation = newRot;
}

void Transform::SetScale(glm::vec3 newScale)
{
	scale = newScale;
}

glm::vec3 Transform::GetPosition()
{
	return position;
}

glm::vec3 Transform::GetRotation()
{
	return rotation;
}

glm::vec3 Transform::GetScale()
{
	return scale;
}

glm::mat4 Transform::GetWorldMatrix()
{
	glm::mat4 translationMat = glm::translate(glm::mat4(1.0), position);
	glm::mat4 rotationMat = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
	worldMatrix = scaleMat * rotationMat * translationMat;
	return worldMatrix;
}

void Transform::MoveAbsolute(glm::vec3 delta)
{
	position = position + delta;
}

void Transform::MoveRelative(glm::vec3 unrotated)
{
	glm::vec4 unrotatedVec(unrotated.x, unrotated.y, unrotated.z, 0);
	glm::mat4 rotationMat = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	glm::vec3 rotatedVec = rotationMat * unrotatedVec;
	position = position + rotatedVec;
}

void Transform::Rotate(glm::vec3 deltaRot)
{
	rotation += deltaRot;
	GetWorldMatrix();
}

void Transform::Scale(glm::vec3 deltaScale)
{
	scale += deltaScale;
}

void Transform::RotateAroundPoint(glm::vec3 point, float angle)
{
	float distance = sqrtf(powf(position.x - point.x, 2) + powf(position.y - point.y, 2) + powf(position.z - point.z, 2));
	glm::vec3 unitDir(cos(angle), sin(angle), 0);
	glm::vec3 dir = distance * unitDir;
	position = dir + point;

}

glm::vec3 Transform::GetForward()
{
	//glm::vec3 globalFwd(0, 0, 1);
	//glm::quat
	//glm::quat rotQuat = (rotation.x, rotation.y, rotation.z);
	//glm::vec3 localFwd = XMVector3Rotate(globalFwd, rotQuat);

	//glm::vec3 result;
	//result = localFwd;
	//return result;
	//glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
	//glm::vec4 vec4Result = rotationMat * glm::vec4(0, 0, 1, 0);
	//return glm::vec3(vec4Result.x, vec4Result.y, vec4Result.z);
	
	
	
	return glm::vec3(worldMatrix[2].x, worldMatrix[2].y, worldMatrix[2].z);
	
	
	// WHY DOESN'T THIS WORK
	/*glm::vec4 globalFwd(0, 0, 1, 0);
	glm::vec4 localFwd = globalFwd * glm::yawPitchRoll(-rotation.y, -rotation.x, -rotation.z);
	return glm::vec3(localFwd.x, localFwd.y, localFwd.z);*/

}
