#pragma once
#include "loop.h"
#include <imgui.h>
#include "GizmoSystem.h"
#include "raycast.h"
#include "EntityListWindow.h"
#include "InspectorWindow.h"
#include "AssetBrowserWindow.h"
#include "WindowHeader.h"
#include "TransformSystem.h"
#include "CameraSystem.h"
#include <thread>
#include "EngineCameraControllerSystem.h"
#include "GLRenderer.h"
#ifdef _WIN64
#include "DIrectXRenderer.h"
#endif
template<class T>
void DrawIteration(Camera& camera, entt::entity selected, entt::registry& registry, IRenderSystem* renderSystem)
{
	auto renderableView = registry.view<T, Transform>();
	for (auto renderable : renderableView)
	{
		T& renderer = registry.get<T>(renderable);
		Mesh& mesh = registry.get<Mesh>(renderable);
		Transform& meshTransform = registry.get<Transform>(renderable);
		//renderSystem->LoadMesh(&renderer, mesh);
		renderSystem->BindRenderer(&renderer);
		renderSystem->UpdateRenderer(&renderer, meshTransform, camera);
		renderSystem->Draw(&renderer);
	}
	if (selected != entt::null)
	{
		Mesh& selectedMesh = registry.get<Mesh>(selected);
		T& r = renderableView.template get<T>(selected);
		//renderSystem->LoadMesh(&r, selectedMesh);
		renderSystem->BindRenderer(&r);
		renderSystem->DrawWireframe(&r);
	}
}

void Loop(IPlatform* plat, IGraphicsAPI* graph, IRenderSystem* renderSystem, IXRAPI* xr, GameWindow* window, entt::registry& registry, Options options)
{
	plat->GetInputSystem()->GetKeyPressed();
	graph->NewGuiFrame();
	plat->NewGuiFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	ImGuizmo::Enable(true);
	static float vertColorPick[4];
	//	ImGui::BeginMainMenuBar();
	//	if (ImGui::BeginMenu("File"))
	//	{
	//		if (ImGui::BeginMenu("New"))
	//		{
	//			if (ImGui::MenuItem("Cube"))
	//			{
	//				MakeMesh(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/cube.obj").c_str(), "Cube");
	//			}
	//			if (ImGui::MenuItem("Helix"))
	//			{
	//				MakeMesh(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/helix.obj").c_str(), "Helix");
	//			}
	//			if (ImGui::MenuItem("Cone"))
	//			{
	//				MakeMesh(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/cone.obj").c_str(), "Cone");
	//			}
	//			if (ImGui::MenuItem("Cylinder"))
	//			{
	//				MakeMesh(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/cylinder.obj").c_str(), "Cylinder");
	//			}
	//			if (ImGui::MenuItem("Sphere"))
	//			{
	//				MakeMesh(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/sphere.obj").c_str(), "Sphere");
	//			}
	//			if (ImGui::MenuItem("Torus"))
	//			{
	//				MakeMesh(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/torus.obj").c_str(), "Torus");
	//			}
	//			ImGui::EndMenu();
	//		}
	//		//if (ImGui::BeginMenu("Save"))
	////{
	////	static char saveFileName[100] = {};
	////	ImGui::InputText("Save as: ", saveFileName, 100);
	////	if (ImGui::Button("Save"))
	////	{
	////		Serialize(saveFileName);
	////	}
	////	ImGui::EndMenu();
	////}
	////if (ImGui::BeginMenu("Open"))
	////{
	////	for (const auto& saveFile : std::filesystem::directory_iterator("./"))
	////	{
	////		if (saveFile.path().extension().generic_string() == ".pg")
	////		{
	////			std::string fileStr = saveFile.path().generic_string();
	//
	////			if (ImGui::MenuItem(fileStr.c_str()))
	////			{
	////				Deserialize(fileStr.c_str());
	////			}
	////		}
	////	}
	////	ImGui::EndMenu();
	////}
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMainMenuBar();

		// TODO: Put the picking code in its own system
	if (plat->GetInputSystem()->WasMouseButtonClicked(MouseButton::Left))
	{
		MakeRayFromCamera(registry.view<Camera, Transform>(), registry.view<AABB>(), window);
	}

	auto entityView = registry.view<Name, Transform>();
	EntityListWindow entityListWindow;
	InspectorWindow inspectorWindow;
	AssetBrowserWindow assetWindow;
	WindowHeader windowHeader;

	windowHeader.Render(registry, plat->GetAssetManager(), renderSystem);
	entityListWindow.Render(entityView);
	inspectorWindow.Render(registry);

	assetWindow.Render(plat->GetAssetManager(), renderSystem);

	graph->ClearScreen();
	auto camEntityView = registry.view<Camera>();
	auto [camera, camTransform] = registry.get<Camera, Transform>(camEntityView[0]);
	ImGui::Begin("Camera Rotation");
	ImGui::DragFloat3("Camera rotation", glm::value_ptr(camTransform.rotation));
	ImGui::End();

	TransformSystem::CalculateWorldMatrix(&camTransform);
	CameraSystem::CalculateViewMatrixLH(camera, camTransform);
	entt::entity selected = GizmoSystem::GetSelectedEntity();
	if (xr->IsSessionRunning())
	{
		xr->RenderFrame(registry, renderSystem);
	}
	else
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
	xr->PollEvents();
	graph->BindToScreen();
	// Note: I do not know why, but when I use CalculateProjectionMatrixLH the view on screen is backwards, even though
	// I use CalculateProjectionMatrixLH for almost everything
	//CameraSystem::CalculateProjectionMatrixRH(camera, (float)window->width / window->height);
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
	//GizmoSystem::UpdateGizmo(plat->GetInputSystem());
	//GizmoSystem::DrawGizmo(camera, transformView);

	EngineCameraControllerSystem::ControlCamera(plat->GetInputSystem(), camTransform);

	if (plat->GetInputSystem()->IsKeyPressed(KeyboardCode::Esc))
	{
		GizmoSystem::DeselectAll();
	}
	ImGui::Render();
	graph->DrawGui();
	graph->_SwapBuffers();
}