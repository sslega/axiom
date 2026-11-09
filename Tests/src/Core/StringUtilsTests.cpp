#include "Core/StringUtils.h"

#include <doctest/doctest.h>

namespace axiom::tests
{
    TEST_CASE("paths that differ only by case lower to the same key")
    {
        CHECK(ToLower("engine://Meshes/Cube.obj") == ToLower("ENGINE://meshes/cube.OBJ"));
        CHECK(ToLower("engine://Meshes/Cube.obj") == "engine://meshes/cube.obj");
    }

    TEST_CASE("lowering keeps paths that really differ apart")
    {
        CHECK(ToLower("engine://Meshes/cube.obj") != ToLower("engine://Meshes/sphere.obj"));
        CHECK(ToLower("engine://Meshes/cube.obj") != ToLower("engine://Meshes/cube.obj2"));
        CHECK(ToLower("").empty());
    }
}
