#ifdef _WIN64
#include <Windows.h>
#include "WindowsPlatform.h"
#include "DirectXAPI.h"
#include "DirectXRenderer.h"
#include "DirectXRenderSystem.h"
#endif
#ifdef __EMSCRIPTEN__
#include "EmscriptenPlatform.h"
#include <emscripten/html5.h>
#endif
#ifdef __linux__
#include "LinuxPlatform.h"
#endif
#include "OpenGLAPI.h"
#include "Mesh.h"
#include "GLRenderer.h"
#include "GLRenderSystem.h"
#include "Camera.h"
#include <entt/entt.hpp>
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
#include "WindowHeader.h"
#ifdef _WIN64
#include "../PE_XR/XRAPI.h"
#elif defined(__EMSCRIPTEN__)
#include "WebXRAPI.h"
#endif
#include "MockXRAPI.h"
#include <thread>
#include "raycast.h"
#include "loop.h"
#include "Tree.h"
#include "Relationship.h"
#include "MeshMaker.h"
#include "BillboardSystem.h"
#include "LightsSystem.h"
#include "Pathfinder.h"

void UpdateChildren(entt::registry& registry, entt::entity e)
{
	Transform currentTransform = registry.get<Transform>(e);
	Relationship& entityRel = registry.get<Relationship>(e);
	for (auto child = entityRel.children.begin(); child != entityRel.children.end(); ++child)
	{
		ComputeTransformHeirarchy(child->second, registry, currentTransform);
	}
}

void RemoveFromHeirarchy(entt::registry& registry, entt::entity e)
{
	Relationship toRemove = registry.get<Relationship>(e);
	Relationship affectedParent = registry.get<Relationship>(toRemove.parent);
	for (auto it = toRemove.children.begin(); it != toRemove.children.end(); ++it)
	{
		registry.destroy(it->second);
	}
	affectedParent.children.erase((int)e);
	registry.replace<Relationship>(toRemove.parent, affectedParent);
}

