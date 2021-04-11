#include "WindowHeader.h"
#include <imgui.h>
#include "MeshLoaderSystem.h"
#include "TransformSystem.h"
#include "misc_components.h"
#include "AABBSystem.h"
void WindowHeader::Render(entt::registry& registry, IAssetManager* assetManager, IRenderSystem* renderSystem)
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::BeginMenu("New"))
		{
			if (ImGui::MenuItem("Cube"))
			{
				MakeMesh(assetManager->GetAssetPath("../../Assets/Models/cube.obj").c_str(), registry, assetManager, renderSystem, "Cube");
			}
			if (ImGui::MenuItem("Helix"))
			{
				MakeMesh(assetManager->GetAssetPath("../../Assets/Models/helix.obj").c_str(), registry, assetManager, renderSystem, "Helix");
			}
			if (ImGui::MenuItem("Cone"))
			{
				MakeMesh(assetManager->GetAssetPath("../../Assets/Models/cone.obj").c_str(), registry, assetManager, renderSystem, "Cone");
			}
			if (ImGui::MenuItem("Cylinder"))
			{
				MakeMesh(assetManager->GetAssetPath("../../Assets/Models/cylinder.obj").c_str(), registry, assetManager, renderSystem, "Cylinder");
			}
			if (ImGui::MenuItem("Sphere"))
			{
				MakeMesh(assetManager->GetAssetPath("../../Assets/Models/sphere.obj").c_str(), registry, assetManager, renderSystem, "Sphere");
			}
			if (ImGui::MenuItem("Torus"))
			{
				MakeMesh(assetManager->GetAssetPath("../../Assets/Models/torus.obj").c_str(), registry, assetManager, renderSystem, "Torus");
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void WindowHeader::MakeMesh(const char* path, entt::registry& registry, IAssetManager* assetManager, IRenderSystem* renderSystem, const char* name) {
	auto camView = registry.view<Camera>();
	auto [camera, camTransform] = registry.get<Camera, Transform>(camView[0]);
	glm::vec3 newMeshPos = camTransform.position + TransformSystem::CalculateForward(&camTransform);
	auto newMeshEntity = registry.create();
	Mesh& newMesh = registry.emplace<Mesh>(newMeshEntity, path);
	MeshLoaderSystem::LoadMesh(newMesh.path.c_str(), newMesh);
	Transform& meshTransform = registry.emplace<Transform>(newMeshEntity);
	meshTransform.position = newMeshPos;
	TransformSystem::CalculateWorldMatrix(&meshTransform);
	IRenderer& newMeshRenderer = renderSystem->CreateRenderer(registry, newMeshEntity);
	renderSystem->Load(&newMeshRenderer, camera);
	renderSystem->LoadMesh(&newMeshRenderer, newMesh);
	//newMeshRenderer.diffuseTexture = PEImage(plat->GetAssetManager()->GetAssetPath("../../Assets/Images/cushion.png"));
	//ImageSystem::CreateImage(newMeshRenderer.diffuseTexture);
	//renderSystem->CreateTexture(newMeshRenderer.diffuseTexture);
	renderSystem->LoadTexture(&newMeshRenderer, assetManager->GetAssetPath("../../Assets/Images/cushion.png")/*newMeshRenderer.diffuseTexture*/);
	//ImageSystem::DestroyImage(newMeshRenderer.diffuseTexture);
	Name& nameComp = registry.emplace<Name>(newMeshEntity);
	nameComp = { name };
	AABB& aabb = registry.emplace<AABB>(newMeshEntity);
	AABBSystem::UpdateAABB(aabb, newMesh, meshTransform);
}
