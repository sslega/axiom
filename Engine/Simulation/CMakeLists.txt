file(GLOB_RECURSE SIMULATION_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_library(Simulation STATIC ${SIMULATION_SOURCES})

target_compile_options(Simulation PRIVATE /W3)

target_include_directories(Simulation
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_precompile_headers(Simulation PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../pch.h
)

# Public because the OpenGL texture code in Graphics also decodes images with it.
target_link_libraries(Simulation PUBLIC
    EngineCore
    stb
)
