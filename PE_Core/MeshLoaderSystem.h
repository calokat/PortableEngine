#pragma once
#include "Vertex.h"
#include "Mesh.h"
class MeshLoaderSystem
{
public:
	static void LoadMesh(const char* fileName, Mesh& m);
	static void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
};

