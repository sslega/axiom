#include "Core/Assert.h"
#include "Core/Log.h"
#include <glad/glad.h>
#include <algorithm>
#include "OpenGLShader.h"
#include <regex>

namespace axiom
{
    OpenGLShader::OpenGLShader(const String &vertexSource, const String &fragmentSource, const Vector<String>& sourceMap, const uint32 shaderID)
    : m_vertexSource(vertexSource)
    , m_fragmentSource(fragmentSource)
    , m_sourceMap(sourceMap)
    , m_shaderID(shaderID)
    , m_isValid(true)
    {
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_shaderID);
    }

    SharedPtr<OpenGLShader> OpenGLShader::Create(const String& vertexSource, const String& fragmentSource, const Vector<String>& sourceMap)
    {
        uint32 shaderID;
        if(Compile(vertexSource, fragmentSource, sourceMap, shaderID))
        {
            return SharedPtr<OpenGLShader>(new OpenGLShader(vertexSource, fragmentSource, sourceMap, shaderID));
        }
        return nullptr;
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_shaderID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    bool OpenGLShader::Reload(const String &vertexSource, const String &fragmentSource, const Vector<String>& sourceMap)
    {
        GLuint oldID = m_shaderID;
        if(Compile(vertexSource, fragmentSource, sourceMap, m_shaderID))
        {
            glDeleteProgram(oldID);
            m_vertexSource = vertexSource;
            m_fragmentSource = fragmentSource;
            m_sourceMap = sourceMap;
            m_variantCache.clear();
            m_isValid = true;
            return true;
        }
        m_isValid = false;
        return false;
    }

    void OpenGLShader::UploadUniform(const String& name, const int& value)
    {
        GLint location = glGetUniformLocation(m_shaderID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniform(const String &name, const float& value)
    {
        GLint location = glGetUniformLocation(m_shaderID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniform(const String &name, const Vec2& value)
    {
        GLint location = glGetUniformLocation(m_shaderID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniform(const String &name, const Vec3& value)
    {
        GLint location = glGetUniformLocation(m_shaderID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniform(const String& name, const Vec4& value)
    {
        GLint location = glGetUniformLocation(m_shaderID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniform(const String& name, const Matrix4& value)
    {
        GLint location = glGetUniformLocation(m_shaderID, name.c_str());
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

        auto variant = OpenGLShader::Create(injectDefines(m_vertexSource), injectDefines(m_fragmentSource), m_sourceMap);
        if(variant)
        {   
            m_variantCache[key] = variant;
        } 
        return variant;
    }

    bool OpenGLShader::IsValid() const
    {
        return m_isValid;
    }

    bool OpenGLShader::Compile(const String& vertexSource, const String& fragmentSource, const Vector<String>& sourceMap, uint32& shaderID)
    {
        uint32 id = 0;
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
            String errorLog = String(infoLog.data());
            errorLog = ResolveSourceIndices(errorLog, sourceMap);
            Log::Error("Vertex shader compilation failed:\n{}", errorLog);
            return false;
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
            String errorLog = String(infoLog.data());
            errorLog = ResolveSourceIndices(errorLog, sourceMap);
            Log::Error("Fragment shader compilation failed:\n{}", errorLog);
            return false;
        }

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);

        GLint isLinked = 0;
        glGetProgramiv(id, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(id, maxLength, &maxLength, infoLog.data());
            glDeleteProgram(id);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            Log::Error("Shader linking failed:\n{}", infoLog.data());
            return false;
        }

        glDetachShader(id, vertexShader);
        glDetachShader(id, fragmentShader);
        shaderID = id;
        return true;
    }

    String OpenGLShader::ResolveSourceIndices(const String &errorLog, const Vector<String> &sourceMap)
    {
        std::regex log_regex(R"((\d+)\((\d+)\))");
        auto begin = std::sregex_iterator(errorLog.begin(), errorLog.end(), log_regex);
        auto end   = std::sregex_iterator();

        if (begin == end) return errorLog;

        String result;
        std::smatch match;
        for (auto it = begin; it != end; ++it)
        {
            match = *it;
            result += match.prefix();          // text before this match
            int index = std::stoi(match[1]);   // source index N
            String line = match[2];            // line number
            if (index < (int)sourceMap.size())
                result += sourceMap[index] + "(" + line + ")";
            else
                result += match[0];            // unknown index, keep as-is
        }

        result += match.suffix();
    
        return result;
    }
}
