#pragma once

class SC_API Compiler
{
public:
    static void Compile(const char* hlslFilePath,
        const wchar_t* pdbFilePath,
        const wchar_t* binFilePath,
        const wchar_t* refFilePath);
};
