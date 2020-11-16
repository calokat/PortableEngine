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
#include "MeshLoaderSystem.h"
#include <json.hpp>
#include <typeinfo>
#include <map>
#include <random>
class RandomColor
{
	bool foo;
public:
	RandomColor() {};
};
struct Name
{
public:
	std::string nameString;
};
IPlatform* plat;
IGraphicsAPI* graph;
GameWindow* window;
entt::registry registry;

bool show_demo_window = true;
using json = nlohmann::json;
void to_json(json& j, const glm::vec3& vec)
{
	j = json{ {"x", vec.x}, {"y", vec.y}, {"z", vec.z} };
}

void from_json(const json& j, glm::vec3& vec)
{
	j.at("x").get_to(vec.x);
	j.at("y").get_to(vec.y);
	j.at("z").get_to(vec.z);
}

void to_json(json& j, const glm::mat4& mat)
{
	j = json{
		{"00", mat[0][0]}, {"01", mat[0][1]}, {"02", mat[0][2]}, {"03", mat[0][3]},
		{"10", mat[1][0]}, {"11", mat[1][1]}, {"12", mat[1][2]}, {"13", mat[1][3]},
		{"20", mat[2][0]}, {"21", mat[2][1]}, {"22", mat[2][2]}, {"23", mat[2][3]},
		{"30", mat[3][0]}, {"31", mat[3][1]}, {"32", mat[3][2]}, {"33", mat[3][3]},
	};
}

void from_json(const json& j, glm::mat4& mat)
{
	j.at("00").get_to(mat[0][0]);
	j.at("01").get_to(mat[0][1]);
	j.at("02").get_to(mat[0][2]);
	j.at("03").get_to(mat[0][3]);
	j.at("10").get_to(mat[1][0]);
	j.at("11").get_to(mat[1][1]);
	j.at("12").get_to(mat[1][2]);
	j.at("13").get_to(mat[1][3]);
	j.at("20").get_to(mat[2][0]);
	j.at("21").get_to(mat[2][1]);
	j.at("22").get_to(mat[2][2]);
	j.at("23").get_to(mat[2][3]);
	j.at("30").get_to(mat[3][0]);
	j.at("31").get_to(mat[3][1]);
	j.at("32").get_to(mat[3][2]);
	j.at("33").get_to(mat[3][3]);
}

void to_json(json& j, const Transform& t)
{
	json worldJson, posJson, rotJson, scaleJson;
	to_json(worldJson, t.worldMatrix);
	to_json(posJson, t.position);
	to_json(rotJson, t.rotation);
	to_json(scaleJson, t.scale);
	j["worldMatrix"] = worldJson;
	j["position"] = posJson;
	j["rotation"] = rotJson;
	j["scale"] = scaleJson;

}

void from_json(const json& j, Transform& t)
{
	//j.at("worldMatrix").get_to(t.worldMatrix);
	//j["worldMatrix"] = t.position;
	//j.at("worldMatrix").get_to(t.position);
	from_json(j["worldMatrix"], t.worldMatrix);
	from_json(j["position"], t.position);
	from_json(j["rotation"], t.rotation);
	from_json(j["scale"], t.scale);
}

void to_json(json& j, const Renderer& r)
{
	j["vertexShaderPath"] = r.vertexShaderPath;
	j["fragmentShaderPath"] = r.fragmentShaderPath;
}

void from_json(const json& j, Renderer& r)
{
	r = Renderer(j["vertexShaderPath"], j["fragmentShaderPath"]);
	r.vertexShaderPath = j["vertexShaderPath"];
	r.fragmentShaderPath = j["fragmentShaderPath"];
}

void to_json(json& j, const Mesh& m)
{
	j["path"] = m.path;
}

void from_json(const json& j, Mesh& m)
{
	std::string path = j["path"];
	m = Mesh(j["path"].get<std::string>().c_str());
}

