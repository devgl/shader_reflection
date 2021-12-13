/*

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
#include <spdlog/spdlog.h>

#define ThrowIfFailed(h) if (FAILED(h)) throw std::exception("failed");


int TestDXCReflect()
{
	std::ifstream shaderFile(ASSEMBLY_PATH("shader.dxc.ps.bin"), std::ios::ate | std::ios::binary);
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

	for (uint32_t i = 0; i < desc.ConstantBuffers; i++)
	{
		auto cb = D3D12Reflection->GetConstantBufferByIndex(i);

		D3D12_SHADER_BUFFER_DESC desc{};
		cb->GetDesc(&desc);
		auto v0 = cb->GetVariableByIndex(0);
	}

	for (uint32_t i = 0; i < desc.BoundResources; i++)
	{
		D3D12_SHADER_INPUT_BIND_DESC d{};
		D3D12Reflection->GetResourceBindingDesc(i, &d);
		if (d.Type == D3D_SIT_TEXTURE)
		{
			SPDLOG_INFO("Bind, {0}, {1}, {2}, {3}", d.BindCount, d.BindPoint, d.Space, d.uID);
		}
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
	
	uint32_t count = 0;
	shaderModule.EnumerateDescriptorSets(&count, nullptr);
	std::vector<SpvReflectDescriptorSet*> descSets(count);
	shaderModule.EnumerateDescriptorSets(&count, descSets.data());
	for (auto s : descSets)
	{
		SPDLOG_INFO("set : {0}, binding : {1}", s->set, s->binding_count);
	}
}
*/

#include <Compiler.hpp>
#include <string>

int main()
{
	std::wstring pdb(Widen(ASSEMBLY_PATH("out/shader.pdb")));
	std::wstring bin(Widen(ASSEMBLY_PATH("out/shader.bin")));
	std::wstring ref(Widen(ASSEMBLY_PATH("out/shader.ref")));

	Compiler::Compile(ShaderFilePath, pdb.c_str(), bin.c_str(), ref.c_str());
	return 0;
}