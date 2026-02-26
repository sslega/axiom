#include "axiom/Renderer/OpenGLRenderer.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <GL/gl.h> // basic OpenGL functions available on Windows

namespace axiom
{
    bool OpenGLRenderer::Initialize(void* nativeWindowHandle)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(nativeWindowHandle);
        if (window)
        {
            glfwMakeContextCurrent(window);
        }

        // nothing more for now; context is already created by the window.
        std::cout << "OpenGLRenderer initialized" << std::endl;
        return true;
    }

    void OpenGLRenderer::Shutdown()
    {
    }

    void OpenGLRenderer::BeginFrame()
    {
        // set a clear colour and clear the backbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGLRenderer::EndFrame()
    {
        GLFWwindow* window = glfwGetCurrentContext();
        if (window)
            glfwSwapBuffers(window);
    }

    void OpenGLRenderer::Resize(uint32 width, uint32 height)
    {
        glViewport(0, 0, width, height);
    }
}
