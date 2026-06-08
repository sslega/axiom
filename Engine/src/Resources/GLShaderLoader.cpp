#include "Resources/GLShaderLoader.h"
#include "Resources/ShaderResource.h"
#include "Resources/ResourceSubsystem.h"
#include "Resources/Resource.h"
#include "Core/Types.h"
#include "Core/Assert.h"
#include <regex>
#include "GLShaderLoader.h"
namespace axiom
{
    GLShaderLoader::GLShaderLoader(const ResourceSubsystem& resourceModule)
    : ResourceLoader(resourceModule)
    {
    }

    SharedPtr<Resource> GLShaderLoader::CreateResource(const FileData &fileData)
    {

        String source(fileData.buffer.begin(), fileData.buffer.end());
        
        Vector<String> sourceMap;
        sourceMap.push_back(fileData.path.filename().string());
        int nextSourceIndex = 1;
        int currentSourceIndex = 0;
        ResolveIncludes(source, sourceMap, nextSourceIndex, currentSourceIndex);
        
        String vert, frag;
        ParseSource(source, vert, frag);
        return MakeShared<ShaderResource>(vert, frag, sourceMap);
    }

    void GLShaderLoader::ParseSource(const String &source, String &vertexSource, String& fragmentSource) const
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

    void GLShaderLoader::ResolveIncludes(String& source, Vector<String>& sourceMap, int& nextSourceIndex, int currentSourceIndex)
    {
        String result;
        std::istringstream stream(source);
        String line;
        uint32 lineNumber = 1;
        while (std::getline(stream, line))
        {
            if (line.starts_with("#include"))
            {
                // extract "filename.glsl" from the line
                size_t start = line.find('"') + 1;
                size_t end   = line.rfind('"');
                // TODO: support project shader dir as well
                String filename = line.substr(start, end - start);
                String includeVirtualPath = "engine://Shaders/Include/" + filename;

                Path physicalPath = m_resourceModule.Resolve(includeVirtualPath);

                uint32 includeIndex = nextSourceIndex++;
                result += "#line 1 " + std::to_string(includeIndex) + "\n";
                sourceMap.push_back(filename);
                
                FileData data = ReadFile(physicalPath);
                String includeSource(data.buffer.begin(), data.buffer.end());
                ResolveIncludes(includeSource, sourceMap, nextSourceIndex, includeIndex);  // recurse for nested includes
                result += includeSource + '\n';  // replace the #include line with file contents;
                result += "#line " +  std::to_string(lineNumber + 1) + " " +  std::to_string(currentSourceIndex) + "\n";
            }
            else
            {
                result += line + '\n';  // keep the line as-is
            }
            lineNumber++;
        }
        source = result;
    }
}