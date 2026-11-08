# =========================================================
# Dependencies – ImGuizmo
# =========================================================

include(FetchContent)

FetchContent_Declare(imguizmo
    GIT_REPOSITORY  https://github.com/CedricGuillemet/ImGuizmo.git
    # Pinned to a commit. A commit hash can't be used with GIT_SHALLOW.
    GIT_TAG         105e4fde45e4e74aae8973d98750820ef7aca7e2
    GIT_PROGRESS    TRUE
    # The repo's root CMakeLists.txt must not join the build, so point FetchContent at a folder without one.
    SOURCE_SUBDIR   no-cmake-entry-point
)
FetchContent_MakeAvailable(imguizmo)

# =========================================================
# ImGuizmo – manual static target
# =========================================================

add_library(imguizmo STATIC
    ${imguizmo_SOURCE_DIR}/src/ImGuizmo.cpp
)

target_include_directories(imguizmo PUBLIC
    ${imguizmo_SOURCE_DIR}/src
)

target_link_libraries(imguizmo PUBLIC imgui)

# =========================================================
# Editor
# =========================================================

file(GLOB_RECURSE EDITOR_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(Editor ${EDITOR_SOURCES})

target_compile_options(Editor PRIVATE /W3)

target_include_directories(Editor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(Editor PRIVATE
    AX_EDITOR_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Assets"
    AX_DEV_PROJECT_DIR="${CMAKE_SOURCE_DIR}/Game/Assets"
)

target_link_libraries(Editor PRIVATE Engine imguizmo)
