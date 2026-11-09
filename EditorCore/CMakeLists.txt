file(GLOB_RECURSE EDITOR_CORE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_library(EditorCore STATIC ${EDITOR_CORE_SOURCES})

target_compile_options(EditorCore PRIVATE /W3)

target_include_directories(EditorCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(EditorCore PUBLIC EngineCore)
