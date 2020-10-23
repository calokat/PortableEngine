#ifdef _WIN64
#include <Windows.h>
#include "WindowsPlatform.h"
#include "DirectXAPI.h"
#endif
#ifdef __EMSCRIPTEN__
#include "EmscriptenPlatform.h"
#include <emscripten/html5.h>
#endif
#include "OpenGLAPI.h"
#include "PortableGame.h"
#include "Mesh.h"
#include "Renderer.h"
#include "RenderSystem.h"
#include "Camera.h"
#include <entt.hpp>
#include <imgui.h>
#include <examples/imgui_impl_win32.h>
#include <examples/imgui_impl_opengl3.h>
#include <ImGuizmo.h>
#include "TransformSystem.h"
#include "CameraSystem.h"
#include "GizmoSystem.h"

IPlatform* plat;
IGraphicsAPI* graph;
entt::registry registry;

bool show_demo_window = true;

void Loop()
{
	ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplWin32_NewFrame();
	plat->NewGuiFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	ImGuizmo::Enable(true);
	static float vertColorPick[4];
	ImGui::Begin("Vertex color picker");
	ImGui::Text("Vertex color: ");
	ImGui::ColorPicker4("Vertex color: ", vertColorPick);
	ImGui::End();

	plat->GetInputSystem()->GetKeyPressed();
	auto view = registry.view<Mesh, Renderer, Transform>();
	graph->ClearScreen();
	auto camEntityView = registry.view<Camera>();
	//Camera& camera = registry.get<Camera>(camEntityView[0]);
	auto [camera, camTransform] = registry.get<Camera, Transform>(camEntityView[0]);
	CameraSystem::CalculateViewMatrix(camera, camTransform);
	for (auto renderable : view)
	{
		Renderer& renderer = registry.get<Renderer>(renderable);
		Mesh& mesh = registry.get<Mesh>(renderable);
		Transform& meshTransform = registry.get<Transform>(renderable);
		for (auto it = mesh.GetRawVertices().begin(); it != mesh.GetRawVertices().end(); ++it)
		{
			it->Color = { vertColorPick[0], vertColorPick[1], vertColorPick[2], vertColorPick[3] };
		}
		LoadMesh(renderer, mesh);
		//renderer.Update();
		//renderer.Draw();
		UpdateRenderer(renderer, meshTransform);
		Draw(renderer);
	}
	//camera.transform = transform;
	GizmoSystem::DrawGizmo(camera);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	graph->_SwapBuffers();
}