void to_json(json& j, const Camera& c)
{
	to_json(j["view"], c.view);
	to_json(j["projection"], c.projection);
	j["fieldOfView"] = c.fieldOfView;
	j["nearPlaneDistance"] = c.nearPlaneDistance;
	j["farPlaneDistance"] = c.farPlaneDistance;
	j["movementSpeed"] = c.movementSpeed;
}

void from_json(const json& j, Camera& c)
{
	from_json(j["view"], c.view);
	from_json(j["projection"], c.projection);
	c.fieldOfView = j["fieldOfView"];
	c.nearPlaneDistance = j["nearPlaneDistance"];
	c.farPlaneDistance = j["farPlaneDistance"];
	c.movementSpeed = j["movementSpeed"];
}

void to_json(json& j, const RandomColor& c)
{
	j = {};
}

void from_json(const json& j, RandomColor& c)
{
	
}

template<class T>
void TrySerializeComponent()
{
	std::string typeName = std::string(typeid(T).name()).append(".json");
	std::ofstream jsonFile = std::ofstream(typeName);
	nlohmann::json json;
	registry.each([&json](entt::entity e) {
		if (registry.has<T>(e))
		{
			T& obj = registry.get<T>(e);
			std::string stringified = std::to_string((int)e);
			json[stringified] = obj;
		}
	});
	jsonFile << json << std::endl;
}
void Serialize()
{
	json entitiesJson;
	std::vector<std::string> stringifiedEntities;
	registry.each([&stringifiedEntities](entt::entity e)
	{
		stringifiedEntities.push_back(std::to_string((int)e));
	});
	entitiesJson = stringifiedEntities;
	std::ofstream entitiesFile("entities.json");
	entitiesFile << entitiesJson << std::endl;
	TrySerializeComponent<Transform>();
	TrySerializeComponent<Mesh>();
	TrySerializeComponent<Camera>();
	TrySerializeComponent<Renderer>();
	TrySerializeComponent<RandomColor>();
}
template<class T>
void TryDeserializeComponent(std::map<std::string, entt::entity> entityMap, std::vector<std::string> storedEntities)
{
	std::string g = std::string(typeid(T).name()).append(".json");
	std::ifstream inFile(g);
	json objJson;
	inFile >> objJson;
	//T obj = objJson.get<T>();
	for (auto it = storedEntities.begin(); it != storedEntities.end(); ++it)
	{
		//auto compIterator = objJson.find(*it);
		if (objJson.find(*it) != objJson.end())
		{
			auto compIter = objJson[*it];
			//auto meshMaybe = compIter.get<Mesh>();
			T obj;
			from_json(compIter, obj);
			//T comp;
			//from_json(compIter->, comp);
			//T comp = objJson[*it];
			//T comp = objJson[*it].get<T>();
			//objJson[*it];
			//T comp;
			//from_json(objJson[*it], comp);

			registry.emplace<T>(entityMap[*it], std::move(obj));
		}
	}
}
void Deserialize()
{
	GizmoSystem::DeselectAll();
	registry.clear();
	std::vector<std::string> storedEntities;
	std::ifstream entitiesFile("entities.json");
	json entitiesJson;
	entitiesFile >> entitiesJson;
	storedEntities = entitiesJson.get<std::vector<std::string>>();
	std::map<std::string, entt::entity> entityMap;
	for (auto it = storedEntities.begin(); it != storedEntities.end(); ++it)
	{
		entt::entity e = registry.create();
		entityMap.emplace(*it, e);
	}
	TryDeserializeComponent<Mesh>(entityMap, storedEntities);
	TryDeserializeComponent<Camera>(entityMap, storedEntities);
	TryDeserializeComponent<Renderer>(entityMap, storedEntities);
	TryDeserializeComponent<Transform>(entityMap, storedEntities);
	TryDeserializeComponent<RandomColor>(entityMap, storedEntities);
	auto compView = registry.view<Mesh>();
	for (auto v : compView)
	{
		Mesh& m = registry.get<Mesh>(v);
		MeshLoaderSystem::LoadMesh(m.path.c_str(), m);
	}
	auto rendererView = registry.view<Renderer>();
	auto cameraView = registry.view<Camera>();
	Camera& camera = registry.get<Camera>(cameraView[0]);
	for (auto rv : rendererView)
	{
		Renderer& renderer = registry.get<Renderer>(rv);
		Load(renderer, camera);
	}
	auto meshView = registry.view<Mesh, Transform>();
	Transform& meshTransform = registry.get<Transform>(*meshView.begin());
	GizmoSystem::Select(*meshView.begin());
}

