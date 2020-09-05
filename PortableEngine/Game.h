#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Material.h"
#include <vector>
#include "SimpleShader.h"
#include "Lights.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void LoadTextures();
	void CreateBasicGeometry();

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Mode, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	// Shaders and shader-related constructs
	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* normalPixelShader;
	SimpleVertexShader* normalVertexShader;
	//Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	//Mesh* triangle;
	//Mesh* square;
	//Mesh* trapezoid;
	//Mesh* cube;

	Mesh* sphere;
	Mesh* helix;
	std::vector<Entity*> entities;
	Camera* camera;

	Material* red;
	Material* green;
	Material* blue;
	Material* white;
	PointLight light;
	PointLight light2;
	PointLight light3;
	DirectionalLight light4;
	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* srv2;

	ID3D11ShaderResourceView* normalSrv;
	ID3D11ShaderResourceView* normalSrv2;

	ID3D11SamplerState* sampler;
};

