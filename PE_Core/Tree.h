#pragma once
#include <vector>
#include <functional>
template<class T>
struct Tree
{
	T data;
	std::vector<Tree> children;
	bool isEmpty;
};