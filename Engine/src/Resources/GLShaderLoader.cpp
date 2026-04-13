#include "Resources/GLShaderLoader.h"
#include "Resources/ShaderResource.h"
#include "Core/Types.h"
#include "Core/Assert.h"

namespace axiom
{
    SharedPtr<void> GLShaderLoader::CreateResource(const FileData &fileData)
    {

        String source(fileData.buffer.begin(), fileData.buffer.end());
        String vert, frag;
        ParseSource(source, vert, frag);
        return MakeShared<ShaderResource>(vert, frag);
    }

    void GLShaderLoader::ParseSource(const String &source, String &vertexSource, String &fragmentSource) const
    {
        const String typeToken = "#type";
        size_t pos = 0;

        for(int i = 0; i < 3; ++i)
        {
            size_t typePos = source.find(typeToken, pos);
            if (typePos == String::npos)
            {
                break;
            }

            size_t eol = source.find_first_of("\r\n", typePos);
            if (eol == String::npos)
            {
                AX_ASSERT(false, "Syntax error: missing end of line after #type");
            }

            size_t begin = typePos + typeToken.size() + 1; // +1 for space
            String stageStr = source.substr(begin, eol - begin);


            size_t nextType = source.find(typeToken, eol);
            String stageSource = source.substr(eol + 1, nextType - (eol + 1));

            if(stageStr == "vertex")
            {
                vertexSource = std::move(stageSource);
            }
            else if(stageStr == "fragment")
            {
                fragmentSource = std::move(stageSource);
            }
            else
            {
                AX_ASSERT(false, "Unknown shader stage: " + String(stageStr));
            }

            pos = nextType;
        }
    }
}
