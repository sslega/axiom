#pragma once

#include <iostream>

struct GLFWwindow;

namespace axiom
{
    struct WindowDesc
    {
        int width = 800;
        int height = 600;
        bool fullscreen = false;
        bool vsync = true;
        std::string title = "Axiom Engine Window";
    };

    class IWindow
    {
    public:
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual bool ShouldClose() const = 0;
        virtual void CloseWindow() = 0;

        virtual std::uint32_t Width()  const = 0;
        virtual std::uint32_t Height() const = 0;
        virtual std::string_view Title() const = 0;
    };
    
    class GlfwWindow : public IWindow
    {
    public:
        GlfwWindow (WindowDesc desc);
        ~GlfwWindow () = default;

        void Update() override;
        void Render() override;
        bool ShouldClose() const override;
        void CloseWindow() override;
        std::uint32_t Width()  const override;
        std::uint32_t Height() const override;  
        std::string_view Title() const override;

    private:
        GLFWwindow* m_window;

    };
}