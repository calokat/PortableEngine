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
#include "Camera.h"
#include <entt.hpp>

IPlatform* plat;
IGraphicsAPI* graph;
entt::registry registry;
void Loop()
{
	auto view = registry.view<Mesh, Renderer>();
	graph->ClearScreen();
	for (auto renderable : view)
	{
		Renderer& renderer = registry.get<Renderer>(renderable);
		//Mesh& entMesh = view.get<Mesh>(renderable);
		//Renderer& entRenderer = view.get<Renderer>(renderable);
		//renderer.LoadMesh(mesh.GetRawVertices());
		renderer.Update();
		renderer.Draw();
		//graph->Draw();
	}
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

	float camMoveSpeed = .05f;
	glm::vec2 prevCursorPos{-1, -1}, currentCursorPos;


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

	Mesh& mesh = registry.emplace<Mesh>(entity, plat->GetAssetPath("../../Assets/Models/torus.obj").c_str());
	Renderer& renderer = registry.emplace<Renderer>(entity, plat, &cam);
	renderer.LoadMesh(mesh.GetRawVertices());
	plat->GetInputSystem()->RegisterRightMouseFunction([&cam]()
	{
		glm::vec2 delta = plat->GetInputSystem()->GetCursorPosition() - plat->GetInputSystem()->GetPreviousCursorPosition();
		printf("Delta: %f, %f\n", delta.x, delta.y);
		cam.GetTransform()->Rotate(glm::vec3(delta.y * .005f, -delta.x * .005f, 0));
	});
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(Loop, 0, 1);
#else
	auto MoveCameraForward = [&cam, camMoveSpeed]() {
		cam.GetTransform()->MoveRelative(glm::vec3(0, 0, 1 * camMoveSpeed));
	};
	auto MoveCameraBackward = [&cam, camMoveSpeed]() {
		cam.GetTransform()->MoveRelative(glm::vec3(0, 0, -1 * camMoveSpeed));
	};
	auto MoveCameraLeft = [&cam, camMoveSpeed]() {
		cam.GetTransform()->MoveRelative(glm::vec3(1 * camMoveSpeed, 0, 0));
	};
	auto MoveCameraRight = [&cam, camMoveSpeed]() {
		cam.GetTransform()->MoveRelative(glm::vec3(-1 * camMoveSpeed, 0, 0));
	};
	plat->GetInputSystem()->RegisterKeyPressFunction('W', MoveCameraForward);
	plat->GetInputSystem()->RegisterKeyPressFunction('S', MoveCameraBackward);
	plat->GetInputSystem()->RegisterKeyPressFunction('A', MoveCameraLeft);
	plat->GetInputSystem()->RegisterKeyPressFunction('D', MoveCameraRight);

	//plat->GetInputSystem()->RegisterRightMouseFunction([&prevCursorPos, &currentCursorPos, &cam]() {
	//	if (prevCursorPos.x == -1)
	//	{
	//		prevCursorPos = currentCursorPos;
	//	}
	//	glm::vec2 delta = currentCursorPos - prevCursorPos;
	//	cam.GetTransform()->Rotate(glm::vec3(delta.x * .01f, -delta.y * .01f, 0));
	//	prevCursorPos = currentCursorPos;
	//});
	while (plat->Run() == 0)
	{
		currentCursorPos = plat->GetInputSystem()->GetCursorPosition();
		plat->GetInputSystem()->GetKeyPressed();
		auto view = registry.view<Mesh, Renderer>();
		graph->ClearScreen();
		for (auto renderable : view)
		{
			renderer.Update();
			renderer.Draw();
		}
		graph->_SwapBuffers();
	}
#endif
	delete window;
	delete plat;
	delete graph;
	return 0;
}
