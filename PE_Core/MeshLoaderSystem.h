#pragma once
#include "Vertex.h"
#include "Mesh.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "Transform.h"
#include "Tree.h"
struct MeshCreateInfo
{
	Mesh m;
	Transform t;
};
class MeshLoaderSystem
{
public:
	static void ProcessMesh(aiMesh* assimpMesh, const aiScene* scene, Mesh& peMesh);
	static Tree<MeshCreateInfo> CreateMeshHeirarchy(const char* path);
	static void AddToMeshHeirarchy(Tree<MeshCreateInfo>& parent, const aiNode* assimpNode, const aiScene* assimpScene);
};

