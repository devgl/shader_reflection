add_rules("mode.debug", "mode.release")

set_project("shader reflection")
set_languages("c++20")

includes("dx_headers")
includes("lib_shader_reflection")
includes("shader_reflector")
includes("shader_assets")
includes("dxc")