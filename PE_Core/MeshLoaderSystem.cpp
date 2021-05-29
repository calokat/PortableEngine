#include "MeshLoaderSystem.h"
#include <fstream>
#pragma warning(disable : 4996)

void MeshLoaderSystem::LoadMesh(const char* fileName, Mesh& m)
{
	Assimp::Importer imp;
	const aiScene* scene = imp.ReadFile(fileName, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (scene == nullptr) return;
	ProcessMesh(scene->mMeshes[0], scene, m);

}

void MeshLoaderSystem::ProcessMesh(aiMesh* assimpMesh, const aiScene* scene, Mesh& peMesh)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < assimpMesh->mNumVertices; ++i)
	{
		Vertex vertex;
		vertex.Position.x = assimpMesh->mVertices[i].x;
		vertex.Position.y = assimpMesh->mVertices[i].y;
		vertex.Position.z = assimpMesh->mVertices[i].z;
		if (assimpMesh->mTextureCoords[0]) {
			vertex.UV.x = (float)assimpMesh->mTextureCoords[0][i].x;
			vertex.UV.y = (float)assimpMesh->mTextureCoords[0][i].y;
		}
		vertices.push_back(vertex);
	}
	for (uint32_t i = 0; i < assimpMesh->mNumFaces; ++i)
	{
		aiFace face = assimpMesh->mFaces[i];

		for (uint32_t j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	peMesh.numIndices = indices.size();
	peMesh.rawIndices = indices;
	peMesh.rawVertices = vertices;
}
