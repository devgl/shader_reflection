#include "shader_reflection/shader_reflection.hpp"
#include "directx/d3d12.h"
#include "directx/d3dx12.h"

int main()
{
    constexpr shader_reflection::options options{
        L"123.hlsl"
    };

    auto result = shader_reflection::reflect(&options);

    return 0;
}