#include "shader_reflection/shader_reflection.hpp"
#include "directx/d3d12.h"
#include "directx/d3dx12.h"
#include "shader_assets/shaders.hpp"

int main()
{
    constexpr const auto* pszHlsl = SHADER_FILE_W("123.hlsl");

    shader_reflection::options options;
    options.file_path = pszHlsl;

    constexpr const char* psz = SHADER_ASSET_PATH;

    auto result = shader_reflection::reflect(&options);

    return 0;
}