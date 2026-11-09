#include "RuntimeTest.h"

#include "Commands/UndoStack.h"
#include "Selection.h"

namespace axiom::tests
{
    TEST_SUITE("runtime")
    {
        TEST_CASE_FIXTURE(RuntimeTest, "a new scene clears the selection and history, and frames keep running")
        {
            CreateSelectedEntity();
            RunFrames(1);

            DuringNextFrame([&] { GetUI().NewScene(); });

            CHECK(GetScene().GetEntities().empty());
            CHECK(GetSelection().Get() == nullptr);
            CHECK_FALSE(GetUndoStack().CanUndo());
            CHECK_FALSE(GetUndoStack().CanRedo());
            RunFrames(2);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "loading a scene clears the selection and history, and frames keep running")
        {
            CreateSelectedEntity();
            RunFrames(1);

            bool loaded = false;
            DuringNextFrame([&] { loaded = GetUI().LoadScene("project://Scenes/main.scene.json"); });

            CHECK(loaded);
            CHECK_FALSE(GetScene().GetEntities().empty());
            CHECK(GetSelection().Get() == nullptr);
            CHECK_FALSE(GetUndoStack().CanUndo());
            CHECK_FALSE(GetUndoStack().CanRedo());
            RunFrames(2);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "a failed load leaves the scene, selection and history as they were")
        {
            ExpectLoggedErrors(1);
            Entity* selected = LoadMainSceneAndSelectMesh();
            CreateSelectedEntity();
            GetSelection().Set(selected);
            const size_t entityCount = GetScene().GetEntities().size();

            CHECK_FALSE(GetUI().LoadScene("project://Scenes/does_not_exist.scene.json"));

            CHECK(GetScene().GetEntities().size() == entityCount);
            CHECK(GetSelection().Get() == selected);
            CHECK(GetUndoStack().CanUndo());
            RunFrames(2);
        }
    }
}
