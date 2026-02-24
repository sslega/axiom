#pragma once

#include "axiom/Core/CoreTypes.h"

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
}