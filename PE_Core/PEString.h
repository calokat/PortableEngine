#pragma once

namespace PE
{
    class String
    {
    public:
        String();
        String(const char* newData);
        String(String&& other) noexcept;
        String& operator=(String&& other) noexcept;
        String(const String& other);
        String& operator=(const String& other) noexcept;
        const char* GetData();
        void SetData(const char* newData);
        static PE::String Concat(const PE::String& str1, const PE::String& str2);
        ~String();
    private:
        char* data = nullptr;

    };
}