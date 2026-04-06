#pragma once

#include "Platform/ApplicationWindow.h"
#include <windows.h>

namespace axiom
{
    class Win32Window : public ApplicationWindow
    {
    public:
        Win32Window (AppWindowConfig desc);
        ~Win32Window () = default;

        void PoolEvents() override;
        void Update() override;
        void Render() override;
        bool ShouldClose() const override;
        void CloseWindow() override;
        uint32 Width()  const override;
        uint32 Height() const override;
        float AspectRatio() const override;
        StringView Title() const override;

        void* GetNativeWindow() const override;

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        HWND m_hwnd = nullptr;
        AppWindowConfig m_desc;
        bool m_shouldClose = false;
        
        
    };
}