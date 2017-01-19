cmake_minimum_required(VERSION 3.0.2)

file(GLOB SOURCES "src/*.c")

add_library(glslalgo STATIC ${SOURCES})

target_include_directories(glslalgo PUBLIC include)