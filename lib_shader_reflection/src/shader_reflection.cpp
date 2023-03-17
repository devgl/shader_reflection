#include "shader_reflection/shader_reflection.hpp"
#include <fstream>
#include <vector>
#include <wrl/client.h>
#include <dxcapi.h>
#include <d3d12shader.h>
#include <directx/d3d12.h>
#include <directx/d3dx12.h>

using namespace Microsoft::WRL;

namespace shader_reflection
{
    static constexpr DXGI_FORMAT format_table[4][16] = {
                {
                    DXGI_FORMAT_UNKNOWN,            // 0
                    DXGI_FORMAT_UNKNOWN,           // 1
                    DXGI_FORMAT_UNKNOWN,            // 2
                    DXGI_FORMAT_UNKNOWN,        // 3
                    DXGI_FORMAT_UNKNOWN,            // 4,
                    DXGI_FORMAT_UNKNOWN,            // 5,
                    DXGI_FORMAT_UNKNOWN,            // 6,
                    DXGI_FORMAT_UNKNOWN,     // 7,
                    DXGI_FORMAT_UNKNOWN,            // 8,
                    DXGI_FORMAT_UNKNOWN,            // 9,
                    DXGI_FORMAT_UNKNOWN,            // 10,
                    DXGI_FORMAT_UNKNOWN,            // 11,
                    DXGI_FORMAT_UNKNOWN,            // 12,
                    DXGI_FORMAT_UNKNOWN,            // 13,
                    DXGI_FORMAT_UNKNOWN,            // 14,
                    DXGI_FORMAT_UNKNOWN   // 15,
                },
                {
                    DXGI_FORMAT_UNKNOWN,            // 0
                    DXGI_FORMAT_R32_UINT,           // 1
                    DXGI_FORMAT_UNKNOWN,            // 2
                    DXGI_FORMAT_R32G32_UINT,        // 3
                    DXGI_FORMAT_UNKNOWN,            // 4,
                    DXGI_FORMAT_UNKNOWN,            // 5,
                    DXGI_FORMAT_UNKNOWN,            // 6,
                    DXGI_FORMAT_R32G32B32_UINT,     // 7,
                    DXGI_FORMAT_UNKNOWN,            // 8,
                    DXGI_FORMAT_UNKNOWN,            // 9,
                    DXGI_FORMAT_UNKNOWN,            // 10,
                    DXGI_FORMAT_UNKNOWN,            // 11,
                    DXGI_FORMAT_UNKNOWN,            // 12,
                    DXGI_FORMAT_UNKNOWN,            // 13,
                    DXGI_FORMAT_UNKNOWN,            // 14,
                    DXGI_FORMAT_R32G32B32A32_UINT   // 15,
                },
                {
                    DXGI_FORMAT_UNKNOWN,            // 0
                    DXGI_FORMAT_R32_SINT,           // 1
                    DXGI_FORMAT_UNKNOWN,            // 2
                    DXGI_FORMAT_R32G32_SINT,        // 3
                    DXGI_FORMAT_UNKNOWN,            // 4,
                    DXGI_FORMAT_UNKNOWN,            // 5,
                    DXGI_FORMAT_UNKNOWN,            // 6,
                    DXGI_FORMAT_R32G32B32_SINT,     // 7,
                    DXGI_FORMAT_UNKNOWN,            // 8,
                    DXGI_FORMAT_UNKNOWN,            // 9,
                    DXGI_FORMAT_UNKNOWN,            // 10,
                    DXGI_FORMAT_UNKNOWN,            // 11,
                    DXGI_FORMAT_UNKNOWN,            // 12,
                    DXGI_FORMAT_UNKNOWN,            // 13,
                    DXGI_FORMAT_UNKNOWN,            // 14,
                    DXGI_FORMAT_R32G32B32A32_SINT   // 15,
                },
                {
                    DXGI_FORMAT_UNKNOWN,            // 0
                    DXGI_FORMAT_R32_FLOAT,          // 1
                    DXGI_FORMAT_UNKNOWN,            // 2
                    DXGI_FORMAT_R32G32_FLOAT,       // 3
                    DXGI_FORMAT_UNKNOWN,            // 4,
                    DXGI_FORMAT_UNKNOWN,            // 5,
                    DXGI_FORMAT_UNKNOWN,            // 6,
                    DXGI_FORMAT_R32G32B32_FLOAT,    // 7,
                    DXGI_FORMAT_UNKNOWN,            // 8,
                    DXGI_FORMAT_UNKNOWN,            // 9,
                    DXGI_FORMAT_UNKNOWN,            // 10,
                    DXGI_FORMAT_UNKNOWN,            // 11,
                    DXGI_FORMAT_UNKNOWN,            // 12,
                    DXGI_FORMAT_UNKNOWN,            // 13,
                    DXGI_FORMAT_UNKNOWN,            // 14,
                    DXGI_FORMAT_R32G32B32A32_FLOAT  // 15,
                },
    };

