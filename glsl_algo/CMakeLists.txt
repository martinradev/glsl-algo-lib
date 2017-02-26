cmake_minimum_required(VERSION 3.0.2)

if(MSVC)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()

add_definitions(-DIS_BENCHMARK_BUILD)

set(CMAKE_BUILD_TYPE Release)

file(GLOB SOURCES "src/*.c" "src/shaders/*.c")

add_library(glslalgo STATIC ${SOURCES})

target_include_directories(glslalgo PUBLIC include)