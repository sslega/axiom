#include "Resources/GLShaderLoader.h"
#include "Resources/ShaderResource.h"
#include "Resources/ResourceModule.h"
#include "Core/Types.h"
#include "Core/Assert.h"

namespace axiom
{
    GLShaderLoader::GLShaderLoader(const ResourceModule& resourceModule)
    : ResourceLoader(resourceModule)
    {
    }

    SharedPtr<void> GLShaderLoader::CreateResource(const FileData &fileData)
    {

        String source(fileData.buffer.begin(), fileData.buffer.end());
        ResolveIncludes(source);
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

    void GLShaderLoader::ResolveIncludes(String& source)
    {
        String result;
        std::istringstream stream(source);
        String line;
        while (std::getline(stream, line))
        {
            if (line.starts_with("#include"))
            {
                // extract "filename.glsl" from the line
                size_t start = line.find('"') + 1;
                size_t end   = line.rfind('"');
                // TODO: support project shader dir as well
                String includeVirtualPath = "engine://Shaders/Include/" + line.substr(start, end - start);

                Path physicalPath = m_resourceModule.Resolve(includeVirtualPath);
                
                FileData data = ReadFile(physicalPath);
                String includeSource(data.buffer.begin(), data.buffer.end());
                ResolveIncludes(includeSource);  // recurse for nested includes
                result += includeSource + '\n';  // replace the #include line with file contents;
            }
            else
            {
                result += line + '\n';  // keep the line as-is
            }
        }
        source = result;
    }
}

