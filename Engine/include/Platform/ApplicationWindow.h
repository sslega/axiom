#pragma once

#include "Core/Types.h"
#include "Event/Event.h"

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

    class ApplicationWindow : public EventDispatcher
    {
    public:
        virtual ~ApplicationWindow() = default;
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

        static UniquePtr<ApplicationWindow> Create(const AppWindowConfig& desc);
    };
}