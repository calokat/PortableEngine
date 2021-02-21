#pragma once
#include <vector>
#include "Vertex.h"
#include <string>
class Mesh
{
public:
	Mesh(void);
	Mesh(const char* fileName);
	int numIndices;
	std::string path;
	std::vector<Vertex> rawVertices;
	std::vector<unsigned int> rawIndices;
	// Move assignment operator
	Mesh& operator=(Mesh&& other);
	// Move constructor
	Mesh(Mesh&& other) noexcept;
	Mesh(Mesh& other);
};
