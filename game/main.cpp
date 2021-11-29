#include <d3d12shader.h>
#include <dxcapi.h>
#include <vector>
#include <wrl/client.h>
using namespace Microsoft::WRL;
#include <exception>
#include <iostream>
#include <string>
#include <fstream>
#include <d3d12.h>
#include "d3dx12.h"

#define ThrowIfFailed(h) if (FAILED(h)) throw std::exception("failed");

int TestDXCReflect()
{
	std::ifstream shaderFile(ASSEMBLY_PATH("shader.dxc.vs.bin"), std::ios::ate | std::ios::binary);
	size_t fileSize = shaderFile.tellg();
	char* shaderContent = new char[fileSize];
	shaderFile.seekg(0, shaderFile.beg);
	shaderFile.read(shaderContent, fileSize);
	shaderFile.close();

	ComPtr<IDxcUtils> utils;
	ThrowIfFailed(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils)));

	DxcBuffer buffer{};
	buffer.Ptr = shaderContent;
	buffer.Size = fileSize;
	buffer.Encoding = 0;

	ComPtr<ID3D12ShaderReflection> D3D12Reflection;
	ThrowIfFailed(utils->CreateReflection(&buffer, IID_PPV_ARGS(&D3D12Reflection)));

	D3D12_SHADER_DESC desc{};
	D3D12Reflection->GetDesc(&desc);

	std::vector<D3D12_SIGNATURE_PARAMETER_DESC> sigDescs(desc.InputParameters);
	for (uint32_t i = 0; i < desc.InputParameters; i++)
	{
		D3D12Reflection->GetInputParameterDesc(i, &sigDescs[i]);
	}

	std::vector<D3D12_SIGNATURE_PARAMETER_DESC> outputDescs(desc.OutputParameters);
	for (uint32_t i = 0; i < desc.OutputParameters; i++)
	{
		D3D12Reflection->GetOutputParameterDesc(i, &outputDescs[i]);
	}

	return 0;
}

#include <spirv_reflect.h>

void TestSpirvReflect()
{
	constexpr char* pvs = ASSEMBLY_PATH("shader.spirv.ps.bin");

	std::ifstream shaderFile(pvs, std::ios::ate | std::ios::binary);
	size_t fileSize = shaderFile.tellg();
	char* shaderContent = new char[fileSize];
	shaderFile.seekg(0, shaderFile.beg);
	shaderFile.read(shaderContent, fileSize);
	shaderFile.close();

	spv_reflect::ShaderModule shaderModule(fileSize, shaderContent);
	

}

int main()
{
	TestDXCReflect();
	TestSpirvReflect();
	return 0;
}