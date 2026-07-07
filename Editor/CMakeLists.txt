file(GLOB_RECURSE EDITOR_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(Editor ${EDITOR_SOURCES})

target_include_directories(Editor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(Editor PRIVATE
    AX_EDITOR_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Assets"
    AX_DEV_PROJECT_DIR="${CMAKE_SOURCE_DIR}/Game/Assets"
)

target_link_libraries(Editor PRIVATE Engine)