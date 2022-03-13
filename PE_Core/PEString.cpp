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
	this->data = nullptr;
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
	this->data = nullptr;
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

PE::String PE::String::Concat(const PE::String& str1, const PE::String& str2)
{
	size_t str1Len = strlen(str1.data);
	size_t str2Len = strlen(str2.data);
	size_t concatDataLen = str1Len + str2Len + 1;
	char* concatData = new char[concatDataLen];
	strcpy(concatData, str1.data);
	strcpy(concatData + str1Len, str2.data);
	concatData[concatDataLen] = 0;
	PE::String result("");
	result.data = concatData;
	return result;
}

PE::String::~String()
{
	if (data != nullptr)
	{
		delete[] data;
		data = nullptr;
	}
}