#pragma once

namespace axiom
{
    class ApplicationWindow;

    class SwapChain
    {
    public:
        virtual void SwapBuffers() = 0;        
    };
}