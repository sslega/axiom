#pragma once

#include "AxiomEngine.h"
#include "UI/EditorUI.h"

using namespace axiom;

namespace axiom
{
    class WindowResizeEvent;

    class Editor : public Application
    {
    public:
        Editor(AppConfig appConfig);
        ~Editor() = default;
        void OnResize(const WindowResizeEvent& event);

    protected:
        virtual void OnRegisterModules() override;
        virtual void OnInitializeModules() override;
        virtual void OnApplicationRun() override;
        virtual void OnRender() override;
        virtual void OnUpdate(float deltaTime) override;
    
    private:
        UniquePtr<EditorUI> m_editorUI;
    };

}