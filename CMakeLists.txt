cmake_minimum_required(VERSION 3.20)
project(YetAnotherPhysicsEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(AX_ENGINE_DIR "${CMAKE_SOURCE_DIR}/Engine" CACHE PATH "Engine root folder")
# set(AX_PROJECT_DIR "${CMAKE_SOURCE_DIR}/Sandbox" CACHE PATH "Project root folder")

add_subdirectory(Engine)
add_subdirectory(Sandbox)