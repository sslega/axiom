file(GLOB_RECURSE ENGINECORE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_library(EngineCore STATIC ${ENGINECORE_SOURCES})

target_compile_options(EngineCore PRIVATE /W3)

target_include_directories(EngineCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_precompile_headers(EngineCore PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../pch.h
)

target_compile_definitions(EngineCore PUBLIC
    $<$<CONFIG:Debug>:AX_DEBUG>
)

target_link_libraries(EngineCore PUBLIC
    nlohmann_json::nlohmann_json
)
