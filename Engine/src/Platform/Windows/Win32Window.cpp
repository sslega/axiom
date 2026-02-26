#include "axiom/Platform/Windows/Win32Window.h"

namespace axiom
{
    Win32Window::Win32Window(ApplicationWindowConfig desc)
    {
        m_desc = desc;

        const char* CLASS_NAME = "AxiomWin32WindowClass";

        HINSTANCE hInstance = GetModuleHandle(nullptr);

        WNDCLASSEX wc{};
        wc.cbSize        = sizeof(WNDCLASSEX);
        wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc   = Win32Window::WindowProc;
        wc.hInstance     = hInstance;
        wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = CLASS_NAME;
        
        RegisterClassEx(&wc);

        DWORD style = WS_OVERLAPPEDWINDOW;

        RECT rect = { 0, 0, (LONG)desc.width, (LONG)desc.height };
        AdjustWindowRect(&rect, style, FALSE);

        m_hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            desc.title.c_str(),
            style,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            hInstance,
            this
        );

        if (!m_hwnd)
        throw std::runtime_error("Failed to create Win32 window.");

        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);
    }

    void Win32Window::PoolEvents()
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void Win32Window::Update()
    {
    }

    void Win32Window::Render()
    {
    }

    bool Win32Window::ShouldClose() const
    {
        return m_shouldClose;
    }

    void* Win32Window::GetNativeWindow() const
    {
        return m_hwnd;
    }

    void Win32Window::CloseWindow()
    {
        m_shouldClose = true;
    }

    uint32 Win32Window::Width() const
    {
        return m_desc.width;
    }

    uint32 Win32Window::Height() const
    {
        return m_desc.height;
    }

    StringView Win32Window::Title() const
    {
        return m_desc.title;
    }

    LRESULT CALLBACK Win32Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Win32Window* window = reinterpret_cast<Win32Window*>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));

        switch (msg)
        {
            case WM_CLOSE:
            {
                if (window)
                    window->m_shouldClose = true;

                DestroyWindow(hwnd);
                return 0;
            }

            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}