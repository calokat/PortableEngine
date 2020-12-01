#include "AABBSystem.h"

void AABBSystem::UpdateAABB(AABB& bb, Mesh& m, Transform& t)
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
	max += t.position;
	min += t.position;
	bb.min = min;
	bb.max = max;
}
