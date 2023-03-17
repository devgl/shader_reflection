target("shader_reflector")
    add_files("*.cpp")
    add_deps("lib_shader_reflection", "shader_assets")
target_end()