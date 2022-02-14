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
#include "OpenGLAPI.h"
#include "Mesh.h"
#include "GLRenderer.h"
#include "GLRenderSystem.h"
#include "Camera.h"
#include <entt.hpp>
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
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
#ifndef __EMSCRIPTEN__
#include "../PE_XR/XRAPI.h"
#else
#include "../PE_XR/WebXRAPI.h"
#endif
#include "../PE_XR/MockXRAPI.h"
#include <thread>
#include "raycast.h"
#include "loop.h"
#include "Tree.h"
#include "Relationship.h"
#include "MeshMaker.h"
#include "BillboardSystem.h"
#include "LightsSystem.h"

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


	if (argc < 4)
	{
		options.platform = PE::Platform::Web;
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
#endif
#ifdef _WIN64
	if (options.graphicsAPI == PE::GraphicsAPI::DirectX11)
	{
		graph = new DirectXAPI(window, cam);
	}
	else
	{
		graph = new OpenGLAPI(window, plat, cam);
	}
#elif defined __EMSCRIPTEN__
	graph = new OpenGLAPI(window, plat, cam);
#endif

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
	//xr = new XRAPI(plat, graph, window);
	if (options.xr == PE::XrPlatform::NoXR)
	{
		xr = new MockXRAPI();
	}
	else if (options.xr == PE::XrPlatform::OpenXR)
	{
		xr = new XRAPI(plat, graph, window, options);
	}

	entt::entity root = registry.create();
	registry.emplace<Name>(root, "$");
	registry.emplace<Transform>(root);
	registry.emplace<Relationship>(root);
	GizmoSystem::Select(root);
	Tree<MeshCreateInfo> duoScene = MeshLoaderSystem::CreateMeshHeirarchy(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/duo.fbx").c_str());
	entt::entity duoRoot = MakeMesh(duoScene, registry, root);

	Relationship& duoRootRel = registry.get<Relationship>(duoRoot);

	for (auto it = duoRootRel.children.begin(); it != duoRootRel.children.end(); it++)
	{
		AttachRenderers(registry, renderSystem, { { "diffuse",  plat->GetAssetManager()->GetAssetPath("../../Assets/Images/rock.png").c_str() }, { "normal",  plat->GetAssetManager()->GetAssetPath("../../Assets/Images/Normal/rock_normals.png").c_str() } }, it->second, ShaderType::Lit_Textured_Normal);
	}



	entt::entity pointLightEntity = LightsSystem::CreatePointLight(registry);
	Relationship& pointLightRel = registry.get<Relationship>(pointLightEntity);
	entt::entity dirLightEntity = LightsSystem::CreateDirectionalLight(registry);
	Relationship& dirLightRel = registry.get<Relationship>(dirLightEntity);

	pointLightRel.parent = root;
	Relationship& rootRel = registry.get<Relationship>(root);
	rootRel.children.insert(std::pair((int)pointLightEntity, pointLightEntity));

	dirLightRel.parent = root;
	rootRel = registry.get<Relationship>(root);
	rootRel.children.insert(std::pair((int)dirLightEntity, dirLightEntity));
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
