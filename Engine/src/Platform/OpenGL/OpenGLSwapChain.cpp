#include "Platform/OpenGL/OpenGLSwapChain.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Core/Assert.h"
#include "Platform/ApplicationWindow.h"

namespace axiom
{
    OpenGLSwapChain::OpenGLSwapChain(const ApplicationWindow &window)
    {
        m_windowHandle = static_cast<GLFWwindow*>(window.GetNativeWindow());
        AX_ASSERT(m_windowHandle, "WindowHandle is null!");

        glfwMakeContextCurrent(m_windowHandle);
        
        int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        AX_ASSERT(gladStatus, "Failed to initialize Glad!");

        int width, height;
        glfwGetFramebufferSize(m_windowHandle, &width, &height);
        glViewport(0, 0, width, height);

        printf("OpenGLSwapChain initialized\n");

        // glfwSwapInterval(1);
    }

    void OpenGLSwapChain::SwapBuffers()
    {
        glfwSwapBuffers(m_windowHandle);
    }
}
