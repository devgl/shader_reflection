target("dx_headers")
    set_kind("static")
    add_files("src/*.cpp")
    add_headerfiles("include/**.h")
    add_includedirs("include", {public=true})
    add_includedirs("include/directx")
target_end()