void MakeMesh(const char* path, const char* name = "GameObject") {
	auto camView = registry.view<Camera>();
	auto [camera, camTransform] = registry.get<Camera, Transform>(camView[0]);
	glm::vec3 newMeshPos = camTransform.position + TransformSystem::CalculateForward(&camTransform);
	auto newMeshEntity = registry.create();
	Mesh& newMesh = registry.emplace<Mesh>(newMeshEntity, path);
	MeshLoaderSystem::LoadMesh(newMesh.path.c_str(), newMesh);
	Transform& meshTransform = registry.emplace<Transform>(newMeshEntity);
	meshTransform.position = newMeshPos;
	TransformSystem::CalculateWorldMatrix(&meshTransform);
	Renderer& newMeshRenderer = registry.emplace<Renderer>(newMeshEntity, plat->GetAssetPath("../../Shaders/GLSL/vertex.glsl"), plat->GetAssetPath("../../Shaders/GLSL/fragment.glsl"));
	Load(newMeshRenderer, camera);
	registry.emplace<RandomColor>(newMeshEntity);
	Name& nameComp = registry.emplace<Name>(newMeshEntity);
	nameComp = { name };
}

void MakeMesh(const char* path, glm::vec3 pos, const char* name = "GameObject") {
	auto camView = registry.view<Camera>();
	auto [camera, camTransform] = registry.get<Camera, Transform>(camView[0]);
	//glm::vec3 newMeshPos = camTransform.position + TransformSystem::CalculateForward(&camTransform);
	auto newMeshEntity = registry.create();
	Mesh& newMesh = registry.emplace<Mesh>(newMeshEntity, path);
	MeshLoaderSystem::LoadMesh(newMesh.path.c_str(), newMesh);
	Transform& meshTransform = registry.emplace<Transform>(newMeshEntity);
	meshTransform.position = pos;
	TransformSystem::CalculateWorldMatrix(&meshTransform);
	Renderer& newMeshRenderer = registry.emplace<Renderer>(newMeshEntity, plat->GetAssetPath("../../Shaders/GLSL/vertex.glsl"), plat->GetAssetPath("../../Shaders/GLSL/fragment.glsl"));
	Load(newMeshRenderer, camera);
	registry.emplace<RandomColor>(newMeshEntity);
	Name& nameComp = registry.emplace<Name>(newMeshEntity);
	nameComp = { name };
}

