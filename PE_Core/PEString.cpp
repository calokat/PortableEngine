#include "PEString.h"
#include <stdio.h>
#include <string>
PE::String::String()
{
	data = nullptr;
}

PE::String::String(const char* newData)
{
	size_t newLength = strlen(newData);
	data = new char[newLength + 1];
	strcpy(data, newData);
}

PE::String::String(String&& other) noexcept
{
	*this = std::move(other);
}

PE::String& PE::String::operator=(String&& other) noexcept
{
	if (this != &other)
	{
		delete[] this->data;
		this->data = other.data;
		other.data = nullptr;
	}
	return *this;
}

PE::String::String(const String& other)
{
	*this = other;
}

PE::String& PE::String::operator=(const String& other) noexcept
{
	if (this != &other)
	{
		size_t dataSize = strlen(other.data);
		delete[] this->data;
		this->data = new char[dataSize + 1];
		strcpy(this->data, other.data);
		data[dataSize] = 0;
	}
	return *this;
}

const char* PE::String::GetData()
{
	return data;
}

void PE::String::SetData(const char* newData)
{
	if (newData != nullptr)
	{
		delete[] data;
		size_t newDataLength = strlen(newData);
		data = new char[newDataLength + 1];
		strcpy(data, newData);
		data[newDataLength] = 0;
	}
}

PE::String::~String()
{
	if (data != nullptr)
	{
		delete[] data;
		data = nullptr;
	}
}