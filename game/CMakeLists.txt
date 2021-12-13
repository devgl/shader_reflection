add_executable(Game "main.cpp" "pch.hpp")
target_precompile_headers(Game PRIVATE "pch.hpp")
target_link_libraries(Game PUBLIC SPDLOG LibShaderCompiler )
target_compile_definitions(Game PUBLIC _UNICODE UNICODE)
target_compile_features(Game PUBLIC cxx_std_20)
target_compile_options(Game PRIVATE /Zc:preprocessor)
add_library(Shaders INTERFACE "shaders/root_sig.hlsl" "Shaders/shader.hlsl")

target_compile_definitions(Game 
	PRIVATE 
		ShaderPath="${CMAKE_CURRENT_LIST_DIR}/shaders/"
		ShaderFilePath="${CMAKE_CURRENT_LIST_DIR}/shaders/shader.hlsl"
		ShaderBinary="${CMAKE_CURRENT_LIST_DIR}/shaders/shader.bin"
		PDBFilePath="${CMAKE_CURRENT_LIST_DIR}/shaders/shader.pdb"
		
)