#pragma once
#include "AABB.h"
#include "Mesh.h"
#include "Transform.h"
class AABBSystem
{
public:
	static void UpdateAABB(AABB& bb, Mesh m, Transform t);
	static Mesh GenerateMeshFromAABB(AABB bb);
	static Mesh GetUnitCube();
};

