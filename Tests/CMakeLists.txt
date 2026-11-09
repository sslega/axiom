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
# AxiomTestsLogic - graphics-free tests. Its include path has no ImGui or Graphics headers,
# so including one here is a compile error.
# =========================================================

file(GLOB_RECURSE TESTS_LOGIC_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Logic/*.cpp
)

add_library(AxiomTestsLogic OBJECT ${TESTS_LOGIC_SOURCES})

target_compile_options(AxiomTestsLogic PRIVATE /W3)

target_include_directories(AxiomTestsLogic
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Logic
)

target_link_libraries(AxiomTestsLogic PRIVATE EditorCore doctest::doctest)

# =========================================================
# AxiomTestsRuntime - tests that drive the real Editor through the harness
# =========================================================

file(GLOB_RECURSE TESTS_RUNTIME_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Runtime/*.cpp
)

add_library(AxiomTestsRuntime OBJECT ${TESTS_RUNTIME_SOURCES})

target_compile_options(AxiomTestsRuntime PRIVATE /W3)

target_include_directories(AxiomTestsRuntime
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Harness
)

target_link_libraries(AxiomTestsRuntime PRIVATE EditorApp doctest::doctest)

# =========================================================
# AxiomTests - object libraries link in whole, so every test case stays registered
# =========================================================

file(GLOB_RECURSE TESTS_HARNESS_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Harness/*.cpp
)

add_executable(AxiomTests
    ${TESTS_HARNESS_SOURCES}
    $<TARGET_OBJECTS:AxiomTestsLogic>
    $<TARGET_OBJECTS:AxiomTestsRuntime>
)

target_compile_options(AxiomTests PRIVATE /W3)

target_include_directories(AxiomTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Harness
)

target_link_libraries(AxiomTests PRIVATE EditorApp doctest::doctest)

add_test(NAME AxiomTests COMMAND AxiomTests)
