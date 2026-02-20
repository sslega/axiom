add_executable(Runtime src/main.cpp)

target_include_directories(Runtime
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Runtime PRIVATE Engine Game)