#include "MeshMaker.h"
#include "Relationship.h"
#include "TransformSystem.h"
#include "misc_components.h"
#include "AABBSystem.h"
entt::entity MakeMesh_Recursive(entt::registry& registry, Tree<MeshCreateInfo> scene, IRenderSystem* renderSystem, IAssetManager* assetManager, Camera& renderingCam, Transform& renderingCamTransform, entt::entity parent)
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
		IRenderer& newMeshRenderer = renderSystem->CreateRenderer(registry, newMeshEntity, ShaderType::Unlit_Textured);
		renderSystem->Load(&newMeshRenderer, renderingCam);
		renderSystem->LoadMesh(&newMeshRenderer, newMesh);
		renderSystem->LoadTexture(&newMeshRenderer, assetManager->GetAssetPath("../../Assets/Images/rock.png"));
		Name& nameComp = registry.emplace<Name>(newMeshEntity);
		nameComp = { newMesh.path };
		AABB& aabb = registry.emplace<AABB>(newMeshEntity);
		AABBSystem::UpdateAABB(aabb, newMesh, meshTransform);
	}
	for (int i = 0; i < scene.children.size(); ++i)
	{
		MakeMesh_Recursive(registry, scene.children[i], renderSystem, assetManager, renderingCam, renderingCamTransform, newMeshEntity);
	}
	return newMeshEntity;
}

entt::entity MakeMesh(Tree<MeshCreateInfo> meshScene, entt::registry& registry, IRenderSystem* renderSystem, IAssetManager* assetManager, entt::entity meshRoot, glm::vec3 pos)
{
	auto camView = registry.view<Camera>();
	auto [camera, camTransform] = registry.get<Camera, Transform>(camView[0]);
	return MakeMesh_Recursive(registry, meshScene, renderSystem, assetManager, camera, camTransform, meshRoot);
	//Scene<Transform&> s = { camTransform, {{camTransform}} };
}