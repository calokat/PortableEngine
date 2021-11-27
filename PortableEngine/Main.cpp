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
#include <json.hpp>
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

//template<class T>
//void TrySerializeComponent(json& master)
//{
//	std::string typeName = typeid(T).name();
//	//std::ofstream jsonFile = std::ofstream(typeName);
//	nlohmann::json json;
//	registry.each([&json](entt::entity e) {
//		if (registry.has<T>(e))
//		{
//			T& obj = registry.get<T>(e);
//			std::string stringified = std::to_string((int)e);
//			json[stringified] = obj;
//		}
//	});
//	//jsonFile << json << std::endl;
//	master[typeName] = json;
//}
//void Serialize(const char* saveFileName)
//{
//	json saveJson;
//	json entitiesJson;
//	std::vector<std::string> stringifiedEntities;
//	registry.each([&stringifiedEntities](entt::entity e)
//	{
//		stringifiedEntities.push_back(std::to_string((int)e));
//	});
//	entitiesJson = stringifiedEntities;
//	//std::ofstream entitiesFile("entities.json");
//	//entitiesFile << entitiesJson << std::endl;
//	saveJson["entities"] = entitiesJson;
//	TrySerializeComponent<Transform>(saveJson);
//	TrySerializeComponent<Mesh>(saveJson);
//	TrySerializeComponent<Camera>(saveJson);
//	TrySerializeComponent<Renderer>(saveJson);
//	TrySerializeComponent<Name>(saveJson);
//	TrySerializeComponent<Rotator>(saveJson);
//	char fullSavePath[150];
//	strcpy_s(fullSavePath, saveFileName);
//	strcat_s(fullSavePath, ".pg");
//	std::ofstream saveFile(fullSavePath);
//	saveFile << std::setw(4) << saveJson << std::endl;
//}
//template<class T>
//void TryDeserializeComponent(std::map<std::string, entt::entity> entityMap, std::vector<std::string> storedEntities, json master)
//{
//	//std::string g = std::string(typeid(T).name()).append(".json");
//	//std::ifstream inFile(g);
//	//json objJson;
//	//inFile >> objJson;
//	json entitiesJson = master["entities"];
//	const char* className = typeid(T).name();
//	json objJson = master[className];
//	for (auto it = storedEntities.begin(); it != storedEntities.end(); ++it)
//	{
//		if (objJson.find(*it) != objJson.end())
//		{
//			auto compIter = objJson[*it];
//			T obj;
//			from_json(compIter, obj);
//			registry.emplace<T>(entityMap[*it], std::move(obj));
//		}
//	}
//}
//void Deserialize(const char* saveFileName)
//{
//	//GizmoSystem::DeselectAll();
//	registry.clear();
//	std::vector<std::string> storedEntities;
//	std::ifstream saveFile(saveFileName);
//	json saveJson;
//	saveFile >> saveJson;
//	json entitiesJson = saveJson["entities"];
//	storedEntities = entitiesJson.get<std::vector<std::string>>();
//	std::map<std::string, entt::entity> entityMap;
//	for (auto it = storedEntities.begin(); it != storedEntities.end(); ++it)
//	{
//		entt::entity e = registry.create();
//		entityMap.emplace(*it, e);
//	}
//	TryDeserializeComponent<Mesh>(entityMap, storedEntities, saveJson);
//	TryDeserializeComponent<Camera>(entityMap, storedEntities, saveJson);
//	TryDeserializeComponent<Renderer>(entityMap, storedEntities, saveJson);
//	TryDeserializeComponent<Transform>(entityMap, storedEntities, saveJson);
//	TryDeserializeComponent<Name>(entityMap, storedEntities, saveJson);
//	TryDeserializeComponent<Rotator>(entityMap, storedEntities, saveJson);
//	auto compView = registry.view<Mesh>();
//	for (auto v : compView)
//	{
//		Mesh& m = registry.get<Mesh>(v);
//		MeshLoaderSystem::LoadMesh(m.path.c_str(), m);
//	}
//	auto rendererView = registry.view<Renderer>();
//	auto cameraView = registry.view<Camera>();
//	Camera& camera = registry.get<Camera>(cameraView[0]);
//	for (auto rv : rendererView)
//	{
//		Renderer& renderer = registry.get<Renderer>(rv);
//		Load(renderer, camera);
//	}
//	auto meshView = registry.view<Mesh, Transform>();
//	Transform& meshTransform = registry.get<Transform>(*meshView.begin());
//	//GizmoSystem::Select(*meshView.begin());
//}

