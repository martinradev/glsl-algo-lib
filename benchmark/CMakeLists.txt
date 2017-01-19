cmake_minimum_required(VERSION 3.0.2)
project(glsl_algo_benchmark)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/benchmark ${CMAKE_CURRENT_BINARY_DIR}/benchmark)

if(NOT TARGET glslalgo_common)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../common ${CMAKE_CURRENT_BINARY_DIR}/common)
endif()

if(NOT TARGET glslalgo)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../glsl_algo ${CMAKE_CURRENT_BINARY_DIR}/glsl_algo)
endif()

if(NOT TARGET glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
endif()

if(NOT TARGET glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
endif()

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(glsl_algo_benchmark ${SOURCES})

find_package(OpenGL REQUIRED)

target_link_libraries(glsl_algo_benchmark glslalgo glslalgo_common glfw glad GL benchmark)