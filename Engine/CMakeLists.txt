cmake_minimum_required(VERSION 3.28)

project(Engine LANGUAGES CXX)

# =========================================================
# Compiler settings
# =========================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
# Dependencies – ImGui
# =========================================================

FetchContent_Declare(imgui
    GIT_REPOSITORY  https://github.com/ocornut/imgui.git
    # docking branch, pinned to a commit. A commit hash can't be used with GIT_SHALLOW.
    GIT_TAG         1ab953d75a760d5102eb2d5c4c6c4e65df6b29f3
    GIT_PROGRESS    TRUE
    DOWNLOAD_ONLY   YES
)
FetchContent_MakeAvailable(imgui)

# =========================================================
# OpenGL (must come before imgui target which uses OpenGL::GL)
# =========================================================

find_package(OpenGL REQUIRED)

# =========================================================
# ImGui – manual static target (ImGui has no CMakeLists.txt)
# =========================================================

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

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
# STB (header-only)
# =========================================================

# add_library(stb INTERFACE)

# target_include_directories(stb INTERFACE
#     ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
# )

add_library(stb
    STATIC
        external/stb/src/stb_image.cpp
)

target_include_directories(stb PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb/include
)

# =========================================================
# nlohmann/json (header-only)
# =========================================================

FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY  https://github.com/nlohmann/json.git
    GIT_TAG         v3.11.3
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

# =========================================================
# Engine
# =========================================================

add_library(Engine STATIC)

target_compile_options(Engine PRIVATE /W3)

file(GLOB_RECURSE ENGINE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

target_sources(Engine PRIVATE
    ${ENGINE_SOURCES}
)

target_include_directories(Engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_compile_definitions(Engine PUBLIC
    AX_ENGINE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Assets"
    $<$<CONFIG:Debug>:AX_DEBUG>
)

target_precompile_headers(Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/pch.h
)

target_link_libraries(Engine PUBLIC
    glad
    glfw
    imgui
    stb
    nlohmann_json::nlohmann_json
    OpenGL::GL
)