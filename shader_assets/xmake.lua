target("shader_assets")
    set_kind("headeronly")
    add_headerfiles("include/**.hpp")
    add_includedirs("include", {public=true})
    local shader_asset_path = string.replace(path.absolute("."), "\\", "/") .. "/"
    add_defines("SHADER_ASSET_PATH=\"" .. shader_asset_path .."\"", {public=true})
target_end()