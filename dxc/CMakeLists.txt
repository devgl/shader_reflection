add_library(DXC INTERFACE)

target_include_directories(DXC INTERFACE "inc")
target_link_directories(DXC INTERFACE "lib/x64")
target_link_libraries(DXC INTERFACE "dxcompiler.lib")