int main(int argc, char* argv[])
{
#if defined(DEBUG) | defined(_DEBUG)
	// Enable memory leak detection as a quick and dirty
	// way of determining if we forgot to clean something up
	//  - You may want to use something more advanced, like Visual Leak Detector
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	
	Options options;
	IPlatform* plat = nullptr;
	IGraphicsAPI* graph = nullptr;
	GameWindow* window = nullptr;
	IRenderSystem* renderSystem = nullptr;
	IXRAPI* xr = nullptr;
	entt::registry registry;


	// Add basic dependencies
	registry.on_construct<Transform>().connect<&entt::registry::emplace_or_replace<Relationship>>();
	registry.on_construct<PointLight>().connect<&entt::registry::emplace_or_replace<Transform>>();
	registry.on_construct<DirectionalLight>().connect<&entt::registry::emplace_or_replace<Transform>>();
	registry.on_construct<SpotLight>().connect<&entt::registry::emplace_or_replace<Transform>>();
	registry.on_destroy<Relationship>().connect<RemoveFromHeirarchy>();
	registry.on_update<Transform>().connect<UpdateChildren>();
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
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	if (argc < 4)
	{
		#ifdef _WIN64
		options.platform = PE::Platform::Win32;
		#endif
		#ifdef __linux__
		options.platform = PE::Platform::Linux;
		#endif
		options.graphicsAPI = PE::GraphicsAPI::OpenGL;
	}
	else
	{
		for (int i = 0; i < argc; ++i)
		{
			if (!strcmp(argv[i], "-p"))
			{
				if (!strcmp(argv[i + 1], "Windows"))
				{
					options.platform = PE::Platform::Win32;
				}
				else if (!strcmp(argv[i + 1], "Web"))
				{
					options.platform = PE::Platform::Web;
				}
			}
			if (!strcmp(argv[i], "-g"))
			{
				if (!strcmp(argv[i + 1], "DX11"))
				{
					options.graphicsAPI = PE::GraphicsAPI::DirectX11;
				}
				else if (!strcmp(argv[i + 1], "OpenGL"))
				{
					options.graphicsAPI = PE::GraphicsAPI::OpenGL;
				}
			}
			if (!strcmp(argv[i], "-xr"))
			{
				options.xr = PE::XrPlatform::OpenXR;
			}
		}
	}

#ifdef _WIN64
	if (options.platform == PE::Platform::Win32)
	{
		plat = new WindowsPlatform(window);
	}
#elif defined __EMSCRIPTEN__
	if (options.platform == PE::Platform::Web)
	{
		plat = new EmscriptenPlatform(window);
	}
#elif defined __linux__
	if (options.platform == PE::Platform::Linux)
	{
		plat = new LinuxPlatform(window);
	}
#endif
#ifdef _WIN64
	if (options.graphicsAPI == PE::GraphicsAPI::DirectX11)
	{
		graph = new DirectXAPI(window, cam);
		registry.on_construct<DirectXRenderer>().connect<&entt::registry::emplace_or_replace<Renderable>>();
	}
#endif
	if (options.graphicsAPI == PE::GraphicsAPI::OpenGL)
	{
		graph = new OpenGLAPI(window, plat, cam);
		registry.on_construct<GLRenderer>().connect<&entt::registry::emplace_or_replace<Renderable>>();
	}

	PathfinderConfig config;
	std::string runTimePathStr = plat->GetAssetManager()->GetAssetPath("");
	config.pathToRuntime = runTimePathStr.c_str();

	Pathfinder::Configure(config);

	plat->InitWindow();
	graph->Init();

	entt::delegate<void()> onResizeDelegate;
	onResizeDelegate.connect<&IGraphicsAPI::OnResize>(graph);
	plat->SetWindowResizeCallback(onResizeDelegate);

#ifdef _WIN64
	if (options.graphicsAPI == PE::GraphicsAPI::DirectX11)
	{
		renderSystem = new DirectXRenderSystem(((DirectXAPI*)(graph))->device.Get(), ((DirectXAPI*)(graph))->context.Get());
	}
#endif
	if (options.graphicsAPI == PE::GraphicsAPI::OpenGL)
	{
		renderSystem = new GLRenderSystem(plat);
	}
	if (options.xr == PE::XrPlatform::NoXR)
	{
		xr = new MockXRAPI();
	}
#ifdef _WIN64
	else if (options.xr == PE::XrPlatform::OpenXR)
	{
		xr = new XRAPI(plat, graph, window, options);
	}
#endif
	registry.on_construct<Mesh>().connect<&IRenderSystem::CreateDefaultRenderer>(renderSystem);
	entt::entity root = registry.create();
	registry.emplace<Name>(root, "$");
	registry.emplace<Transform>(root);
	GizmoSystem::Select(root);
	Tree<MeshCreateInfo> duoScene = MeshLoaderSystem::CreateMeshHeirarchy(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/stage.fbx").c_str());
	entt::entity duoRoot = MakeMesh(duoScene, registry, root);

	Relationship& duoRootRel = registry.get<Relationship>(duoRoot);

	entt::entity pointLightEntity = LightsSystem::CreatePointLight(registry);
	Relationship& pointLightRel = registry.get<Relationship>(pointLightEntity);
	entt::entity dirLightEntity = LightsSystem::CreateDirectionalLight(registry);
	Relationship& dirLightRel = registry.get<Relationship>(dirLightEntity);

	pointLightRel.parent = root;
	Relationship& rootRel = registry.get<Relationship>(root);
	rootRel.children.insert(std::pair((int)pointLightEntity, pointLightEntity));

	dirLightRel.parent = root;
	rootRel.children.insert(std::pair((int)dirLightEntity, dirLightEntity));

	entt::entity spotLightEntity = LightsSystem::CreateSpotLight(registry);
	Relationship& spotLightRel = registry.get<Relationship>(spotLightEntity);
	//Relationship& rootRel = registry.get<Relationship>(root);
	spotLightRel.parent = root;
	rootRel.children.insert(std::pair((int)spotLightEntity, spotLightEntity));

	if (options.xr == PE::XrPlatform::OpenXR)
	{
		Tree<MeshCreateInfo> xrHand = MeshLoaderSystem::CreateMeshHeirarchy(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/cube.obj").c_str());
		entt::entity leftHand = MakeMesh(xrHand, registry, root);
		Transform handTransform = {};
		entt::entity rightHand = MakeMesh(xrHand, registry, root);
		handTransform.scale = glm::vec3(.1f, .1f, .1f);
		TransformSystem::CalculateWorldMatrix(&handTransform);
		registry.replace<Transform>(leftHand, handTransform);
		registry.replace<Transform>(rightHand, handTransform);

		registry.emplace<XRDevice>(leftHand, XRDeviceType::LeftHand);
		registry.emplace_or_replace<Name>(leftHand, "Left Hand");
		registry.emplace<XRDevice>(rightHand, XRDeviceType::RightHand);
		registry.emplace_or_replace<Name>(rightHand, "Right Hand");
	}
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
