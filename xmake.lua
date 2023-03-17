add_rules("mode.debug", "mode.release")

set_project("dx12pg")
set_languages("c++20")

includes("dx_headers")
includes("lib_shader_reflection")
includes("shader_reflector")