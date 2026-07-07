#pragma once

#include "AxiomEngine.h"
#include "EditorCamera.h"

using namespace axiom;

namespace axiom
{
    class Shader;
    class WindowResizeEvent;
    class KeyboardEvent;
    class Texture2D;
    class CameraComponent;

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
        EditorCamera m_viewportCamera;
        SharedPtr<FrameBuffer> m_viewportFrameBuffer;
        Vec2 m_lastViewportSize {0, 0};
        bool m_viewportFocused {false};

        EditorCamera m_viewportCamera2;
        SharedPtr<FrameBuffer> m_viewportFrameBuffer2;
        Vec2 m_lastViewportSize2 {0, 0};
        bool m_viewport2Focused {false};
    };

}