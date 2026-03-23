#include "Rendering/GL/GLRenderDevice.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>


namespace axiom
{
    bool GLRenderDevice::Initialize(void* nativeWindowHandle)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(nativeWindowHandle);
        assert(window);
        
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            printf("Failed to initialize GLAD\n");
            return false;
        }


        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);


        // if (!CompileShaders())
        // {
        //     printf("Failed to compile shaders\n");
        //     return false;
        // }

        // if (!SetupTriangle())
        // {
        //     printf("Failed to setup triangle\n");
        //     return false;
        // }

        printf("OpenGLRenderDevice initialized\n");
        return true;
    }

    void GLRenderDevice::Shutdown()
    {
        // if (m_ColorVBO)
        // {
        //     glDeleteBuffers(1, &m_ColorVBO);
        //     m_ColorVBO = 0;
        // }
        // if (m_VBO)
        // {
        //     glDeleteBuffers(1, &m_VBO);
        //     m_VBO = 0;
        // }
        // if (m_VAO)
        // {
        //     glDeleteVertexArrays(1, &m_VAO);
        //     m_VAO = 0;
        // }
        // if (m_ShaderProgram)
        // {
        //     glDeleteProgram(m_ShaderProgram);
        //     m_ShaderProgram = 0;
        // }
    }

    void GLRenderDevice::BeginFrame()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glUseProgram(m_ShaderProgram);
        // glBindVertexArray(m_VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0);
        // glUseProgram(0);
    }

    void GLRenderDevice::Draw()
    {
    }
    
    void GLRenderDevice::EndFrame()
    {
        GLFWwindow* window = glfwGetCurrentContext();
        if (window)
        {
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void GLRenderDevice::Resize(uint32 width, uint32 height)
    {
        glViewport(0, 0, width, height);
    }

    // bool GLRenderDevice::CompileShaders()
    // {
    //     const char* vertexShaderSrc = R"(
    //     #version 330 core
    //     layout (location = 0) in vec3 aPos;
    //     layout (location = 1) in vec4 aColor;

    //     out vec4 vertexColor;

    //     void main()
    //     {
    //         gl_Position = vec4(aPos, 1.0);
    //         vertexColor = aColor;
    //     }
    //     )";

    //     const char* fragmentShaderSrc = R"(
    //     #version 330 core
    //     in vec4 vertexColor;
    //     out vec4 FragColor;

    //     void main()
    //     {
    //         FragColor = vertexColor;
    //     }
    //     )";

    //     GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //     glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    //     glCompileShader(vertexShader);
    //     GLint success;
    //     glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    //     if (!success)
    //     {
    //         char infoLog[512];
    //         glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    //         std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
    //         return false;
    //     }

    //     GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //     glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
    //     glCompileShader(fragmentShader);
    //     glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //     if (!success)
    //     {
    //         char infoLog[512];
    //         glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    //         std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
    //         return false;
    //     }

    //     m_ShaderProgram = glCreateProgram();
    //     glAttachShader(m_ShaderProgram, vertexShader);
    //     glAttachShader(m_ShaderProgram, fragmentShader);
    //     glLinkProgram(m_ShaderProgram);
    //     glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
    //     if (!success)
    //     {
    //         char infoLog[512];
    //         glGetProgramInfoLog(m_ShaderProgram, 512, nullptr, infoLog);
    //         std::cerr << "RenderShader program linking failed: " << infoLog << std::endl;
    //         return false;
    //     }

    //     glDeleteShader(vertexShader);
    //     glDeleteShader(fragmentShader);
    //     return true;
    // }

    // bool GLRenderDevice::SetupTriangle()
    // {
    //     float vertices[] = {
    //         // positions
    //         0.0f, 0.5f, 0.0f,
    //         0.5f, -0.5f, 0.0f,
    //         -0.5f, -0.5f, 0.0f
    //     };

    //     float colors[] = {
    //         // positions
    //         1.0f, 0.0f, 0.0f, 1.0f,
    //         0.0f, 1.0f, 0.0f, 1.0f,
    //         0.0f, 0.0f, 1.0f, 1.0f
    //     };

    //     glGenVertexArrays(1, &m_VAO);
    //     glGenBuffers(1, &m_VBO);

    //     glBindVertexArray(m_VAO);

    //     glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    //     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //     glEnableVertexAttribArray(0);
    //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //     uint32_t colorVBO;
    //     glGenBuffers(1, &colorVBO);
    //     glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    //     glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    //     glEnableVertexAttribArray(1);
    //     glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    //     m_ColorVBO = colorVBO;

    //     glBindBuffer(GL_ARRAY_BUFFER, 0);
    //     glBindVertexArray(0);

    //     return true;
    // }
}
