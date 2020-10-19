#pragma once
#include <vector>
#include "Vertex.h"
class Mesh
{
private:
	//Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	//Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int numIndices;
	std::vector<Vertex> rawVertices;
	std::vector<unsigned int> rawIndices;
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
	std::vector<Vertex>& GetRawVertices();
	std::vector<unsigned int> GetRawIndices();
	// Move assignment operator
	Mesh& operator=(Mesh&& other);
	// Move constructor
	Mesh(Mesh&& other) noexcept;
	Mesh(Mesh& other);
};
