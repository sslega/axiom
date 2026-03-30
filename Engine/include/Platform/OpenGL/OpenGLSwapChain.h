#pragma once

#include "Renderer/SwapChain.h"

class GLFWwindow;

namespace axiom
{
    class OpenGLSwapChain : public SwapChain
    {
    public:
        OpenGLSwapChain(const ApplicationWindow& window);
        virtual void SwapBuffers() override;
        
    private:
        GLFWwindow* m_windowHandle;
    };
}