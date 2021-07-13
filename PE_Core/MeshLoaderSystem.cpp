#include "MeshLoaderSystem.h"
#include <fstream>
#pragma warning(disable : 4996)

void MeshLoaderSystem::ProcessMesh(aiMesh* assimpMesh, const aiScene* scene, Mesh& m)
{
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
		m.rawVertices.push_back(vertex);
	}
	for (uint32_t i = 0; i < assimpMesh->mNumFaces; ++i)
	{
		aiFace face = assimpMesh->mFaces[i];

		for (uint32_t j = 0; j < face.mNumIndices; ++j)
		{
			m.rawIndices.push_back(face.mIndices[j]);
		}
	}
	m.path = assimpMesh->mName.C_Str();
	m.numIndices = m.rawIndices.size();
}

Tree<MeshCreateInfo> MeshLoaderSystem::CreateMeshHeirarchy(const char* path)
{
	Tree<MeshCreateInfo> res;
	Assimp::Importer imp;
	const aiScene* meshScene = imp.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_EmbedTextures);
	const aiNode* root = meshScene->mRootNode;
	MeshLoaderSystem::AddToMeshHeirarchy(res, root, meshScene);
	return res;
}

void MeshLoaderSystem::AddToMeshHeirarchy(Tree<MeshCreateInfo>& parent, const aiNode* assimpNode, const aiScene* assimpScene)
{
	aiVector3D pos, scale;
	aiQuaternion rot;
	assimpNode->mTransformation.Decompose(scale, rot, pos);
	parent.data.t.position = glm::vec3(pos.x, pos.y, pos.z);
	//parent.data.t.orientation = glm::quat(rot.w, rot.x, rot.y, rot.z);
	parent.data.t.scale = glm::vec3(scale.x, scale.y, scale.z);
	if (assimpNode->mNumMeshes == 0)
	{
		parent.isEmpty = true;
		parent.data.m.path = assimpNode->mName.C_Str();
	}
	for (int i = 0; i < assimpNode->mNumMeshes; ++i)
	{
		ProcessMesh(assimpScene->mMeshes[assimpNode->mMeshes[i]], assimpScene, parent.data.m);
	}
	parent.children.resize(assimpNode->mNumChildren);
	for (int j = 0; j < assimpNode->mNumChildren; ++j)
	{
		AddToMeshHeirarchy(parent.children[j], assimpNode->mChildren[j], assimpScene);
	}
}