    std::vector<char> LoadFromFile(LPCWSTR file_path)
    {
        std::ifstream input(file_path, std::ios::in | std::ios::ate);
        const auto fileSize = input.tellg();
        std::vector<char> data;
        data.resize(fileSize);
        input.seekg(0);
        input.read(data.data(), fileSize);
        input.close();
        return data;
    }

    void WriteToFile(LPCWSTR file_path, const void* pdata, const size_t size)
    {
        std::ofstream output(file_path, std::ios::out | std::ios::binary);
        if (!output.is_open())
        {
            std::printf("error open file %ls", file_path);
        }
        output.write(static_cast<const char*>(pdata), size);
        output.flush();
        output.close();
    }

    ComPtr<IDxcResult> CompileVS(const compile_option* ops)
    {
        // create compiler
        ComPtr<IDxcUtils> pUtils;
        DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));

        ComPtr<IDxcIncludeHandler> pIncludeHandler;
        pUtils->CreateDefaultIncludeHandler(&pIncludeHandler);

        ComPtr<IDxcCompiler3> pCompiler;
        DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler));

        // load file content
        auto shader_content = LoadFromFile(ops->file_path.data());
        DxcBuffer shader_buffer{};
        shader_buffer.Ptr = shader_content.data();
        shader_buffer.Size = shader_content.size();

        std::vector<LPCWSTR> arguments;
        arguments.push_back(L"-E");
        arguments.push_back(L"VSMain");
        arguments.push_back(L"-T");
        arguments.push_back(L"vs_6_6");
        arguments.push_back(L"-Zi");

        ComPtr<IDxcResult> pResult{};
        pCompiler->Compile(&shader_buffer, arguments.data(), arguments.size(), pIncludeHandler.Get(), IID_PPV_ARGS(&pResult));
        return pResult;
    }

    result ReflectVS(const compile_option* ops)
    {
        auto pResult = CompileVS(ops);

        ComPtr<IDxcBlobWide> shader_error;
        auto hr = pResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(shader_error.GetAddressOf()), nullptr);
        if (shader_error && shader_error->GetStringPointer() != nullptr && shader_error->GetStringLength() > 0)
        {
            return SR_FAILED;
        }

        ComPtr<IDxcBlob> shader_reflection_bin;
        hr = pResult->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(shader_reflection_bin.GetAddressOf()), nullptr);

        // create compiler
        ComPtr<IDxcUtils> pUtils;
        DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));

        DxcBuffer shader_reflection_buffer{};
        shader_reflection_buffer.Ptr = shader_reflection_bin->GetBufferPointer();
        shader_reflection_buffer.Size = shader_reflection_bin->GetBufferSize();
        shader_reflection_buffer.Encoding = 0;

        ComPtr<ID3D12ShaderReflection> shader_reflection;
        hr = pUtils->CreateReflection(&shader_reflection_buffer, IID_PPV_ARGS(shader_reflection.GetAddressOf()));

        D3D12_SHADER_DESC shader_desc;
        shader_reflection->GetDesc(&shader_desc);

        // here, we parse the input layout that can be used in the process of creation of the rendering pipeline objects.
        std::vector<D3D12_INPUT_ELEMENT_DESC> input_layouts{};
        input_layouts.resize(shader_desc.InputParameters);
        for(UINT i = 0; i < shader_desc.InputParameters; i++)
        {
            D3D12_SIGNATURE_PARAMETER_DESC input_element_desc{};
            shader_reflection->GetInputParameterDesc(i, &input_element_desc);

            input_layouts[i].SemanticName = input_element_desc.SemanticName;
            input_layouts[i].SemanticIndex = input_element_desc.SemanticIndex;
            input_layouts[i].Format = format_table[input_element_desc.ComponentType][input_element_desc.Mask];
            input_layouts[i].InputSlot = 0;
            input_layouts[i].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
            input_layouts[i].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            input_layouts[i].InstanceDataStepRate = 0;
        }

        return SR_OK;
    }

    ComPtr<IDxcResult> CompilePS(const compile_option* ops)
    {
        // create compiler
        ComPtr<IDxcUtils> pUtils;
        DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));

        ComPtr<IDxcIncludeHandler> pIncludeHandler;
        pUtils->CreateDefaultIncludeHandler(&pIncludeHandler);

        ComPtr<IDxcCompiler3> pCompiler;
        DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler));

        // load file content
        auto shader_content = LoadFromFile(ops->file_path.data());
        DxcBuffer shader_buffer{};
        shader_buffer.Ptr = shader_content.data();
        shader_buffer.Size = shader_content.size();

        std::vector<LPCWSTR> arguments;
        arguments.push_back(L"-E");
        arguments.push_back(L"PSMain");
        arguments.push_back(L"-T");
        arguments.push_back(L"ps_6_6");
        arguments.push_back(L"-Zi");

        ComPtr<IDxcResult> pResult{};
        pCompiler->Compile(&shader_buffer, arguments.data(), arguments.size(), pIncludeHandler.Get(), IID_PPV_ARGS(&pResult));
        return pResult;
    }

    result ReflectPS(const compile_option* ops)
    {
        auto pResult = CompilePS(ops);

        ComPtr<IDxcBlobWide> shader_error;
        auto hr = pResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(shader_error.GetAddressOf()), nullptr);
        if (shader_error && shader_error->GetStringPointer() != nullptr && shader_error->GetStringLength() > 0)
        {
            return SR_FAILED;
        }

        ComPtr<IDxcBlob> shader_reflection_bin;
        hr = pResult->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(shader_reflection_bin.GetAddressOf()), nullptr);

        // create compiler
        ComPtr<IDxcUtils> pUtils;
        DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));

        DxcBuffer shader_reflection_buffer{};
        shader_reflection_buffer.Ptr = shader_reflection_bin->GetBufferPointer();
        shader_reflection_buffer.Size = shader_reflection_bin->GetBufferSize();
        shader_reflection_buffer.Encoding = 0;

        ComPtr<ID3D12ShaderReflection> shader_reflection;
        hr = pUtils->CreateReflection(&shader_reflection_buffer, IID_PPV_ARGS(shader_reflection.GetAddressOf()));

        D3D12_SHADER_DESC shader_desc;
        shader_reflection->GetDesc(&shader_desc);

        // here, let's figure out per material properties
        std::vector<std::string> texture_names;
        std::vector<std::string> sampler_names;
        const auto material_cbuffer = shader_reflection->GetConstantBufferByName("material_uniforms");
        const auto material_structure = material_cbuffer->GetVariableByIndex(0)->GetType();

        D3D12_SHADER_TYPE_DESC material_type_desc{};
        hr = material_structure->GetDesc(&material_type_desc);
        for(UINT i = 0; i < material_type_desc.Members ; i++)
        {
            auto field_type = material_structure->GetMemberTypeByIndex(i);
            D3D12_SHADER_TYPE_DESC field_type_desc{};
            field_type->GetDesc(&field_type_desc);

            auto field_name = material_structure->GetMemberTypeName(i);
            std::string name(field_name);
            if(name.ends_with("_texture_id"))
            {
                texture_names.push_back(name.substr(0, name.rfind("_texture_id")));
            }
            if(name.ends_with("_sampler_id"))
            {
                sampler_names.push_back(name.substr(0, name.rfind("_sampler_id")));
            }

            

        }
        
        return SR_OK;
    }

    result reflect(const compile_option* ops)
    {
        ReflectVS(ops);
        ReflectPS(ops);
        return SR_OK;
    }
}


