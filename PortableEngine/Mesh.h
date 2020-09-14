# if 0
#pragma once
#include <wrl/client.h>
#include <vector>
#include "Vertex.h"
class Mesh
{
private:
	//Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	//Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int numIndices;
	std::vector<Vertex> rawVertices;
	std::vector<UINT> rawIndices;
	//void CreateBuffers(Vertex* vertices, int numVertices, unsigned int* indices, int numIndicesParam, Microsoft::WRL::ComPtr<ID3D11Device> device);

public:
	Mesh(void);
	//Mesh(Vertex* vertices, int numVertices, unsigned int* indices, int numIndicesParam, Microsoft::WRL::ComPtr<ID3D11Device> device);
	//Mesh(const char* fileName, Microsoft::WRL::ComPtr<ID3D11Device> device);
	Mesh(const char* fileName);
	//Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	//Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();
	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	std::vector<Vertex> GetRawVertices();
	std::vector<UINT> GetRawIndices();
};
#endif
