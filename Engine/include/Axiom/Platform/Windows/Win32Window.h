#pragma once

#include "axiom/Platform/ApplicationWindow.h"
#include <windows.h>

namespace axiom
{
    class Win32Window : public IApplicationWindow
    {
    public:
        Win32Window (ApplicationWindowDesc desc);
        ~Win32Window () = default;

        void PoolEvents();
        void Update() override;
        void Render() override;
        bool ShouldClose() const override;
        void CloseWindow() override;
        uint32 Width()  const override;
        uint32 Height() const override;  
        StringView Title() const override;

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        HWND m_hwnd = nullptr;
        ApplicationWindowDesc m_desc;
        bool m_shouldClose = false;
        
        
    };
}