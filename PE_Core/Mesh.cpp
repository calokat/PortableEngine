#include "Mesh.h"
#include <fstream>
using namespace std;
Mesh::Mesh(void)
{
	numIndices = 0;
}
Mesh::Mesh(const char* fileName)
{
	this->path = std::string(fileName);
	numIndices = 0;
}

Mesh& Mesh::operator=(Mesh&& other)
{
	if (this != &other)
	{
		this->numIndices = other.numIndices;
		this->rawVertices = other.rawVertices;
		this->rawIndices = other.rawIndices;
		this->path = other.path;
	}
	return *this;
}

Mesh::Mesh(Mesh&& other) noexcept
{
	*this = std::move(other);
}

Mesh::Mesh(Mesh& other)
{
	*this = std::move(other);
}
