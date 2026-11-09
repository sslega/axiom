#pragma once

#include "Editor.h"

#include <functional>

namespace axiom::tests
{
    /// The real Editor, stepped frame by frame by the test harness instead of its own main loop.
    ///
    /// One instance serves the whole run: it boots on first use, and its window and GL context stay up
    /// between cases. Reach it through the RuntimeTest fixture; only that and main.cpp touch it directly.
    ///
    /// @code{.cpp}
    /// EditorHarness& editor = EditorHarness::GetOrBoot();
    /// editor.QueueDuringRender([&] { editor.GetEditorUI().GetUndoStack().Undo(); });
    /// editor.RunFrame();
    /// @endcode
    ///
    /// The layout file stays off, so a run can't overwrite the user's. Set the window's visibility before
    /// the first boot; it can't change afterwards.
    class EditorHarness : public Editor
    {
    public:
        explicit EditorHarness(AppConfig appConfig);

        static EditorHarness& GetOrBoot();
        static void ShutdownIfBooted();
        static void SetWindowVisible(bool visible);

        using Application::Startup;
        using Application::RunFrame;
        using Application::Shutdown;
        using Editor::GetEditorUI;

        /// Runs the callback once, at the end of the next OnRender, where the UI makes its own changes.
        void QueueDuringRender(std::function<void()> callback);

    protected:
        void OnInitializeModules() override;
        void OnRender() override;

    private:
        std::function<void()> m_duringRender;
    };
}
