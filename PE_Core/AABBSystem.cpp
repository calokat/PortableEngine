#include "AABBSystem.h"

void AABBSystem::UpdateAABB(AABB& bb, Mesh m, Transform t)
{
	glm::vec3 min(FLT_MAX);
	glm::vec3 max(FLT_MIN);
	for (std::vector<Vertex>::const_iterator it = m.rawVertices.begin(); it != m.rawVertices.end(); ++it)
	{
		if (it->Position.x < min.x)
		{
			min.x = it->Position.x;
		}
		if (it->Position.y < min.y)
		{
			min.y = it->Position.y;
		}
		if (it->Position.z < min.z)
		{
			min.z = it->Position.z;
		}
		if (it->Position.x > max.x)
		{
			max.x = it->Position.x;
		}
		if (it->Position.y > max.y)
		{
			max.y = it->Position.y;
		}
		if (it->Position.z > max.z)
		{
			max.z = it->Position.z;
		}
	}
	bb.min = min;
	bb.max = max;
}

Mesh AABBSystem::GenerateMeshFromAABB(AABB bb)
{
	Vertex vertices[8];
	vertices[0].Position = bb.max;
	vertices[1].Position = glm::vec3(bb.max.x, bb.max.y, bb.min.z);
	vertices[2].Position = glm::vec3(bb.max.x, bb.min.y, bb.min.z);
	vertices[3].Position = glm::vec3(bb.max.x, bb.min.y, bb.max.z);
	vertices[4].Position = glm::vec3(bb.min.x, bb.max.y, bb.min.z);
	vertices[5].Position = glm::vec3(bb.min.x, bb.min.y, bb.max.z);
	vertices[6].Position = glm::vec3(bb.min.x, bb.max.y, bb.max.z);
	vertices[7].Position = bb.min;
	Mesh result = { 8, "", {vertices[0], vertices[1], vertices[2], vertices[3], vertices[4], vertices[5], vertices[6], vertices[7]}, {0, 1, 2, 3, 4, 5, 6, 7} };	
	return result;
}

Mesh AABBSystem::GetUnitCube()
{
	return GenerateMeshFromAABB({ glm::vec3(-.5, -.5, -.5), glm::vec3(.5, .5, .5) });
}
