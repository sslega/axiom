#include "axiom/Renderer/OpenGLRenderer.h"
#include <GLFW/glfw3.h>
#include <iostream>


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

    // void InitializeShaders()
    // {
    //     const char* vertexShaderSrc = R"(
    //     #version 330 core
    //     layout (location = 0) in vec3 aPos;

    //     void main()
    //     {
    //         gl_Position = vec4(aPos, 1.0);
    //     }
    //     )";

    //     const char* fragmentShaderSrc = R"(
    //     #version 330 core
    //     out vec4 FragColor;

    //     void main()
    //     {
    //         FragColor = vec4(0.2, 0.7, 1.0, 1.0);
    //     }
    //     )";

    //     GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //     glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    //     glCompileShader(vertexShader);

    //     GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //     glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
    //     glCompileShader(fragmentShader);

    //     GLuint shaderProgram = glCreateProgram();
    //     glAttachShader(shaderProgram, vertexShader);
    //     glAttachShader(shaderProgram, fragmentShader);
    //     glLinkProgram(shaderProgram);

    //     glDeleteShader(vertexShader);
    //     glDeleteShader(fragmentShader);
    // }

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
