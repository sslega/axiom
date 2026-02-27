cmake_minimum_required(VERSION 3.20)
project(Game LANGUAGES CXX)

# ensure all MSVC targets use the DLL runtime (/MD or /MDd) to avoid
# conflicts such as LNK4098 about MSVCRT vs other libs.  This is a
# global setting and also affects fetched libraries (glfw, etc.).
if(MSVC)
    # the CACHE FORCE ensures it's set before any targets are created
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL" CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

file(GLOB_RECURSE ENGINE_SOURCES
    src/*.cpp
)

add_library(Engine STATIC 
    ${ENGINE_SOURCES}
)

add_library(glad STATIC
    ${CMAKE_CURRENT_LIST_DIR}/external/glad/src/glad.c
)

target_include_directories(glad
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/external/glad/include
)

target_include_directories(Engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Engine PUBLIC
        glad
        glfw
        opengl32
)
