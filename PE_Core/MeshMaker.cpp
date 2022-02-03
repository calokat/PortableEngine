#include "MeshMaker.h"
#include "Relationship.h"
#include "TransformSystem.h"
#include "misc_components.h"
#include "AABBSystem.h"
#include <map>

entt::entity MakeMesh_Recursive(entt::registry& registry, Tree<MeshCreateInfo> scene, entt::entity parent, bool attachAABBs)
{
	auto newMeshEntity = registry.create();
	Relationship& childRel = registry.emplace<Relationship>(newMeshEntity);
	Relationship& parentRel = registry.get<Relationship>(parent);
	childRel.parent = parent;
	parentRel.children.insert(std::pair<int, entt::entity>((int)newMeshEntity, newMeshEntity));
	//Tree<entt::entity> newEntityNode = { newMeshEntity };
	//parent.children.push_back(newEntityNode);
	if (scene.data.m.rawVertices.size() == 0)
	{
		TransformSystem::CalculateWorldMatrix(&scene.data.t);
		registry.emplace<Transform>(newMeshEntity, scene.data.t);
		Name& nameComp = registry.emplace<Name>(newMeshEntity);
		nameComp = { scene.data.m.path };
	}
	else if (!scene.isEmpty)
	{
		Mesh& newMesh = registry.emplace<Mesh>(newMeshEntity, scene.data.m);
		TransformSystem::CalculateWorldMatrix(&scene.data.t);
		Transform& meshTransform = registry.emplace<Transform>(newMeshEntity, scene.data.t);
		Name& nameComp = registry.emplace<Name>(newMeshEntity);
		nameComp = { newMesh.path };
		if (attachAABBs)
		{
			AABB& aabb = registry.emplace<AABB>(newMeshEntity);
			AABBSystem::UpdateAABB(aabb, newMesh, meshTransform);
		}
	}
	for (int i = 0; i < scene.children.size(); ++i)
	{
		MakeMesh_Recursive(registry, scene.children[i], newMeshEntity, attachAABBs);
	}
	return newMeshEntity;
}

void AttachRenderers(entt::registry& registry, IRenderSystem* renderSystem, std::map<const char*, const char*> texturePaths, entt::entity rootEntity, ShaderType shaderType)
{
	IRenderer& newMeshRenderer = renderSystem->CreateRenderer(registry, rootEntity, shaderType);
	Mesh& entityMesh = registry.get<Mesh>(rootEntity);
	Camera dummyCam;
	// TODO: Remove camera parameter from renderSystem->Load()
	renderSystem->Load(&newMeshRenderer, dummyCam);
	renderSystem->LoadMesh(&newMeshRenderer, entityMesh);
	Relationship rel = registry.get<Relationship>(rootEntity);
	if (shaderType & ShaderProgramProperties::Textured)
	{
		renderSystem->LoadTexture(&newMeshRenderer, texturePaths);
	}
	for (auto childIt = rel.children.begin(); childIt != rel.children.end(); ++childIt)
	{
		AttachRenderers(registry, renderSystem, texturePaths, childIt->second, shaderType);
	}
}

entt::entity MakeMesh(Tree<MeshCreateInfo> meshScene, entt::registry& registry, entt::entity meshRoot)
{
	return MakeMesh_Recursive(registry, meshScene, meshRoot);
}