void MakeRayFromCamera()
{
	auto camView = registry.view<Camera>();
	auto [camera, camTransform] = registry.get<Camera, Transform>(camView[0]);
	RECT clientRect;
	GetClientRect(GetDesktopWindow(), &clientRect);
	glm::vec2 clientDimensions{ clientRect.right - clientRect.left, clientRect.bottom - clientRect.top };
	ImGuiIO& io = ImGui::GetIO();

	ImVec2 mousePos = io.MousePos;
	//glm::vec2 mousePos = plat->GetInputSystem()->GetCursorPosition();

	// mostly stolen from ImGuizmo.cpp
	//float ndcX = ((plat->GetInputSystem()->GetCursorPosition().x - window->x) / window->width) * 2.f - 1.f;
	//float ndcY = (1.f - ((plat->GetInputSystem()->GetCursorPosition().y - window->y) / window->height)) * 2.f - 1.f;
	
	float ndcX = ((mousePos.x - window->x) / window->width) * 2.f - 1.f;
	float ndcY = (1.f - ((mousePos.y - window->y) / window->height)) * 2.f - 1.f;


	glm::mat4 viewProjInverse = camera.projection * camera.view;
	viewProjInverse = glm::inverse(viewProjInverse);
	
	glm::vec4 rayOrigin = glm::vec4(ndcX, ndcY, 0, 1) * viewProjInverse;
	rayOrigin /= rayOrigin.w;
	glm::vec4 rayEnd = glm::vec4(ndcX, ndcY, 1.0f - FLT_EPSILON, 1) * viewProjInverse;
	rayEnd /= rayEnd.w;
	glm::vec3 ray = glm::normalize(rayOrigin - rayEnd);
	ray *= 3;

	//float depth;
	//glReadPixels(mousePos.x, mousePos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	//glm::vec3 toUnproject(mousePos.x, clientDimensions.y - 1 - mousePos.y, depth);
	////glm::vec4 ohno = glm::vec4(toUnproject.x, toUnproject.y, toUnproject.z, 1)* glm::inverse(camera.projection)* camTransform.worldMatrix;
	//glm::vec3 whoopee = glm::unProject(toUnproject, camera.view, camera.projection, glm::vec4(0, 0, 800, 600));
	////glm::vec3 whoopee = { ohno.x, ohno.y, ohno.z };
	//whoopee = (whoopee - camTransform.position);
	//whoopee = glm::normalize(whoopee);
	//whoopee *= 5;
	//
	MakeMesh(plat->GetAssetPath("../../Assets/Models/cube.obj").c_str(), ray);
	
	
	//whoopee *= 10;
	//Vertex lineEnd = { whoopee, {0, 0, 0}, {0, 0}, {0, 0, 0}, {0, 0, 1, 1} };
	//lineMesh.rawVertices = { lineStart, lineEnd };
	//Renderer& lineRenderer = registry.emplace<Renderer>(lineEntity, plat->GetAssetPath("../../Shaders/GLSL/vertex.glsl"), plat->GetAssetPath("../../Shaders/GLSL/fragment.glsl"));
	//Transform& lineTransform = registry.emplace<Transform>(lineEntity);
	//Load(lineRenderer, camera);
	//LoadMesh(lineRenderer, lineMesh);
}


void RandomizeVertexColors()
{
	auto rcView = registry.view<RandomColor, Mesh, Renderer, Transform>();
	for (auto rc : rcView)
	{
		Mesh& m = registry.get<Mesh>(rc);
		for (Vertex& v : m.rawVertices)
		{
			v.Color = { (rand() % 255) / 255.0f, (rand() % 255) / 255.0f, (rand() % 255) / 255.0f, 1 };
		}
		Renderer& renderer = registry.get<Renderer>(rc);
		LoadMesh(renderer, m);
		Transform& t = registry.get<Transform>(rc);
		auto cameraView = registry.view<Camera>();
		Camera& cam = registry.get<Camera>(*cameraView.begin());
		UpdateRenderer(renderer, t, cam);
	}
}