//void MakeMesh(const char* path, const char* name = "GameObject") {
//	auto camView = registry.view<Camera>();
//	auto [camera, camTransform] = registry.get<Camera, Transform>(camView[0]);
//	glm::vec3 newMeshPos = camTransform.position + TransformSystem::CalculateForward(&camTransform);
//	auto newMeshEntity = registry.create();
//	Mesh& newMesh = registry.emplace<Mesh>(newMeshEntity, path);
//	MeshLoaderSystem::LoadMesh(newMesh.path.c_str(), newMesh);
//	Transform& meshTransform = registry.emplace<Transform>(newMeshEntity);
//	meshTransform.position = newMeshPos;
//	TransformSystem::CalculateWorldMatrix(&meshTransform);
//	IRenderer& newMeshRenderer = renderSystem->CreateRenderer(registry, newMeshEntity);
//	renderSystem->Load(&newMeshRenderer, camera);
//	renderSystem->LoadMesh(&newMeshRenderer, newMesh);
//	//newMeshRenderer.diffuseTexture = PEImage(plat->GetAssetManager()->GetAssetPath("../../Assets/Images/cushion.png"));
//	//ImageSystem::CreateImage(newMeshRenderer.diffuseTexture);
//	//renderSystem->CreateTexture(newMeshRenderer.diffuseTexture);
//	renderSystem->LoadTexture(&newMeshRenderer, plat->GetAssetManager()->GetAssetPath("../../Assets/Images/cushion.png")/*newMeshRenderer.diffuseTexture*/);
//	//ImageSystem::DestroyImage(newMeshRenderer.diffuseTexture);
//	Name& nameComp = registry.emplace<Name>(newMeshEntity);
//	nameComp = { name };
//	AABB& aabb = registry.emplace<AABB>(newMeshEntity);
//	AABBSystem::UpdateAABB(aabb, newMesh, meshTransform);
//}
//
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
		IRenderer& newMeshRenderer = renderSystem->CreateRenderer(registry, newMeshEntity);
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

entt::entity MakeMesh(Tree<MeshCreateInfo> meshScene, entt::registry& registry, IRenderSystem* renderSystem, IAssetManager* assetManager, entt::entity meshRoot, glm::vec3 pos = glm::vec3(0, 0, 0))
{
	auto camView = registry.view<Camera>();
	auto [camera, camTransform] = registry.get<Camera, Transform>(camView[0]);
	return MakeMesh_Recursive(registry, meshScene, renderSystem, assetManager, camera, camTransform, meshRoot);
	//Scene<Transform&> s = { camTransform, {{camTransform}} };
}


//void MakeRayFromCamera()
//{
//	auto camView = registry.view<Camera>();
//	auto [camera, camTransform] = registry.get<Camera, Transform>(camView[0]);
//	ImGuiIO& io = ImGui::GetIO();
//	ImVec2 mousePos = io.MousePos;
//	// Blessed be this code taken from https://gamedev.stackexchange.com/questions/157674/simple-mouseray-picking-in-opengl
//	glm::vec3 mouse_world_nearplane = glm::unProject(
//		glm::vec3(mousePos.x, window->height - mousePos.y, 0.0f),
//		camera.view, //view matrix
//		camera.projection,
//		glm::ivec4(0, 0, window->width, window->height));
//
//	glm::vec3 mouse_world_farplane = glm::unProject(
//		glm::vec3(mousePos.x, window->height - mousePos.y, 1.0f),
//		camera.view, //view matrix
//		camera.projection,
//		glm::ivec4(0, 0, window->width, window->height));
//
//	glm::vec3 camray = glm::normalize(mouse_world_farplane - mouse_world_nearplane);
//	camray *= 30;
//	glm::vec3 newMeshLocation = mouse_world_nearplane + camray;
//
//	RaycastAgainstAABB(mouse_world_nearplane, mouse_world_farplane - mouse_world_nearplane, registry.view <AABB>());
//	return;
//}

