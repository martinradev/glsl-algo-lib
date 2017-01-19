cmake_minimum_required(VERSION 3.0.2)
project(glsl_algo_common)

if(NOT TARGET glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
endif()

if(NOT TARGET glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
endif()

file(GLOB SOURCES "src/*.cpp")

add_library(glslalgo_common STATIC ${SOURCES})
target_link_libraries(glslalgo_common glad glfw)

target_include_directories(glslalgo_common PUBLIC include)