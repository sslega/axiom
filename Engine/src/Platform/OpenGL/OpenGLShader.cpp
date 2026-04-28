#include "Core/Assert.h"
#include "Core/Log.h"
#include <glad/glad.h>
#include <algorithm>
#include "OpenGLShader.h"

namespace axiom
{
    OpenGLShader::OpenGLShader(const String &vertexSource, const String &fragmentSource)
    : m_vertexSource(vertexSource), m_fragmentSource(fragmentSource)
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const GLchar *source = (const GLchar *)vertexSource.c_str();
        glShaderSource(vertexShader, 1, &source, 0);
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data());
            glDeleteShader(vertexShader);

            Log::Error("Vertex shader compilation failed:\n{}", infoLog.data());
            Log::Error("Source:\n{}", vertexSource);
            AX_ASSERT(false, "Vertex shader compilation failed");
            return;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        source = (const GLchar *)fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data());
            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);

            Log::Error("Fragment shader compilation failed:\n{}", infoLog.data());
            Log::Error("Source:\n{}", fragmentSource);
            AX_ASSERT(false, "Fragment shader compilation failed");
            return;
        }

        m_rendererID = glCreateProgram();
        glAttachShader(m_rendererID, vertexShader);
        glAttachShader(m_rendererID, fragmentShader);
        glLinkProgram(m_rendererID);

        GLint isLinked = 0;
        glGetProgramiv(m_rendererID, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_rendererID, maxLength, &maxLength, infoLog.data());
            glDeleteProgram(m_rendererID);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            Log::Error("Shader linking failed:\n{}", infoLog.data());
            AX_ASSERT(false, "Shader linking failed");
            return;
        }

        glDetachShader(m_rendererID, vertexShader);
        glDetachShader(m_rendererID, fragmentShader);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_rendererID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_rendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::UploadUniform(const String& name, const int& value)
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniform(const String &name, const float& value)
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniform(const String &name, const Vec2& value)
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniform(const String &name, const Vec3& value)
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniform(const String& name, const Vec4& value)
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniform(const String& name, const Matrix4& value)
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, value.data);
    }

    SharedPtr<Shader> OpenGLShader::GetVariant(const Vector<String>& defines)
    {
        Vector<String> sorted = defines;
        std::sort(sorted.begin(), sorted.end());
        String key;
        for (const auto& d : sorted) { key += d; key += ';'; }

        auto it = m_variantCache.find(key);
        if (it != m_variantCache.end()) return it->second;

        auto injectDefines = [&](const String& src) {
            size_t newline = src.find('\n', src.find("#version"));
            String result = src.substr(0, newline + 1);
            for (const auto& d : sorted) result += "#define " + d + " 1\n";
            return result + src.substr(newline + 1);
        };

        auto variant = MakeShared<OpenGLShader>(injectDefines(m_vertexSource), injectDefines(m_fragmentSource));
        m_variantCache[key] = variant;
        return variant;
    }
}
