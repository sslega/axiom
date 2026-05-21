#include "Resources/MaterialLoader.h"
#include "Resources/MaterialResource.h"
#include "Renderer/RenderSubsystem.h"
#include "Renderer/Shader.h"
#include "nlohmann/json.hpp"
#include "Math/Vector.h"

namespace axiom
{
    MaterialLoader::MaterialLoader(const ResourceSubsystem& resourceModule, RenderSubsystem& renderModule)
    : ResourceLoader(resourceModule)
    , m_renderModule(renderModule)
    {
    }

    SharedPtr<Resource> MaterialLoader::CreateResource(const FileData& fileData)
    {
        String content(fileData.buffer.begin(), fileData.buffer.end());
        nlohmann::json json = nlohmann::json::parse(content);

        String shaderPath = json["shader"].get<String>();
        SharedPtr<Shader> shader = m_renderModule.GetShader(shaderPath);
        SharedPtr<MaterialResource> material = MakeShared<MaterialResource>(shader);

        if (json.contains("uniforms"))
        {
            for (auto& [name, value] : json["uniforms"].items())
            {
                if (value.is_number())
                {
                    material->SetUniform(name, value.get<float>());
                }
                else if (value.is_array())
                {
                    size_t size = value.size();
                    if (size == 2) material->SetUniform(name, Vec2{ value[0], value[1] });
                    else if (size == 3) material->SetUniform(name, Vec3{ value[0], value[1], value[2] });
                    else if (size == 4) material->SetUniform(name, Vec4{ value[0], value[1], value[2], value[3] });
                }
            }
        }

        return material;
    }
}
