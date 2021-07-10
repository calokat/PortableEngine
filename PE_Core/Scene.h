#pragma once
#include <vector>
#include <functional>
template<class T>
struct Scene
{
	T data;
	std::vector<Scene> children;
	bool isEmpty;
};