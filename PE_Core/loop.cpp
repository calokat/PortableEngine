#include "loop.h"
#include <imgui.h>
#include "GizmoSystem.h"
#include "raycast.h"
#include "EntityListWindow.h"
#include "InspectorWindow.h"
#include "WindowHeader.h"
#include "TransformSystem.h"
#include "CameraSystem.h"
#include <thread>
#include "EngineCameraControllerSystem.h"
#include "GLRenderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Relationship.h"
#ifdef _WIN64
#include "DIrectXRenderer.h"
#endif
#include "BillboardSystem.h"
#include "LightsSystem.h"
#include "MeshLoaderSystem.h"
#include "MeshMaker.h"
#include "RendererMenu.h"
template<class T>
void DrawIteration(Camera& camera, entt::entity selected, entt::registry& registry, IRenderSystem* renderSystem)
{
	auto renderableView = registry.view<T, Transform>();
	PointLight pointLights[MAX_POINT_LIGHTS];
	DirectionalLight dirLights[MAX_DIR_LIGHTS];
	LightsSystem::ExtractLightsFromRegistry(registry, dirLights, pointLights);
	for (auto rIt = renderableView.begin(); rIt != renderableView.end(); ++rIt)
	{
		T& renderer = registry.get<T>(*rIt);
		Transform& meshTransform = registry.get<Transform>(*rIt);
		renderSystem->BindRenderer(&renderer);
		renderSystem->UpdateRenderer(&renderer, meshTransform, camera, dirLights, pointLights);
		renderSystem->Draw(&renderer);
	}
}

void Loop(IPlatform* plat, IGraphicsAPI* graph, IRenderSystem* renderSystem, IXRAPI* xr, GameWindow* window, entt::registry& registry, Options options, entt::entity sceneRoot)
{
	plat->GetInputSystem()->GetKeyPressed();
	graph->NewGuiFrame();
	plat->NewGuiFrame();
	ImGui::NewFrame();

	auto camEntityView = registry.view<Camera>();
	auto [camera, camTransform] = registry.get<Camera, Transform>(camEntityView[0]);

	TransformSystem::CalculateWorldMatrix(&camTransform);
	CameraSystem::CalculateViewMatrixLH(camera, camTransform);


	// TODO: Put the picking code in its own system
	if (plat->GetInputSystem()->WasMouseButtonClicked(MouseButton::Left))
	{
		MakeRayFromCamera(camera, registry.view<AABB, Transform>(), window, plat->GetInputSystem()->GetCursorPosition());
	}

	auto entityView = registry.view<Name, Relationship, Transform>();
	EntityListWindow entityListWindow;
	InspectorWindow inspectorWindow;
	WindowHeader windowHeader;
	RendererMenu();
	Relationship& rootRel = registry.get<Relationship>(sceneRoot);
	entityListWindow.Render(rootRel, entityView);
	inspectorWindow.Render(registry);
	const char* meshPath = windowHeader.Render();
	if (meshPath)
	{
		Tree<MeshCreateInfo> meshTree = MeshLoaderSystem::CreateMeshHeirarchy(plat->GetAssetManager()->GetAssetPath(meshPath).c_str());
		entt::entity meshEntity = MakeMesh(meshTree, registry, sceneRoot);
		Transform newMeshTransform = {};
		newMeshTransform.position = camTransform.position + (-camTransform.orientation) * glm::vec3(0, 0, 10);
		registry.replace<Transform>(meshEntity, newMeshTransform);
	}

	graph->ClearScreen();
	entt::entity selected = GizmoSystem::GetSelectedEntity();
	ComputeTransformHeirarchy(sceneRoot, registry, Transform());
	LightsSystem::LoadPointLightPositions(registry.view<PointLight, Transform>());
	LightsSystem::LoadDirLightDirections(registry.view<DirectionalLight, Transform>());
	if (xr->IsSessionRunning())
	{
		xr->Frame(registry, renderSystem);
	}
	xr->PollEvents();
	graph->BindToScreen();
#ifdef _WIN64
	if (options.graphicsAPI == PE::GraphicsAPI::DirectX11)
	{
		DrawIteration<DirectXRenderer>(camera, selected, registry, renderSystem);
	}
#endif
	if (options.graphicsAPI == PE::GraphicsAPI::OpenGL)
	{
		DrawIteration<GLRenderer>(camera, selected, registry, renderSystem);
	}
	auto transformView = registry.view<Transform>();

	EngineCameraControllerSystem::ControlCamera(plat->GetInputSystem(), camTransform);

	if (plat->GetInputSystem()->IsKeyPressed(KeyboardCode::Esc))
	{
		GizmoSystem::DeselectAll();
	}
	if (selected != entt::null && registry.any_of<Renderable>(selected))
	{
		RendererMenu().Render(registry, selected, renderSystem);
	}
	ImGui::Render();
	graph->DrawGui();
	graph->_SwapBuffers();
	if (selected != entt::null && plat->GetInputSystem()->IsKeyPressed(KeyboardCode::Delete))
	{
		GizmoSystem::DeselectAll();
		registry.destroy(selected);
	}
}

void ComputeTransformHeirarchy(entt::entity parent, entt::registry& registry, Transform cumulativeTransform)
{
	Transform entityTransform = registry.get<Transform>(parent);
	glm::mat4 translationMat = glm::translate(glm::mat4(1.0), entityTransform.position);
	glm::mat4 rotationMat = glm::mat4(entityTransform.orientation);
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), entityTransform.scale);
	glm::mat4 localMatrix = translationMat * rotationMat * scaleMat;
	entityTransform.worldMatrix = cumulativeTransform.worldMatrix * localMatrix;
	registry.replace<Transform>(parent, entityTransform);
}
