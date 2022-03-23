#pragma once
#include "PEString.h"
namespace PE
{
    class WideString
    {
    public:
        WideString();
        WideString(const char* mbStr);
        WideString(const PE::String& str);
        WideString(const wchar_t* newData);
        WideString(WideString&& other);
        PE::WideString& operator=(WideString&& other);
        PE::WideString(const WideString& other);
        PE::WideString& operator=(const WideString& other);
        const wchar_t* GetData();
        void SetData(const wchar_t* newData);
        static PE::WideString Concat(const WideString& str1, const WideString& str2);
        ~WideString();
    private:
        wchar_t* data = nullptr;
    };
}