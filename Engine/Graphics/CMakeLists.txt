file(GLOB_RECURSE GRAPHICS_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_library(Graphics STATIC ${GRAPHICS_SOURCES})

target_compile_options(Graphics PRIVATE /W3)

target_include_directories(Graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_precompile_headers(Graphics PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../pch.h
)

target_link_libraries(Graphics
    PUBLIC
        Simulation
        imgui
    PRIVATE
        glad
        glfw
)