void Loop()
{
	ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplWin32_NewFrame();
	plat->NewGuiFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	ImGuizmo::Enable(true);
	static float vertColorPick[4];
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::BeginMenu("New"))
		{
			if (ImGui::MenuItem("Cube"))
			{
				MakeMesh(plat->GetAssetPath("../../Assets/Models/cube.obj").c_str(), "Cube");
			}
			if (ImGui::MenuItem("Helix"))
			{
				MakeMesh(plat->GetAssetPath("../../Assets/Models/helix.obj").c_str(), "Helix");
			}
			if (ImGui::MenuItem("Cone"))
			{
				MakeMesh(plat->GetAssetPath("../../Assets/Models/cone.obj").c_str(), "Cone");
			}
			if (ImGui::MenuItem("Cylinder"))
			{
				MakeMesh(plat->GetAssetPath("../../Assets/Models/cylinder.obj").c_str(), "Cylinder");
			}
			if (ImGui::MenuItem("Sphere"))
			{
				MakeMesh(plat->GetAssetPath("../../Assets/Models/sphere.obj").c_str(), "Sphere");
			}
			if (ImGui::MenuItem("Torus"))
			{
				MakeMesh(plat->GetAssetPath("../../Assets/Models/torus.obj").c_str(), "Torus");
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save"))
		{
			Serialize();
		}
		if (ImGui::MenuItem("Open"))
		{
			Deserialize();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Components"))
	{
		if (ImGui::BeginMenu("Color picker"))
		{
			ImGui::ColorPicker4("Vertex color: ", vertColorPick);
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Random Vertex Colors"))
		{
			RandomizeVertexColors();
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	//ImGui::Begin("Vertex color picker", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);
	//ImGui::Text("Vertex color: ");
	//ImGui::ColorPicker4("Vertex color: ", vertColorPick);
	//ImGui::End();

	plat->GetInputSystem()->GetKeyPressed();
	auto view = registry.view<Mesh, Renderer, Transform, Name>();
	ImGui::Begin("Entity List");
	//ImGui::SetWindowPos("Entity List", { 600, 20 });
	//ImGui::SetWindowSize({ 200, 780 });
	for (auto renderable : view)
	{
		Name name = view.get<Name>(renderable);
		if (ImGui::MenuItem(name.nameString.c_str()))
		{
			GizmoSystem::DeselectAll();
			Transform& newSelected = view.get<Transform>(renderable);
			GizmoSystem::Select(renderable);
		}
	}
	ImGui::End();
	graph->ClearScreen();
	auto camEntityView = registry.view<Camera>();
	auto [camera, camTransform] = registry.get<Camera, Transform>(camEntityView[0]);
	TransformSystem::CalculateWorldMatrix(&camTransform);
	CameraSystem::CalculateViewMatrix(camera, camTransform);
	for (auto renderable : view)
	{
		Renderer& renderer = registry.get<Renderer>(renderable);
		Mesh& mesh = registry.get<Mesh>(renderable);
		Transform& meshTransform = registry.get<Transform>(renderable);
		// if RandomColor component is attached, do not assign every vertex a color
		if (!registry.has<RandomColor>(renderable))
		{
			for (auto it = mesh.rawVertices.begin(); it != mesh.rawVertices.end(); ++it)
			{
				it->Color = { vertColorPick[0], vertColorPick[1], vertColorPick[2], vertColorPick[3] };
			}
		}
		LoadMesh(renderer, mesh);
		UpdateRenderer(renderer, meshTransform, camera);
		//renderer.Update();
		//renderer.Draw();
		Draw(renderer);
	}
	auto transformView = registry.view<Transform>();
	//camera.transform = transform;
	GizmoSystem::DrawGizmo(camera, transformView);
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
	window = new GameWindow(0, 0, 800, 600);
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
	MeshLoaderSystem::LoadMesh(mesh.path.c_str(), mesh);
	Renderer& renderer = registry.emplace<Renderer>(entity, plat->GetAssetPath("../../Shaders/GLSL/vertex.glsl"), plat->GetAssetPath("../../Shaders/GLSL/fragment.glsl"));
	Transform& t1 = registry.emplace<Transform>(entity);
	Name& name = registry.emplace<Name>(entity);
	name = { "Cone" };
	Load(renderer, cam);
	//renderer.LoadMesh(mesh.GetRawVertices());
	LoadMesh(renderer, mesh);

	GizmoSystem::Select(entity);


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
	plat->GetInputSystem()->RegisterKeyPressFunction('e', []() {GizmoSystem::op = ImGuizmo::ROTATE; });
	plat->GetInputSystem()->RegisterKeyPressFunction('w', []() {GizmoSystem::op = ImGuizmo::TRANSLATE; });

	//plat->GetInputSystem()->RegisterKeyPressFunction('b', []() {Serialize(); });
	//plat->GetInputSystem()->RegisterKeyPressFunction('m', []() {Deserialize(); });

	// TODO: Remove this MakeRayFromCamera call
	plat->GetInputSystem()->RegisterKeyPressFunction('j', []() {MakeRayFromCamera(); });
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
