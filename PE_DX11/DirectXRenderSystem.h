#ifdef _WIN64
#pragma once
#include "DirectXRenderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "DirectXAPI.h"
#include "IRenderSystem.h"
#include "PEImage.h"
#include <map>

class DirectXRenderSystem : public IRenderSystem
{
public:
	IRenderer& CreateRenderer(entt::registry& reg, entt::entity& e, ShaderType type);
	void Load(IRenderer* renderer, Camera& camera);
	void BindRenderer(IRenderer* renderer);
	void LoadMesh(IRenderer* renderer, Mesh& mesh);
	void Draw(IRenderer* DirectXRenderer);
	void DrawWireframe(IRenderer* DirectXRenderer);
	void UpdateRenderer(IRenderer* renderer, Transform meshTransform, Camera camera);
	DirectXRenderSystem(ID3D11Device* dev, ID3D11DeviceContext* ctx);
	void CreateTexture(PEImage& img);
	void LoadTexture(IRenderer* renderer, std::string imagePath);
	void LoadTexture(PEImage& img);
private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	std::map<ShaderType, LPCWSTR> typeToVertexPath = { {ShaderType::Unlit_Color, L"../x64/Debug/VertexShader_UnlitColor.cso"}, {ShaderType::Unlit_Textured, L"../x64/Debug/VertexShader.cso"} };
	std::map<ShaderType, LPCWSTR> typeToPixelPath = { {ShaderType::Unlit_Color, L"../x64/Debug/PixelShader_UnlitColor.cso" }, { ShaderType::Unlit_Textured, L"../x64/Debug/PixelShader.cso" } };
	void BindTexture(DirectXRenderer* renderer);
};
//void DrawGizmo(Camera camera);
#endif