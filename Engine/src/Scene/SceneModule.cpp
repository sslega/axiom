#include "Scene/SceneModule.h"
#include "SceneModule.h"
#include "Core/Assert.h"
#include "Resources/ResourceModule.h"
#include "Renderer/RenderModule.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include "Core/FileSystemModule.h"
#include "nlohmann/json.hpp"

#include "Scene/Component.h"
#include "Scene/TransformComponent.h"
#include "Renderer/CameraComponent.h"
#include "Renderer/CameraController.h"
#include "Renderer/MeshComponent.h"
#include "Geometry/Quad.h"
#include "Geometry/Cube.h"
#include "Math/Math.h"
#include "Serialization/Archive.h"

namespace axiom
{
    SceneLoader::SceneLoader(Scene& scene, ResourceModule& resourceModule, RenderModule& renderModule, FileSystemModule& fileSystemModule)
    : m_scene(scene)
    , m_resourceModule(resourceModule)
    , m_renderModule(renderModule)
    , m_fileSystemModule(fileSystemModule)
    {
    }

    void SceneLoader::Load(const String &path)
    {
        Path absolutePath = m_fileSystemModule.Resolve(path);
        std::ifstream fstream(absolutePath);
        nlohmann::json jsonData = nlohmann::json::parse(fstream);
        int version = jsonData["version"].get<int>();
        for(auto& jsonMaterial : jsonData["materials"])
        {
            String materialID = jsonMaterial["id"].get<String>();
            String shaderPath = jsonMaterial["shader"].get<String>();
            SharedPtr<Shader> shader = m_renderModule.GetShader(shaderPath);
            SharedPtr<Material> material = MakeShared<Material>(shader);
            m_materials[materialID] = material;
            for (auto& [uniformName, uniformValue] : jsonMaterial["uniforms"].items())
            {
                if (uniformValue.is_number())
                {
                    float v = uniformValue.get<float>();
                    material->SetUniform(uniformName, v);
                }
                else if (uniformValue.is_array())
                {
                    size_t size = uniformValue.size();
                    if (size == 2)
                    {
                        Vec2 v = { uniformValue[0], uniformValue[1] };
                        material->SetUniform(uniformName, v);
                    }
                    else if (size == 3)
                    {
                        Vec3 v = { uniformValue[0], uniformValue[1], uniformValue[2] };
                        material->SetUniform(uniformName, v);
                    }
                    else if (size == 4)
                    {
                        Vec4 v = { uniformValue[0], uniformValue[1], uniformValue[2], uniformValue[3] };
                        material->SetUniform(uniformName, v);
                    }
                }
            }

        }
        
        Vector<UniquePtr<IResolvable>> handles;
        
        for (auto& entityJson : jsonData["entities"])
        {
            Entity& entity = m_scene.CreateEntity(entityJson["name"]);
            for (auto& componentJson : entityJson["components"])
            {
                String type = componentJson["type"].get<String>();
                UniquePtr<Component> component = ClassRegistry::Get().Create(type);
                Archive ar(componentJson, handles);
                component->Deserialize(ar);
                entity.AddComponent(std::move(component));
            }
        }

        for (auto& handle : handles)
        {
            handle->Resolve(m_resourceModule);
        }

        // Temporary hack to make asset referencing working
        for (auto* mc : m_scene.GetComponents<MeshComponent>())
        {
            auto it = m_materials.find(mc->m_materialID);
            if (it != m_materials.end())
                mc->SetMaterial(it->second);
        }
    }

    SceneModule::SceneModule(Application& application)
    : ApplicationModule(application)
    {
        m_activeScene = MakeUnique<Scene>("Scene");
    }

    Scene& SceneModule::GetActiveScene() const
    {
        AX_ASSERT(m_activeScene, "Scene cannot be null!");
        return *m_activeScene.get();
    }

    void SceneModule::OnRegister()
    {
        m_activeScene->Register();
    }

    void SceneModule::OnInitialize()
    {
        ResourceModule& resources = GetModule<ResourceModule>();
        RenderModule& render = GetModule<RenderModule>();
        FileSystemModule& fileSystemModule = GetModule<FileSystemModule>();
        
        m_sceneLoader = MakeUnique<SceneLoader>(*m_activeScene, resources, render, fileSystemModule);
        // m_sceneLoader->Register<TransformComponent>("TransformComponent");

        m_activeScene->Initialize();
    }

    void SceneModule::OnShutdown()
    {
        m_activeScene->Shutdown();
    }

    void SceneModule::OnUpdate(float deltaTime)
    {
        m_activeScene->Update(deltaTime);
    }

    void SceneModule::OnBeginFrame()
    {
        m_activeScene->BeginFrame();
    }

    void SceneModule::OnRender()
    {
        m_activeScene->Render();
    }

    void SceneModule::OnEndFrame()
    {
        m_activeScene->EndFrame();
    }
}
