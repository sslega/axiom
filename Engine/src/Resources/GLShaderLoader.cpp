#include "Resources/GLShaderLoader.h"
// #include "Rendering/GL/GLRenderShader.h"
// #include "Platform/OpenGL/OpenGLShader.h"

namespace axiom
{
    SharedPtr<void> GLShaderLoader::CreateResource(const FileData &fileData)
    {
        return nullptr;

        // String shaderSource(fileData.buffer.begin(), fileData.buffer.end());

        // String vertexShader;
        // String fragmentShader;

        // ParseSource(shaderSource, vertexShader, fragmentShader);
        // // TODO: this probably should return lightweight "asset" instead of actual shader
        // // Changed to OpenGLShader to make it work during compilation
        // return MakeShared<OpenGLShader>(vertexShader, fragmentShader);
    }

    void GLShaderLoader::ParseSource(const String &source, String &vertexSource, String &fragmentSource) const
    {
        // const String typeToken = "#type";
        // size_t pos = 0;

        // for(int i = 0; i < 3; ++i)
        // {
        //     size_t typePos = source.find(typeToken, pos);
        //     if (typePos == std::string::npos)
        //     {
        //         break;
        //     }

        //     size_t eol = source.find_first_of("\r\n", typePos);
        //     if (eol == std::string::npos)
        //     {
        //         throw std::runtime_error("Syntax error: missing end of line after #type");
        //     }

        //     size_t begin = typePos + typeToken.size() + 1; // +1 for space
        //     String stageStr = source.substr(begin, eol - begin);


        //     size_t nextType = source.find(typeToken, eol);
        //     String stageSource = source.substr(eol + 1, nextType - (eol + 1));

        //     if(stageStr == "vertex")
        //     {
        //         vertexSource = std::move(stageSource);
        //     }
        //     else if(stageStr == "fragment")
        //     {
        //         fragmentSource = std::move(stageSource);
        //     }
        //     else
        //     {
        //         throw std::runtime_error("Unknown shader stage: " + String(stageStr));
        //     }

        //     pos = nextType;
        // }
    }
}
