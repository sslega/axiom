#pragma once

#include "Core/Types.h"
// #include "Renderer/RenderContext.h"

class GLFWwindow;

namespace axiom
{
    enum class ApplicationWindowBackend
    {
        GLFW,
        Win32
    };

    struct AppWindowConfig
    {
        int width = 800;
        int height = 600;
        bool fullscreen = false;
        bool vsync = true;
        String title = "Axiom Engine Window";
        ApplicationWindowBackend backend = ApplicationWindowBackend::GLFW;
    };

    // TODO: Remove IApplicationWindow, make ApplicationWindow base
    class IApplicationWindow
    {
    public:
        virtual ~IApplicationWindow() = default;
        virtual void PoolEvents() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual bool ShouldClose() const = 0;
        virtual void CloseWindow() = 0;

        virtual uint32 Width()  const = 0;
        virtual uint32 Height() const = 0;
        virtual float AspectRatio() const = 0;
        virtual StringView Title() const = 0;

        virtual void* GetNativeWindow() const = 0;

        static UniquePtr<IApplicationWindow> Create(const AppWindowConfig& desc);
    };

    class ApplicationWindow : public IApplicationWindow
    {
        public:  
        ApplicationWindow(const AppWindowConfig& desc);
        ~ApplicationWindow();

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