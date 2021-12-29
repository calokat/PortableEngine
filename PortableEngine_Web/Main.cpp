#include "EmscriptenPlatform.h"
#include <emscripten/html5.h>
#include "OpenGLAPI.h"
#include "Mesh.h"
#include "GLRenderer.h"
#include "GLRenderSystem.h"
#include "Camera.h"
#include <entt.hpp>
#include <imgui.h>
#include "TransformSystem.h"
#include "CameraSystem.h"
#include "GizmoSystem.h"
#include "MeshLoaderSystem.h"
#include <typeinfo>
#include <map>
#include <random>
#include <iomanip>
#include "InspectorGUI.h"
#include "misc_components.h"
// Thanks to https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
#include <filesystem>
#include "AABB.h"
#include "AABBSystem.h"
#include "EngineCameraControllerSystem.h"
#include "ImageSystem.h"
#include "OpenGLImageGraphicsData.h"
#include "EntityListWindow.h"
#include "InspectorWindow.h"
#include "AssetBrowserWindow.h"
#include "WindowHeader.h"
#include "../PE_XR/WebXRAPI.h"
#include "../PE_XR/MockXRAPI.h"
#include <thread>
#include "raycast.h"
#include "loop.h"
#include "Tree.h"
#include "Relationship.h"
#include "MeshMaker.h"

int main(int argc, char* argv[])
{
#if defined(DEBUG) | defined(_DEBUG)
	// Enable memory leak detection as a quick and dirty
	// way of determining if we forgot to clean something up
	//  - You may want to use something more advanced, like Visual Leak Detector
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	Options options;
	//using json = nlohmann::json;
	IPlatform* plat = nullptr;
	IGraphicsAPI* graph = nullptr;
	GameWindow* window = nullptr;
	IRenderSystem* renderSystem = nullptr;
	entt::registry registry;


	window = new GameWindow(0, 0, 800, 600);
	auto entity = registry.create();
	auto cameraEntity = registry.create();

	Transform& camTransform = registry.emplace<Transform>(cameraEntity);
	Camera& cam = registry.emplace<Camera>(cameraEntity, (float)window->width / window->height);
	camTransform.position += glm::vec3(0, 0, -15);
	TransformSystem::CalculateWorldMatrix(&camTransform);
	CameraSystem::CalculateProjectionMatrixLH(cam, (float)window->width / window->height);
	float camMoveSpeed = .05f;
	glm::vec2 prevCursorPos{-1, -1}, currentCursorPos;

	// (Try to) Setup IMGUI	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	options.platform = PE::Platform::Web;
	options.graphicsAPI = PE::GraphicsAPI::OpenGL;

	plat = new EmscriptenPlatform(window);
	graph = new OpenGLAPI(window, plat, cam);
	plat->InitWindow();
	graph->Init();

	entt::delegate<void()> onResizeDelegate;
	onResizeDelegate.connect<&IGraphicsAPI::OnResize>(graph);
	plat->SetWindowResizeCallback(onResizeDelegate);

	renderSystem = new GLRenderSystem(plat);
	IXRAPI* xr = nullptr;
	xr = new WebXRAPI(graph, renderSystem);
	plat->GetAssetManager()->LoadDefaultThumbnails(renderSystem);
	plat->GetAssetManager()->LoadAssetsFromCurrentDirectory(renderSystem);
	
	Tree<MeshCreateInfo> duoScene = MeshLoaderSystem::CreateMeshHeirarchy(plat->GetAssetManager()->GetAssetPath("../../../../Secret_Meshes/duo.fbx").c_str());
	entt::entity root = registry.create();
	registry.emplace<Name>(root, "$");
	registry.emplace<Transform>(root);
	registry.emplace<Relationship>(root);
	GizmoSystem::Select(root);
	entt::entity duoRoot = MakeMesh(duoScene, registry, renderSystem, plat->GetAssetManager(), root);

	
	Tree<MeshCreateInfo> handMeshInfo = MeshLoaderSystem::CreateMeshHeirarchy(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/index_controller.stl").c_str());

	entt::entity leftXRHand = MakeMesh(handMeshInfo, registry, renderSystem, plat->GetAssetManager(), root);
	entt::entity rightXRHand = MakeMesh(handMeshInfo, registry, renderSystem, plat->GetAssetManager(), root);

	Relationship& leftHandRel = registry.get<Relationship>(leftXRHand);
	for (auto it = leftHandRel.children.begin(); it != leftHandRel.children.end(); it++)
	{
		AttachRenderers(registry, renderSystem, plat->GetAssetManager()->GetAssetPath("../../Assets/Images/rock.png").c_str(), it->second, ShaderType::Unlit_Textured);
	}

	Relationship& duoRel = registry.get<Relationship>(duoRoot);

	for (auto it = duoRel.children.begin(); it != duoRel.children.end(); it++)
	{
		AttachRenderers(registry, renderSystem, plat->GetAssetManager()->GetAssetPath("../../Assets/Images/rock.png").c_str(), it->second, ShaderType::Unlit_Textured);
	}

	Transform& leftXRHandTransform = registry.get<Transform>(leftXRHand);
	Transform& rightXRHandTransform = registry.get<Transform>(rightXRHand);
	leftXRHandTransform.scale = { -1, 1, 1 };
	rightXRHandTransform.scale = { 1, 1, 1 };

	registry.emplace<XRDevice>(leftXRHand, XRDeviceType::LeftHand);
	registry.emplace<XRDevice>(rightXRHand, XRDeviceType::RightHand);

	while (plat->Run() == 0)
	{
		Loop(plat, graph, renderSystem, xr, window, registry, options, root);
	}
	onResizeDelegate.reset();
	delete window;
	delete plat;
	delete graph;
	delete renderSystem;
	delete xr;
	ImGui::DestroyContext();
	return 0;
}
