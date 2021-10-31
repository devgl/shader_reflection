#include <d3d12shader.h>
#include <dxcapi.h>
#include <vector>
#include <wrl/client.h>
using namespace Microsoft::WRL;
#include <exception>
#include <iostream>
#include <string>
#include <fstream>

#define ThrowIfFailed(h) if (FAILED(h)) throw std::exception("failed");

int main()
{
	std::vector<LPCWSTR> arguments;

	arguments.push_back(L"-E");
	arguments.push_back(L"main");

	arguments.push_back(L"-T");
	arguments.push_back(L"ps_6_0");

	arguments.push_back(L"-Zi");
	arguments.push_back(L"-Fd");
	arguments.push_back(Widen(PDBFilePath));

	arguments.push_back(L"-Qstrip_reflect");
	arguments.push_back(L"-Fre");
	arguments.push_back(L"refPath");

	arguments.push_back(L"-D");
	arguments.push_back(L"MY_DEFINE=1");

	std::ifstream shaderFile(ShaderPath, std::ios::ate|std::ios::binary);
	size_t fileSize = shaderFile.tellg();
	char* shaderContent = new char[fileSize];
	shaderFile.seekg(0, shaderFile.beg);
	shaderFile.read(shaderContent, fileSize);
	shaderFile.close();

	ComPtr<IDxcUtils> utils;
	ThrowIfFailed(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(utils.ReleaseAndGetAddressOf())));
	ComPtr<IDxcBlobEncoding> source;
	utils->CreateBlob(shaderContent, fileSize, CP_UTF8, source.GetAddressOf());

	ComPtr<IDxcIncludeHandler> includeHandler;
	utils->CreateDefaultIncludeHandler(includeHandler.ReleaseAndGetAddressOf());

	ComPtr<IDxcCompiler3> compiler3;
	ThrowIfFailed(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler3)));

	DxcBuffer buffer{};
	buffer.Ptr = source->GetBufferPointer();
	buffer.Size = source->GetBufferSize();
	buffer.Encoding = 0;

	ComPtr<IDxcResult> result;
	ThrowIfFailed(compiler3->Compile(&buffer, arguments.data(), arguments.size(), includeHandler.Get(), IID_PPV_ARGS(&result)));

	ComPtr<IDxcBlobUtf8> errorMessages;
	ThrowIfFailed(result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errorMessages), nullptr));
	if (errorMessages && errorMessages->GetStringLength())
	{
		std::string errmsg = std::string(errorMessages->GetStringPointer(), errorMessages->GetStringLength());
		std::cout << errmsg << "\n";
		return 0;
	}

	ComPtr<IDxcBlob> shaderobj;
	ThrowIfFailed(result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderobj), nullptr));
	std::ofstream shaderbin(ShaderBinary, std::ios::binary);
	shaderbin.write((const char*)shaderobj->GetBufferPointer(), shaderobj->GetBufferSize());
	shaderbin.close();

	ComPtr<IDxcBlob> pdbData;
	ComPtr<IDxcBlobUtf16> pdbPathFromCompiler;
	ThrowIfFailed(result->GetOutput(DXC_OUT_PDB, IID_PPV_ARGS(&pdbData), &pdbPathFromCompiler));

	std::ofstream pdbbin(PDBFilePath, std::ios::binary);
	pdbbin.write((const char*)pdbData->GetBufferPointer(), pdbData->GetBufferSize());
	pdbbin.close();

	ComPtr<IDxcBlob> reflection;
	ThrowIfFailed(result->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(&reflection), nullptr));

	DxcBuffer reflectionData =
	{
		reflection->GetBufferPointer(),
		reflection->GetBufferSize(),
		0U
	};

	ComPtr<ID3D12ShaderReflection> D3D12Reflection;
	ThrowIfFailed(utils->CreateReflection(&reflectionData, IID_PPV_ARGS(&D3D12Reflection)));

	return 0;
}