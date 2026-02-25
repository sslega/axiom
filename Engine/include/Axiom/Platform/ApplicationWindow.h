#pragma once

#include "axiom/Core/CoreTypes.h"

class GLFWwindow;

namespace axiom
{
    struct ApplicationWindowDesc
    {
        int width = 800;
        int height = 600;
        bool fullscreen = false;
        bool vsync = true;
        String title = "Axiom Engine Window";
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

        static UniquePtr<IApplicationWindow> Create(const ApplicationWindowDesc& desc);
    };

    class ApplicationWindow : public IApplicationWindow
    {
        public:  
        ApplicationWindow(const ApplicationWindowDesc& desc);
        ~ApplicationWindow();

        void PoolEvents() override;
        void Update() override;
        void Render() override;
        bool ShouldClose() const override;
        void CloseWindow() override;

        uint32 Width() const override;
        uint32 Height() const override;
        StringView Title() const override;

        GLFWwindow* m_window;
        ApplicationWindowDesc m_desc;
    };
}