//template<class T>
//void DrawIteration(Camera& camera, entt::entity selected, entt::registry& registry, IRenderSystem* renderSystem)
//{
//	auto renderableView = registry.view<T, Transform>();
//	for (auto renderable : renderableView)
//	{
//		T& renderer = registry.get<T>(renderable);
//		Mesh& mesh = registry.get<Mesh>(renderable);
//		Transform& meshTransform = registry.get<Transform>(renderable);
//		//renderSystem->LoadMesh(&renderer, mesh);
//		renderSystem->BindRenderer(&renderer);
//		renderSystem->UpdateRenderer(&renderer, meshTransform, camera);
//		renderSystem->Draw(&renderer);
//	}
//	if (selected != entt::null)
//	{
//		Mesh& selectedMesh = registry.get<Mesh>(selected);
//		T& r = renderableView.template get<T>(selected);
//		//renderSystem->LoadMesh(&r, selectedMesh);
//		renderSystem->BindRenderer(&r);
//		renderSystem->DrawWireframe(&r);
//	}
//}
//
//void Loop(IPlatform* plat, IGraphicsAPI* graph, IRenderSystem* renderSystem, IXRAPI* xr, GameWindow* window, entt::registry& registry, Options options)
//{
//	plat->GetInputSystem()->GetKeyPressed();
//	graph->NewGuiFrame();
//	plat->NewGuiFrame();
//	ImGui::NewFrame();
//	ImGuizmo::BeginFrame();
//	ImGuizmo::Enable(true);
//	static float vertColorPick[4];
////	ImGui::BeginMainMenuBar();
////	if (ImGui::BeginMenu("File"))
////	{
////		if (ImGui::BeginMenu("New"))
////		{
////			if (ImGui::MenuItem("Cube"))
////			{
////				MakeMesh(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/cube.obj").c_str(), "Cube");
////			}
////			if (ImGui::MenuItem("Helix"))
////			{
////				MakeMesh(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/helix.obj").c_str(), "Helix");
////			}
////			if (ImGui::MenuItem("Cone"))
////			{
////				MakeMesh(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/cone.obj").c_str(), "Cone");
////			}
////			if (ImGui::MenuItem("Cylinder"))
////			{
////				MakeMesh(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/cylinder.obj").c_str(), "Cylinder");
////			}
////			if (ImGui::MenuItem("Sphere"))
////			{
////				MakeMesh(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/sphere.obj").c_str(), "Sphere");
////			}
////			if (ImGui::MenuItem("Torus"))
////			{
////				MakeMesh(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/torus.obj").c_str(), "Torus");
////			}
////			ImGui::EndMenu();
////		}
////		//if (ImGui::BeginMenu("Save"))
//////{
//////	static char saveFileName[100] = {};
//////	ImGui::InputText("Save as: ", saveFileName, 100);
//////	if (ImGui::Button("Save"))
//////	{
//////		Serialize(saveFileName);
//////	}
//////	ImGui::EndMenu();
//////}
//////if (ImGui::BeginMenu("Open"))
//////{
//////	for (const auto& saveFile : std::filesystem::directory_iterator("./"))
//////	{
//////		if (saveFile.path().extension().generic_string() == ".pg")
//////		{
//////			std::string fileStr = saveFile.path().generic_string();
////
//////			if (ImGui::MenuItem(fileStr.c_str()))
//////			{
//////				Deserialize(fileStr.c_str());
//////			}
//////		}
//////	}
//////	ImGui::EndMenu();
//////}
////		ImGui::EndMenu();
////	}
////	ImGui::EndMainMenuBar();
//
//	// TODO: Put the picking code in its own system
//	if (plat->GetInputSystem()->WasMouseButtonClicked(MouseButton::Left))
//	{
//		MakeRayFromCamera(registry.view<Camera, Transform>(), registry.view<AABB>(), window);
//	}
//	
//	auto entityView = registry.view<Name, Transform>();
//	EntityListWindow entityListWindow;
//	InspectorWindow inspectorWindow;
//	AssetBrowserWindow assetWindow;
//	WindowHeader windowHeader;
//
//	windowHeader.Render(registry, plat->GetAssetManager(), renderSystem);
//	entityListWindow.Render(entityView);
//	inspectorWindow.Render(registry);
//	assetWindow.Render(plat->GetAssetManager(), renderSystem);
//
//	graph->ClearScreen();
//	auto camEntityView = registry.view<Camera>();
//	auto [camera, camTransform] = registry.get<Camera, Transform>(camEntityView[0]);
//	TransformSystem::CalculateWorldMatrix(&camTransform);
//	CameraSystem::CalculateViewMatrixLH(camera, camTransform);
//	entt::entity selected = GizmoSystem::GetSelectedEntity();
//	if (xr->IsSessionRunning())
//	{
//		xr->RenderFrame(registry, renderSystem);
//	}
//	else
//	{
//		std::this_thread::sleep_for(std::chrono::milliseconds(250));
//	}
//	xr->PollEvents();
//	graph->BindToScreen();
//	// Note: I do not know why, but when I use CalculateProjectionMatrixLH the view on screen is backwards, even though
//	// I use CalculateProjectionMatrixLH for almost everything
//	//CameraSystem::CalculateProjectionMatrixRH(camera, (float)window->width / window->height);
//	#ifdef _WIN64
//	if (options.graphicsAPI == PE::GraphicsAPI::DirectX11)
//	{
//		DrawIteration<DirectXRenderer>(camera, selected, registry, renderSystem);
//	}
//	#endif
//	if (options.graphicsAPI == PE::GraphicsAPI::OpenGL)
//	{
//		DrawIteration<GLRenderer>(camera, selected, registry, renderSystem);
//	}
//	auto transformView = registry.view<Transform>();
//	GizmoSystem::UpdateGizmo(plat->GetInputSystem());
//	GizmoSystem::DrawGizmo(camera, transformView);
//
//	EngineCameraControllerSystem::ControlCamera(plat->GetInputSystem(), camTransform);
//	
//	if (plat->GetInputSystem()->IsKeyPressed(KeyboardCode::Esc))
//	{
//		GizmoSystem::DeselectAll();
//	}
//	ImGui::Render();
//	graph->DrawGui();
//	graph->_SwapBuffers();
//}

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
	ImGuiIO& io = ImGui::GetIO();
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
	IXRAPI* xr = nullptr;
