#include "Scene/WorldSubsystem.h"
#include "Core/Assert.h"
#include "Resources/ResourceSubsystem.h"
#include "Renderer/RenderSubsystem.h"
#include "Core/FileSubsystem.h"
#include "nlohmann/json.hpp"

#include "Scene/Component.h"
#include "Scene/TransformComponent.h"
#include "Renderer/CameraComponent.h"
#include "Renderer/CameraController.h"
#include "Renderer/MeshComponent.h"
#include "Serialization/Archive.h"
#include "Reflection/ReflectionSubsystem.h"

namespace axiom
{
    SceneLoader::SceneLoader(Scene& scene, ResourceSubsystem& resourceModule, RenderSubsystem& renderModule, FileSubsystem& fileSystemModule,  ReflectionSubsystem& reflectionSubsystem)
    : m_scene(scene)
    , m_resourceModule(resourceModule)
    , m_renderModule(renderModule)
    , m_fileSystemModule(fileSystemModule)
    , m_reflectionSubsystem(reflectionSubsystem)
    {
    }

    void SceneLoader::Load(const String& path)
    {
        Vector<UniquePtr<IResolvable>> handles;

        Path absolutePath = m_fileSystemModule.Resolve(path);
        std::ifstream fstream(absolutePath);
        nlohmann::json jsonData = nlohmann::json::parse(fstream);

        for (auto& entityJson : jsonData["entities"])
        {
            SharedPtr<Entity> entity = m_scene.CreateEntity();
            Archive ar(entityJson, handles);
            entity->Deserialize(ar);
            for (auto& componentJson : entityJson["components"])
            {
                String type = componentJson["type"].get<String>();
                UniquePtr<Component> component = m_reflectionSubsystem.GetTypeRegistry().Create(type);
                if(component == nullptr)
                {
                    Log::Error("Unknown reflection type: {}", type);
                    continue;
                }
                Archive ar(componentJson, handles);
                component->Deserialize(ar);
                entity->AddComponent(std::move(component));
            }
        }

        for (auto& handle : handles)
        {
            handle->Resolve(m_resourceModule);
        }
    }

    WorldSubsystem::WorldSubsystem(Application& application)
    : ApplicationSubsystem(application)
    {
        m_activeScene = MakeUnique<Scene>("Scene");
    }

    Scene& WorldSubsystem::GetActiveScene() const
    {
        AX_ASSERT(m_activeScene, "Scene cannot be null!");
        return *m_activeScene.get();
    }

    void WorldSubsystem::OnRegister()
    {
        m_activeScene->Register();
    }

    void WorldSubsystem::OnInitialize()
    {
        ResourceSubsystem& resources = GetSubsystem<ResourceSubsystem>();
        RenderSubsystem& render = GetSubsystem<RenderSubsystem>();
        FileSubsystem& fileSystemModule = GetSubsystem<FileSubsystem>();
        ReflectionSubsystem& reflection = GetSubsystem<ReflectionSubsystem>();

        m_sceneLoader = MakeUnique<SceneLoader>(*m_activeScene, resources, render, fileSystemModule, reflection);

        m_activeScene->Initialize();
    }

    void WorldSubsystem::OnShutdown()
    {
        m_activeScene->Shutdown();
    }

    void WorldSubsystem::OnUpdate(float deltaTime)
    {
        m_activeScene->Update(deltaTime);
    }

    void WorldSubsystem::OnBeginFrame()
    {
        m_activeScene->BeginFrame();
    }

    void WorldSubsystem::OnRender()
    {
        m_activeScene->Render();
    }

    void WorldSubsystem::OnEndFrame()
    {
        m_activeScene->EndFrame();
    }
}
