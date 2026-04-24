#pragma once

#include "Platform/ApplicationWindow.h"

struct GLFWwindow;

namespace axiom
{
    class GLFWWindow : public ApplicationWindow
    {
    public:
        GLFWWindow(const AppWindowConfig& desc);
        ~GLFWWindow();

        bool ShouldClose() const override;
        void CloseWindow() override;

        uint32 GetWidth() const override;
        uint32 GetHeight() const override;
        float GetAspectRatio() const override;
        StringView GetTitle() const override;

        void* GetNativeWindow() const override;

    protected:
        GLFWwindow* m_window;

        void OnPollEvents() override;
        void OnUpdate() override;
    };
}
