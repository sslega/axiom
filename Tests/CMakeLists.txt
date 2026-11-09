# =========================================================
# Dependencies - doctest
# =========================================================

include(FetchContent)

FetchContent_Declare(doctest
    GIT_REPOSITORY  https://github.com/doctest/doctest.git
    GIT_TAG         v2.4.11
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
)
set(DOCTEST_WITH_TESTS OFF CACHE BOOL "" FORCE)
set(DOCTEST_NO_INSTALL ON CACHE BOOL "" FORCE)
set(DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(doctest)

# =========================================================
# AxiomTests
# =========================================================

file(GLOB_RECURSE TESTS_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(AxiomTests ${TESTS_SOURCES})

target_compile_options(AxiomTests PRIVATE /W3)

target_include_directories(AxiomTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(AxiomTests PRIVATE EditorCore doctest::doctest)

add_test(NAME AxiomTests COMMAND AxiomTests)

# =========================================================
# EditorSmoke - boots the real Editor for a fixed number of frames
# =========================================================

add_executable(EditorSmoke ${CMAKE_CURRENT_SOURCE_DIR}/Smoke/EditorSmoke.cpp)

target_compile_options(EditorSmoke PRIVATE /W3)

target_link_libraries(EditorSmoke PRIVATE EditorApp)
