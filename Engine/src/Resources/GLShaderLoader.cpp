#include "Resources/GLShaderLoader.h"
#include "Rendering/GLShader.h"

namespace axiom
{
    SharedPtr<void> GLShaderLoader::CreateResource(const FileData &fileData)
    {
        StringView shaderSource = String(fileData.buffer.begin(), fileData.buffer.end());

        String vertexShader;
        String fragmentShader;

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
            StringView stageStr = shaderSource.substr(begin, eol - begin);


            size_t nextType = shaderSource.find(typeToken, eol);
            StringView stageSource = shaderSource.substr(eol + 1, nextType - (eol + 1));

            if(stageStr == "vertex")
            {
                vertexShader = std::move(stageSource);
            }
            else if(stageStr == "fragment")
            {
                fragmentShader = std::move(stageSource);
            }
            else
            {
                throw std::runtime_error("Unknown shader stage: " + String(stageStr));
            }

            pos = nextType;
        }

        return MakeShared<GLShader>(vertexShader, fragmentShader);
    }
}