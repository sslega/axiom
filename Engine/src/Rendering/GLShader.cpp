#include "Axiom/Rendering/GLShader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace axiom
{
    GLShader::GLShader(const Path &path)
    :Shader(path)
    {
        Load();
        ParseData();
        Compile();
    }

    GLShader::~GLShader()
    {
        glDeleteProgram(m_ID);
    }

    void GLShader::Bind() const
    {
        glUseProgram(m_ID);
    }


    void GLShader::Unbind() const
    {
        glUseProgram(0);
    }

    const char *GLShader::GetVertexShader()
    {
        return m_VertexShader;
    }

    const char* GLShader::GetFragmentShader()
    {
        return m_FragmentShader;
    }

    void GLShader::ParseData()
    {
        const String& shaderSource = GetRawData().data();
        
        const String typeToken = "#type";
        size_t pos = 0;
        

        for(int i = 0; i < 3; ++i)
        {
            size_t typePos = shaderSource.find(typeToken, pos);
            if (typePos == std::string::npos)
            {
                break;
            }

            size_t eol = shaderSource.find_first_of("\r\n", typePos);
            if (eol == std::string::npos)
            {
                throw std::runtime_error("Syntax error: missing end of line after #type");
            }

            size_t begin = typePos + typeToken.size() + 1; // +1 for space
            std::string stageStr = shaderSource.substr(begin, eol - begin);
           

            size_t nextType = shaderSource.find(typeToken, eol);
            String stageSource = shaderSource.substr(eol + 1, nextType - (eol + 1));
            
            if(stageStr == "vertex")
            {
                m_VertexShader = stageSource.c_str();
            }
            else if(stageStr == "fragment")
            {
                m_FragmentShader = stageSource.c_str();
            }
            else
            {
                throw std::runtime_error("Unknown shader stage: " + stageStr);
            }

            pos = nextType;
        }
    }

    void GLShader::Compile()
    {
        int success;

        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &m_VertexShader, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        }

        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &m_FragmentShader, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        }
        

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertex);
        glAttachShader(m_ID, fragment);
        glLinkProgram(m_ID);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
}

