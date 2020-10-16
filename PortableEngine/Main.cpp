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
	ImGui::ShowDemoWindow(&show_demo_window);

	plat->GetInputSystem()->GetKeyPressed();
	auto view = registry.view<Mesh, Renderer>();
	graph->ClearScreen();
	for (auto renderable : view)
	{
		Renderer& renderer = registry.get<Renderer>(renderable);
		//renderer.Update();
		//renderer.Draw();
		UpdateRenderer(renderer);
		Draw(renderer);
	}
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
	Camera cam = Camera(glm::vec3(0, 0, -3), (float)window->width / window->height);
	auto entity = registry.create();
	auto entityTwo = registry.create();

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
	Load(renderer);
	//renderer.LoadMesh(mesh.GetRawVertices());
	LoadMesh(renderer, mesh);

	Mesh& helix = registry.emplace<Mesh>(entityTwo, plat->GetAssetPath("../../Assets/Models/helix.obj").c_str());
	Renderer& rendererTwo = registry.emplace<Renderer>(entityTwo, plat, &cam);
	Load(rendererTwo);
	//rendererTwo.LoadMesh(helix.GetRawVertices());
	LoadMesh(rendererTwo, helix);

	plat->GetInputSystem()->RegisterRightMouseFunction([&cam]()
	{
		glm::vec2 delta = plat->GetInputSystem()->GetCursorPosition() - plat->GetInputSystem()->GetPreviousCursorPosition();
		cam.GetTransform()->Rotate(glm::vec3(delta.y * .005f, -delta.x * .005f, 0));
	});

	auto MoveCamera = [&cam](glm::vec3 dir) {
		return[&cam, dir]() {
			cam.GetTransform()->MoveRelative(dir);
		};
	};

	plat->GetInputSystem()->RegisterKeyPressFunction('w', MoveCamera(glm::vec3(0, 0, 1 * camMoveSpeed)));
	plat->GetInputSystem()->RegisterKeyPressFunction('s', MoveCamera(glm::vec3(0, 0, -1 * camMoveSpeed)));
	plat->GetInputSystem()->RegisterKeyPressFunction('a', MoveCamera(glm::vec3(1 * camMoveSpeed, 0, 0)));
	plat->GetInputSystem()->RegisterKeyPressFunction('d', MoveCamera(glm::vec3(-1 * camMoveSpeed, 0, 0)));
	plat->GetInputSystem()->RegisterKeyPressFunction('q', MoveCamera(glm::vec3(0, -1 * camMoveSpeed, 0)));
	plat->GetInputSystem()->RegisterKeyPressFunction('e', MoveCamera(glm::vec3(0, 1 * camMoveSpeed, 0)));

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
