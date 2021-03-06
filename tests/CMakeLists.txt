cmake_minimum_required(VERSION 3.0.2)

if(MSVC)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

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

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/googletest/googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest)

if(NOT TARGET glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
endif()

if(NOT TARGET glfw)
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../deps/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
endif()

if(NOT TARGET glslalgo_common)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../common ${CMAKE_CURRENT_BINARY_DIR}/common)
endif()

if(NOT TARGET glslalgo)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../glsl_algo ${CMAKE_CURRENT_BINARY_DIR}/glsl_algo)
endif()

file(GLOB SOURCES "src/*.cpp")

add_executable(glsl_algo_unittests ${SOURCES})

include(FindOpenGL)

if(NOT OPENGL_FOUND)
	message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

include_directories(${OPENGL_INCLUDE_DIR})

target_link_libraries(glsl_algo_unittests glslalgo glslalgo_common gtest ${OPENGL_gl_LIBRARY} glfw glad)
