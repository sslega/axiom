cmake_minimum_required(VERSION 3.28)
project(Axiom LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(AX_ENGINE_DIR "${CMAKE_SOURCE_DIR}/Engine" CACHE PATH "Engine root folder")
# set(AX_PROJECT_DIR "${CMAKE_SOURCE_DIR}/Game" CACHE PATH "Project root folder")

enable_testing()

add_subdirectory(Engine)
add_subdirectory(EditorCore)
add_subdirectory(Editor)
add_subdirectory(Game)
add_subdirectory(Tests)