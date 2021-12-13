set(LibShaderCompiler_HPP
    "Compiler.hpp"
)

set(LibShaderCompiler_CPP
    "Compiler.cpp"
)


add_library(LibShaderCompiler SHARED)
target_sources(LibShaderCompiler PRIVATE ${LibShaderCompiler_HPP} ${LibShaderCompiler_CPP})
target_compile_features(LibShaderCompiler PUBLIC cxx_std_20)
target_link_libraries(LibShaderCompiler PUBLIC DXC)
target_include_directories(LibShaderCompiler PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(LibShaderCompiler PRIVATE LIBSHADERCOMPILER_IMPL)
target_precompile_headers(LibShaderCompiler PUBLIC "pch.hpp")
set_target_properties(
    LibShaderCompiler PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/game/
)