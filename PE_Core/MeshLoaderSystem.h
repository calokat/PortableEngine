#pragma once
#include "Vertex.h"
#include "Mesh.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

class MeshLoaderSystem
{
public:
	static void LoadMesh(const char* fileName, Mesh& m);
	static void ProcessMesh(aiMesh* assimpMesh, const aiScene* scene, Mesh& peMesh);
};

