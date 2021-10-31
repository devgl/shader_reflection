add_executable(Game "main.cpp" "pch.hpp")
target_precompile_headers(Game PRIVATE "pch.hpp")
target_link_libraries(Game PUBLIC DXC)


target_compile_definitions(Game 
	PRIVATE 
		ShaderPath="${CMAKE_CURRENT_LIST_DIR}/shaders/shader.hlsl"
		ShaderBinary="${CMAKE_CURRENT_LIST_DIR}/shaders/shader.bin"
		PDBFilePath="${CMAKE_CURRENT_LIST_DIR}/shaders/shader.pdb")