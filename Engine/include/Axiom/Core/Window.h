#pragma once

#include <iostream>

struct GLFWwindow;

namespace ax
{
    struct WindowDesc
    {
        int width = 800;
        int height = 600;
        bool fullscreen = false;
        bool vsync = true;
        std::string title = "Axiom Engine";
    };
    
    class Window
    {
    public:
        Window(WindowDesc desc);
        ~Window() = default;

        void Update();
        void Render();

        bool ShouldClose() const;

        void CloseWindow();

    private:
        GLFWwindow* m_window;

    };
}