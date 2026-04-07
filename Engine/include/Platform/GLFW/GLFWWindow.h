#pragma once

#include "Platform/ApplicationWindow.h"

class GLFWwindow;

namespace axiom
{
    class GLFWWindow : public ApplicationWindow
    {
    public:
        GLFWWindow(const AppWindowConfig& desc);
        ~GLFWWindow();

        void PoolEvents() override;
        void Update() override;
        void Render() override;
        bool ShouldClose() const override;
        void CloseWindow() override;

        uint32 Width() const override;
        uint32 Height() const override;
        float AspectRatio() const override;
        StringView Title() const override;

        void* GetNativeWindow() const override;

        GLFWwindow* m_window;
        AppWindowConfig m_desc;
    };
}