int main(int argc, char* argv[])
{
#if defined(DEBUG) | defined(_DEBUG)
	// Enable memory leak detection as a quick and dirty
	// way of determining if we forgot to clean something up
	//  - You may want to use something more advanced, like Visual Leak Detector
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	GameWindow* window = new GameWindow(0, 0, 800, 600);
	auto entity = registry.create();
	auto entityTwo = registry.create();
	auto cameraEntity = registry.create();

	Transform& camTransform = registry.emplace<Transform>(cameraEntity);
	Camera& cam = registry.emplace<Camera>(cameraEntity, (float)window->width / window->height);
	camTransform.position += glm::vec3(0, 0, -3);
	//cam.transform = &camTransform;
	CameraSystem::CalculateProjectionMatrix(cam, (float)window->width / window->height);
	//Camera cam = Camera(glm::vec3(0, 0, -3), (float)window->width / window->height);

	float camMoveSpeed = .05f;
	glm::vec2 prevCursorPos{-1, -1}, currentCursorPos;

	// (Try to) Setup IMGUI	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();


#ifdef _WIN64
	plat = new WindowsPlatform(window);
#elif defined __EMSCRIPTEN__
	plat = new EmscriptenPlatform(window);
#endif
#ifdef _WIN64
	graph = new OpenGLAPI(window, plat);
#elif defined __EMSCRIPTEN__
	graph = new OpenGLAPI(window, plat);
#endif
	plat->InitWindow();
	graph->Init();

	Mesh& mesh = registry.emplace<Mesh>(entity, plat->GetAssetPath("../../Assets/Models/cone.obj").c_str());
	Renderer& renderer = registry.emplace<Renderer>(entity, plat, &cam);
	Transform& t1 = registry.emplace<Transform>(entity);
	Load(renderer);
	//renderer.LoadMesh(mesh.GetRawVertices());
	LoadMesh(renderer, mesh);

	GizmoSystem::Select(&t1);

	Mesh& helix = registry.emplace<Mesh>(entityTwo, plat->GetAssetPath("../../Assets/Models/helix.obj").c_str());
	Renderer& rendererTwo = registry.emplace<Renderer>(entityTwo, plat, &cam);
	Load(rendererTwo);
	//rendererTwo.LoadMesh(helix.GetRawVertices());
	LoadMesh(rendererTwo, helix);

	plat->GetInputSystem()->RegisterRightMouseFunction([]()
	{
		auto camView = registry.view<Camera>();
		auto [camera, camTransform] = registry.get<Camera, Transform>(camView[0]);
		glm::vec2 delta = plat->GetInputSystem()->GetCursorPosition() - plat->GetInputSystem()->GetPreviousCursorPosition();
		float camRotX = camTransform.rotation.x;
		bool tooFarUp = camRotX > 3.f / 2;
		bool tooFarDown = camRotX < -3.f / 2;
		// checks to see if camera is in danger of gimbal lock
		if (tooFarUp || tooFarDown)
		{
			float newCamRotX;
			// we allow the input to rotate the camera on the x axis
			// if it is "too far down" and going up or "too far up" 
			// and going down. Otherwise zero out the x axis input
			if ((tooFarUp && delta.y < 0) || (tooFarDown && delta.y > 0))
			{
				newCamRotX = delta.y;
			}
			else
			{
				newCamRotX = 0;
			}
			//cam.GetTransform()->Rotate(glm::vec3(newCamRotX * .005f, -delta.x * .005f, 0));
			TransformSystem::Rotate(glm::vec3(newCamRotX * .005f, -delta.x * .005f, 0), &camTransform);
		}
		else
		{
			//cam.GetTransform()->Rotate(glm::vec3(delta.y * .005f, -delta.x * .005f, 0));
			TransformSystem::Rotate(glm::vec3(delta.y * .005f, -delta.x * .005f, 0), &camTransform);
		}
	});

	auto MoveCamera = [](glm::vec3 dir) {
		return[dir]() {
			auto camView = registry.view<Camera>();
			auto [camera, camTransform] = registry.get<Camera, Transform>(camView[0]);
			TransformSystem::MoveRelative(dir, &camTransform);
		};
	};

	plat->GetInputSystem()->RegisterKeyPressFunction('w', MoveCamera(glm::vec3(0, 0, 1 * camMoveSpeed)));
	plat->GetInputSystem()->RegisterKeyPressFunction('s', MoveCamera(glm::vec3(0, 0, -1 * camMoveSpeed)));
	plat->GetInputSystem()->RegisterKeyPressFunction('a', MoveCamera(glm::vec3(1 * camMoveSpeed, 0, 0)));
	plat->GetInputSystem()->RegisterKeyPressFunction('d', MoveCamera(glm::vec3(-1 * camMoveSpeed, 0, 0)));
	plat->GetInputSystem()->RegisterKeyPressFunction('q', MoveCamera(glm::vec3(0, -1 * camMoveSpeed, 0)));
	plat->GetInputSystem()->RegisterKeyPressFunction('e', MoveCamera(glm::vec3(0, 1 * camMoveSpeed, 0)));

	// gizmo keyboard controls
	plat->GetInputSystem()->RegisterKeyPressFunction('r', []() {GizmoSystem::op = ImGuizmo::SCALE; });
	//plat->GetInputSystem()->RegisterKeyPressFunction('e', []() {GizmoSystem::op = ImGuizmo::ROTATE; });
	//plat->GetInputSystem()->RegisterKeyPressFunction('w', []() {GizmoSystem::op = ImGuizmo::TRANSLATE; });

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(Loop, 0, 1);
#else
	while (plat->Run() == 0)
	{
		Loop();
	}
#endif
	delete window;
	delete plat;
	delete graph;
	ImGui::DestroyContext();
	return 0;
}
