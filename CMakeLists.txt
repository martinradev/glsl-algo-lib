cmake_minimum_required(VERSION 3.0.2)

add_subdirectory(deps/glad)

set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "")
add_subdirectory(deps/glfw)

add_subdirectory(common)
add_subdirectory(glsl_algo)
add_subdirectory(tests)
add_subdirectory(benchmark)

