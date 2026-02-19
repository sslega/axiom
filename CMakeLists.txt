cmake_minimum_required(VERSION 3.10)
project(YetAnotherPhysicsEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest   # albo konkretny commit hash!
)

FetchContent_MakeAvailable(glfw)

add_executable(YetAnotherPhysicsEngine src/YetAnotherPhysicsEngine.cpp)
target_link_libraries(YetAnotherPhysicsEngine PRIVATE glfw opengl32)