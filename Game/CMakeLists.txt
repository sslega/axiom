file(GLOB_RECURSE GAME_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(Game ${GAME_SOURCES})

target_include_directories(Game
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(Game PRIVATE
    AX_PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Assets"
)

target_link_libraries(Game PRIVATE Engine)