#include "PEWideString.h"
#include <string.h>
#include <utility>

PE::WideString::WideString()
{
	data = nullptr;
}

PE::WideString::WideString(const char* mbStr)
{
	size_t wideCount = mbstowcs(nullptr, mbStr, 0) + 1;
	wchar_t* wideData = new wchar_t[wideCount];
	mbstowcs(wideData, mbStr, wideCount);
	this->data = wideData;
}

PE::WideString::WideString(const PE::String& str) : PE::WideString::WideString(str.GetData())
{
}

PE::WideString::WideString(const wchar_t* newData)
{
	size_t newLength = wcslen(newData);
	data = new wchar_t[newLength + 1];
	wcscpy(data, newData);
	data[newLength] = 0;
}

PE::WideString::WideString(WideString&& other)
{
	this->data = nullptr;
	*this = std::move(other);
}

PE::WideString& PE::WideString::operator=(WideString&& other)
{
	if (this != &other)
	{
		delete[] this->data;
		this->data = other.data;
		other.data = nullptr;
	}
	return *this;
}

PE::WideString& PE::WideString::operator=(const WideString& other)
{
	if (this != &other)
	{
		size_t dataSize = wcslen(other.data);
		delete[] this->data;
		this->data = new wchar_t[dataSize + 1];
		wcscpy(this->data, other.data);
		data[dataSize] = 0;
	}
	return *this;
}

const wchar_t* PE::WideString::GetData()
{
	return data;
}

void PE::WideString::SetData(const wchar_t* newData)
{
	if (newData != nullptr)
	{
		delete[] data;
		size_t newDataLength = wcslen(newData);
		data = new wchar_t[newDataLength + 1];
		wcscpy(data, newData);
		data[newDataLength] = 0;
	}
}

PE::WideString PE::WideString::Concat(const WideString& str1, const WideString& str2)
{
	size_t str1Len = wcslen(str1.data);
	size_t str2Len = wcslen(str2.data);
	size_t concatDataLen = str1Len + str2Len + 1;
	wchar_t* concatData = new wchar_t[concatDataLen];
	wcscpy(concatData, str1.data);
	wcscpy(concatData + str1Len, str2.data);
	//concatData[concatDataLen] = 0;
	PE::WideString result;
	result.data = concatData;
	return result;
}

PE::WideString::~WideString()
{
	if (data != nullptr)
	{
		delete[] data;
		data = nullptr;
	}
}

PE::WideString::WideString(const WideString& other)
{
	this->data = nullptr;
	*this = other;
}
