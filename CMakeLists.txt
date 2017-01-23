cmake_minimum_required(VERSION 3.0.2)

add_subdirectory(deps/glad)

set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "")
add_subdirectory(deps/glfw)

if(NOT TARGET glslalgo_common)
add_subdirectory(common)
endif()

if(NOT TARGET glslalgo)
add_subdirectory(glsl_algo)
endif()

add_subdirectory(tests)
add_subdirectory(benchmark)

