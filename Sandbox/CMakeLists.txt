file(GLOB_RECURSE SANDBOX_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(Sandbox ${SANDBOX_SOURCES})

target_include_directories(Sandbox
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(Sandbox PRIVATE
    AX_PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Assets"
)

target_link_libraries(Sandbox PRIVATE Engine)