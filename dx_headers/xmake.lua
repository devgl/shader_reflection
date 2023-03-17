target("dx_headers")
    -- version 1.608.2

    set_kind("static")
    add_files("src/*.cpp")
    add_headerfiles("include/**.h")
    add_includedirs("include", {public=true})
    add_includedirs("include/directx", {public=true})
target_end()