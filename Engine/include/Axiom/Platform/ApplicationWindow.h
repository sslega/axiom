#pragma once

#include "axiom/Core/Types.h"
#include "axiom/Core/Memory.h"

class GLFWwindow;

namespace axiom
{
    enum class ApplicationWindowBackend
    {
        GLFW,
        Win32
    };

    struct ApplicationWindowConfig
    {
        int width = 800;
        int height = 600;
        bool fullscreen = false;
        bool vsync = true;
        String title = "Axiom Engine Window";
        ApplicationWindowBackend backend = ApplicationWindowBackend::GLFW;
    };

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
        virtual StringView Title() const = 0;

        virtual void* GetNativeWindow() const = 0;

        static UniquePtr<IApplicationWindow> Create(const ApplicationWindowConfig& desc);
    };

    class ApplicationWindow : public IApplicationWindow
    {
        public:  
        ApplicationWindow(const ApplicationWindowConfig& desc);
        ~ApplicationWindow();

        void PoolEvents() override;
        void Update() override;
        void Render() override;
        bool ShouldClose() const override;
        void CloseWindow() override;

        uint32 Width() const override;
        uint32 Height() const override;
        StringView Title() const override;

        void* GetNativeWindow() const override;

        GLFWwindow* m_window;
        ApplicationWindowConfig m_desc;
    };
}