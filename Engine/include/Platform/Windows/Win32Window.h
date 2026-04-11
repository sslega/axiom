#pragma once

#include "Platform/ApplicationWindow.h"
#include <windows.h>

namespace axiom
{
    class Win32Window : public ApplicationWindow
    {
    public:
        Win32Window(AppWindowConfig desc);
        ~Win32Window() = default;

        bool ShouldClose() const override;
        void CloseWindow() override;
        uint32 Width()  const override;
        uint32 Height() const override;
        float AspectRatio() const override;
        StringView Title() const override;

        void* GetNativeWindow() const override;

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    protected:
        void OnPollEvents() override;
        void OnUpdate() override;

    private:
        HWND m_hwnd = nullptr;
        bool m_shouldClose = false;
    };
}