#ifdef _WIN64
	xr = new XRAPI(plat, graph, window, options);
#else
	xr = new WebXRAPI(graph, renderSystem);
#endif
	//xr = new WebXRAPI(graph, renderSystem);
	plat->GetAssetManager()->LoadDefaultThumbnails(renderSystem);
	plat->GetAssetManager()->LoadAssetsFromCurrentDirectory(renderSystem);
	//entt::entity assetImageEntity = registry.create();
	//assetThumbnail.assetImageRenderer = renderSystem->CreateRenderer(registry, assetImageEntity);
	//assetThumbnail.assetImage = PEImage(plat->GetAssetManager()->GetAssetPath("../../Assets/Images/directory.png"));
	//ImageSystem::CreateImage(assetThumbnail.assetImage);
	//renderSystem->CreateTexture(assetThumbnail.assetImage);
	//renderSystem->LoadTexture(&assetThumbnail.assetImageRenderer, assetThumbnail.assetImage);	
	
	Tree<MeshCreateInfo> duoScene = MeshLoaderSystem::CreateMeshHeirarchy(plat->GetAssetManager()->GetAssetPath("../../../../Secret_Meshes/duo.fbx").c_str());
	entt::entity root = registry.create();
	registry.emplace<Name>(root, "$");
	registry.emplace<Transform>(root);
	registry.emplace<Relationship>(root);
	GizmoSystem::Select(root);
	entt::entity duoRoot = MakeMesh(duoScene, registry, renderSystem, plat->GetAssetManager(), root);
	//entt::entity subDuo = MakeMesh(duoScene, registry, renderSystem, plat->GetAssetManager(), duoRoot);

	
	Tree<MeshCreateInfo> handMeshInfo = MeshLoaderSystem::CreateMeshHeirarchy(plat->GetAssetManager()->GetAssetPath("../../Assets/Models/index_controller.stl").c_str());

	entt::entity leftXRHand = MakeMesh(handMeshInfo, registry, renderSystem, plat->GetAssetManager(), root);
	entt::entity rightXRHand = MakeMesh(handMeshInfo, registry, renderSystem, plat->GetAssetManager(), root);

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
