#include "Compiler.hpp"

#include <wrl.h>
using namespace Microsoft::WRL;
#include <dxcapi.h>

#include <cassert>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

#include <stdio.h>

bool ReadHLSL(const char* hlslFilePath, const void** ppData, size_t* pSize)
{
    try
    {
        std::ifstream hlslInput(hlslFilePath, std::ios::ate);
        if (!hlslInput.is_open())
        {
            return false;
        }

        auto fileSize = hlslInput.tellg();
        char* pData = new char[fileSize];
        hlslInput.seekg(0, std::ios::beg);
        hlslInput.read(pData, fileSize);
        hlslInput.close();

        *ppData = pData;
        *pSize = fileSize;

        return true;
    }
    catch (std::runtime_error& e)
    {
        return false;
    }
}

void Compiler::Compile(
    const char* hlslFilePath,
    const wchar_t* pdbFilePath,
    const wchar_t* binFilePath,
    const wchar_t* refFilePath)
{
    DxcBuffer buffer = {};

    const char* pData = nullptr;
    size_t size = 0;
    if (!ReadHLSL(hlslFilePath, &buffer.Ptr, &buffer.Size))
    {
        return;
    }

    ComPtr<IDxcUtils> utils;
    auto hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(utils.GetAddressOf()));
    assert(SUCCEEDED(hr));

    ComPtr<IDxcIncludeHandler> includeHeader;
    utils->CreateDefaultIncludeHandler(includeHeader.GetAddressOf());

    ComPtr<IDxcCompiler3> compiler3;
    hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(compiler3.GetAddressOf()));
    assert(SUCCEEDED(hr));

    std::vector<LPCWSTR> args;
    args.push_back(L"-E");
    args.push_back(L"VSMain");

    args.push_back(L"-T");
    args.push_back(L"vs_6_0");

    args.push_back(L"-Zi");
    args.push_back(L"-Fd");
    args.push_back(pdbFilePath);

    args.push_back(L"-Qstrip_reflect");
    args.push_back(L"-Fre");
    args.push_back(refFilePath);

    args.push_back(L"-Fo");
    args.push_back(binFilePath);

    args.push_back(L"-D");
    args.push_back(L"MY_DEFINE=1");

    ComPtr<IDxcResult> result;
    hr = compiler3->Compile(&buffer, args.data(), args.size(), includeHeader.Get(), IID_PPV_ARGS(result.GetAddressOf()));
    assert(SUCCEEDED(hr));

    ComPtr<IDxcBlobUtf16> hash;
    hr = result->GetOutput(DXC_OUT_SHADER_HASH, IID_PPV_ARGS(hash.GetAddressOf()), nullptr);
    assert(SUCCEEDED(hr));


}

