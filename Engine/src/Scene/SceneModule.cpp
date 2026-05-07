#include "Scene/SceneModule.h"
#include "SceneModule.h"
#include "Core/Assert.h"
#include "Resources/ResourceModule.h"
#include "Renderer/RenderModule.h"
#include "Core/FileSystemModule.h"
#include "nlohmann/json.hpp"

#include "Scene/Component.h"
#include "Scene/TransformComponent.h"
#include "Renderer/CameraComponent.h"
#include "Renderer/CameraController.h"
#include "Renderer/MeshComponent.h"
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
                UniquePtr<Component> component = ClassRegistry::Get().Create(type);
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
