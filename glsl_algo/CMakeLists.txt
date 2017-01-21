cmake_minimum_required(VERSION 3.0.2)

if(MSVC)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()
add_definitions(-DWIN32_LEAN_AND_MEAN)

if(NOT TARGET glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
endif()

file(GLOB SOURCES "src/*.c" "src/shaders/*.c")

add_library(glslalgo STATIC ${SOURCES})
target_link_libraries(glslalgo glad)

target_include_directories(glslalgo PUBLIC include)