#pragma once

#include "AxiomEngine.h"
#include "UI/EditorUI.h"

using namespace axiom;

namespace axiom
{
    class WindowResizeEvent;

    /// The Editor application: mounts its paths, builds the EditorUI and loads the startup scene.
    ///
    /// It lives in the EditorApp library so other executables can run it; Main.cpp is the real entry point.
    ///
    /// @code{.cpp}
    /// return MakeUnique<Editor>(Editor::DefaultConfig());
    /// @endcode
    class Editor : public Application
    {
    public:
        static AppConfig DefaultConfig();

        Editor(AppConfig appConfig);
        ~Editor() = default;
        void OnResize(const WindowResizeEvent& event);

    protected:
        virtual void OnRegisterModules() override;
        virtual void OnInitializeModules() override;
        virtual void OnApplicationRun() override;
        virtual void OnRender() override;
        virtual void OnUpdate(float deltaTime) override;

        EditorUI& GetEditorUI() { return *m_editorUI; }

    private:
        UniquePtr<EditorUI> m_editorUI;
    };

}
