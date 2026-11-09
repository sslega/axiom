#include "ResourceListing.h"

#include <doctest/doctest.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

namespace axiom::tests
{
    namespace
    {
        // A real folder for one test, removed when the test ends.
        struct TempFolder
        {
            Path path;

            TempFolder()
            {
                const auto stamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
                path = fs::temp_directory_path() / ("AxiomTests_" + std::to_string(stamp));
                fs::create_directories(path);
            }

            ~TempFolder()
            {
                std::error_code error;
                fs::remove_all(path, error);
            }

            void Touch(const String& relativePath) const
            {
                const Path file = path / relativePath;
                fs::create_directories(file.parent_path());
                std::ofstream stream(file);
            }
        };
    }

    TEST_CASE("listing keeps only the wanted extensions, in every subfolder")
    {
        TempFolder folder;
        folder.Touch("Meshes/cube.obj");
        folder.Touch("Meshes/Deep/er/rock.obj");
        folder.Touch("Materials/Lit.mat");
        folder.Touch("readme");
        folder.Touch("Meshes/cube.objx");

        const Vector<String> files = ListResourceFiles({ { folder.path, "engine://" } }, { ".obj" });

        CHECK(files == Vector<String>{ "engine://Meshes/cube.obj", "engine://Meshes/Deep/er/rock.obj" });
    }

    TEST_CASE("listing several extensions covers each of them")
    {
        TempFolder folder;
        folder.Touch("a.png");
        folder.Touch("b.jpg");
        folder.Touch("c.obj");

        const Vector<String> files = ListResourceFiles({ { folder.path, "project://" } }, { ".png", ".jpg" });

        CHECK(files == Vector<String>{ "project://a.png", "project://b.jpg" });
    }

    TEST_CASE("listing ignores the case of an extension, as loading does")
    {
        TempFolder folder;
        folder.Touch("UPPER.OBJ");

        const Vector<String> files = ListResourceFiles({ { folder.path, "engine://" } }, { ".obj" });

        CHECK(files == Vector<String>{ "engine://UPPER.OBJ" });
    }

    TEST_CASE("listing sorts by name without regard to case, across roots")
    {
        TempFolder engine;
        TempFolder project;
        engine.Touch("Zebra.obj");
        engine.Touch("apple.obj");
        project.Touch("bunny.obj");

        const Vector<String> files = ListResourceFiles(
            { { project.path, "project://" }, { engine.path, "engine://" } }, { ".obj" });

        CHECK(files == Vector<String>{ "engine://apple.obj", "engine://Zebra.obj", "project://bunny.obj" });
    }

    TEST_CASE("listing accepts a root that ends in a separator")
    {
        TempFolder folder;
        folder.Touch("Meshes/cube.obj");

        const Vector<String> files = ListResourceFiles({ { folder.path / "", "engine://" } }, { ".obj" });

        CHECK(files == Vector<String>{ "engine://Meshes/cube.obj" });
    }

    TEST_CASE("listing skips a root that doesn't exist")
    {
        TempFolder folder;
        folder.Touch("cube.obj");

        const Vector<String> files = ListResourceFiles(
            { { folder.path / "missing", "engine://" }, { folder.path, "project://" } }, { ".obj" });

        CHECK(files == Vector<String>{ "project://cube.obj" });
    }

    TEST_CASE("listing with no extensions finds nothing")
    {
        TempFolder folder;
        folder.Touch("cube.obj");

        CHECK(ListResourceFiles({ { folder.path, "engine://" } }, {}).empty());
    }

    TEST_CASE("listing sees a file added since the last call")
    {
        TempFolder folder;
        folder.Touch("cube.obj");
        const Vector<ResourceRoot> roots = { { folder.path, "engine://" } };
        CHECK(ListResourceFiles(roots, { ".obj" }).size() == 1);

        folder.Touch("sphere.obj");

        CHECK(ListResourceFiles(roots, { ".obj" }).size() == 2);
    }
}
