target("lib_shader_reflection")
    set_kind("shared")
    add_files("src/*.cpp")
    add_includedirs("include", {public=true})
    add_headerfiles("include/**.hpp")
    add_defines("SHADER_REFLECTION_IMPL")

    add_deps("dx_headers", {public=true})
    add_deps("dxc", {public=true})
    add_links("dxcompiler")

    after_build(function (target) 
        -- copy the dxc dll to the output path
        local dxc_dlls_output = "$(buildir)/$(plat)/$(arch)/$(mode)/"
        os.cp("$(projectdir)/dxc/bin/$(arch)/*.dll", dxc_dlls_output)
    end)
target_end()