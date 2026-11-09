#pragma once

#include "EditorHarness.h"

#include <doctest/doctest.h>

#include <functional>

namespace axiom
{
    class Entity;
    class Selection;
    class UndoStack;
}

namespace axiom::tests
{
    /// Fixture for runtime tests: the real Editor with an empty scene, selection and undo history.
    ///
    /// @code{.cpp}
    /// TEST_CASE_FIXTURE(RuntimeTest, "undoing a selected entity's creation deselects it")
    /// {
    ///     CreateSelectedEntity();
    ///     Undo();
    ///     RunFrames(2);
    ///     CHECK(GetSelection().Get() == nullptr);
    /// }
    /// @endcode
    ///
    /// Any error logged during the case, including while the Editor boots for it, fails the case unless
    /// the case declares it with ExpectLoggedErrors(). Cases never depend on each other's order or leftovers.
    ///
    /// Don't use REQUIRE inside a DuringNextFrame() callback: it would throw through a half-finished frame.
    /// Change the scene in the callback and check the result after it returns.
    class RuntimeTest
    {
    public:
        RuntimeTest();
        ~RuntimeTest();

    protected:
        void RunFrames(int count);

        /// Runs one frame and calls fn at the end of its OnRender, where the UI makes its own changes.
        void DuringNextFrame(std::function<void()> fn);

        void LoadMainScene();
        Scene& GetScene();
        Selection& GetSelection();
        UndoStack& GetUndoStack();
        void Undo();
        void Redo();

        /// Creates an entity through the undo stack and selects it, as the Outliner does.
        Entity* CreateSelectedEntity();

        /// Loads the main scene and selects its first entity with a transform and mesh, so the gizmo
        /// and the Inspector's resource fields run in later frames.
        Entity* LoadMainSceneAndSelectMesh();

        void ExpectLoggedErrors(uint32 count);

        ResourceSubsystem& GetResources();
        FileSubsystem& GetFiles();

        EditorUI& GetUI();

    private:
        // Taken first, so errors logged while booting count against this case.
        uint32 m_errorsAtStart;
        uint32 m_expectedErrors = 0;
        EditorHarness& m_editor;
    };
}
