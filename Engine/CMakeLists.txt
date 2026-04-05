cmake_minimum_required(VERSION 3.20)

project(Engine LANGUAGES CXX)

# =========================================================
# Compiler settings
# =========================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

# =========================================================
# Dependencies – GLFW
# =========================================================

include(FetchContent)

FetchContent_Declare(glfw
    GIT_REPOSITORY  https://github.com/glfw/glfw.git
    GIT_TAG         3.4
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# =========================================================
# GLAD
# =========================================================

add_library(glad 
    STATIC
        external/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)

# =========================================================
# Engine
# =========================================================

add_library(Engine STATIC)

target_compile_definitions(Engine PUBLIC ENGINE_ROOT_PATH="${ENGINE_ROOT}")


file(GLOB_RECURSE ENGINE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

target_sources(Engine PRIVATE
    ${ENGINE_SOURCES}
)

target_include_directories(Engine 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_compile_definitions(Engine PUBLIC
    AX_ENGINE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Assets"
)

target_precompile_headers(Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/pch.h
)

target_link_libraries(Engine PUBLIC
    glad
    glfw
    OpenGL::GL
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Axiom PRIVATE AX_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(Axiom PRIVATE AX_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Dist")
    target_compile_definitions(Axiom PRIVATE AX_DIST)
endif()

# =========================================================
# OpenGL
# =========================================================

find_package(OpenGL REQUIRED)

message(STATUS "Engine include path: ${CMAKE_CURRENT_SOURCE_DIR}/include/Axiom")