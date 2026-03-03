#include "Resources/Shader.h"

namespace axiom
{


    const String &Shader::GetVertexShader()
    {
        return m_VertexShader;
    }

    const String &Shader::GetFragmentShader()
    {
        return m_FragmentShader;
    }

    void Shader::ParseData()
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
            String* output;
            if(stageStr == "vertex")
            {
                output = &m_VertexShader;
            }
            else if(stageStr == "fragment")
            {
                output = &m_FragmentShader;
            }
            else
            {
                throw std::runtime_error("Unknown shader stage: " + stageStr);
            }

            size_t nextType = shaderSource.find(typeToken, eol);
            String stageSource = shaderSource.substr(eol + 1, nextType - (eol + 1));
            *output = stageSource;

            pos = nextType;
        }
    }
}

