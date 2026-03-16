cmake_minimum_required(VERSION 3.20)
project(YetAnotherPhysicsEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENGINE_ROOT "${CMAKE_SOURCE_DIR}/Engine" CACHE PATH "Engine root folder")
set(SANDBOX_ROOT "${CMAKE_SOURCE_DIR}/Sandbox" CACHE PATH "Sandbox root folder")

add_subdirectory(Engine)
add_subdirectory(Sandbox)