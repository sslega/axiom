file(GLOB_RECURSE RUNTIME_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_library(Runtime STATIC ${RUNTIME_SOURCES})

target_compile_options(Runtime PRIVATE /W3)

target_include_directories(Runtime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_precompile_headers(Runtime PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../pch.h
)

target_compile_definitions(Runtime PUBLIC
    AX_ENGINE_DIR="${CMAKE_SOURCE_DIR}/Engine/Assets"
)

target_link_libraries(Runtime PUBLIC
    Graphics
)
