#include "shader_reflection/shader_reflection.hpp"
#include "shader_assets/shaders.hpp"

int main()
{
    shader_reflection::compile_option options;
    options.file_path = SHADER_FILE_W("test1.hlsl");
    auto result = shader_reflection::reflect(&options);
    return 0;
}