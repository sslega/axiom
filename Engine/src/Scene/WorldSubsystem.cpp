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
#include "Reflection/Reflection.h"

#include <algorithm>
#include <fstream>

namespace axiom
{
    SceneLoader::SceneLoader(Scene& scene, ResourceSubsystem& resourceSubsystem, RenderSubsystem& renderSubsystem, FileSubsystem& fileSystemModule,  ReflectionSubsystem& reflectionSubsystem)
    : m_scene(scene)
    , m_resourceSubsystem(resourceSubsystem)
    , m_renderSubsystem(renderSubsystem)
    , m_fileSubsystem(fileSystemModule)
    , m_reflectionSubsystem(reflectionSubsystem)
    {
    }

    bool SceneLoader::Load(const String& path)
    {
        Path absolutePath = m_fileSubsystem.Resolve(path);
        std::ifstream fstream(absolutePath);
        if(!fstream)
        {
            Log::Error("Could not open scene file: {}", path);
            return false;
        }

        // Parse before touching the scene, so a bad file leaves the current scene intact.
        nlohmann::json jsonData = nlohmann::json::parse(fstream, nullptr, false);
        if(jsonData.is_discarded() || !jsonData.contains("entities"))
        {
            Log::Error("Not a valid scene file: {}", path);
            return false;
        }

        m_scene.Clear();

        for (auto& entityJson : jsonData["entities"])
        {
            SharedPtr<Entity> entity = MakeShared<Entity>();
            Archive ar(entityJson, m_resourceSubsystem);
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
                Archive ar(componentJson, m_resourceSubsystem);
                component->Deserialize(ar);
                entity->AddComponent(std::move(component));
            }

            // Added last: the scene files the entity under its saved ID, and joining initializes all its components.
            m_scene.AddEntity(entity);
        }

        return true;
    }

    SceneWriter::SceneWriter(Scene &scene, ResourceSubsystem& resourceSubsystem, FileSubsystem &fileSystemModule, ReflectionSubsystem &reflectionSubsystem)
    : m_scene(scene)
    , m_resourceSubsystem(resourceSubsystem)
    , m_fileSubsystem(fileSystemModule)
    , m_reflectionSubsystem(reflectionSubsystem)
    {
    }

    bool SceneWriter::Save(const String &path)
    {
        struct TypedComponent
        {
            String type;
            Component* component;
        };

        Vector<Entity*> entities = m_scene.GetEntities();
        std::sort(entities.begin(), entities.end(), [](const Entity* a, const Entity* b) { return a->GetID() < b->GetID(); });

        nlohmann::json entitiesJson = nlohmann::json::array();
        for(Entity* entity : entities)
        {
            nlohmann::json entityJson;
            Archive entityArchive(entityJson, m_resourceSubsystem);
            entity->Serialize(entityArchive);

            Vector<TypedComponent> components;
            for(Component* component : entity->GetComponents())
            {
                const TypeDescriptor* descriptor = m_reflectionSubsystem.GetTypeRegistry().Find(typeid(*component));
                if(descriptor == nullptr)
                {
                    Log::Error("Not saved, type isn't reflected: {} on entity {}", typeid(*component).name(), entity->GetName());
                    continue;
                }
                components.push_back({ descriptor->name, component });
            }

            std::sort(components.begin(), components.end(), [](const TypedComponent& a, const TypedComponent& b) { return a.type < b.type; });

            nlohmann::json componentsJson = nlohmann::json::array();
            for(const TypedComponent& typed : components)
            {
                nlohmann::json componentJson;
                componentJson["type"] = typed.type;
                Archive componentArchive(componentJson, m_resourceSubsystem);
                typed.component->Serialize(componentArchive);
                componentsJson.push_back(std::move(componentJson));
            }

            entityJson["components"] = std::move(componentsJson);
            entitiesJson.push_back(std::move(entityJson));
        }

        nlohmann::json root;
        root["version"] = 1;
        root["entities"] = std::move(entitiesJson);

        // Build the text before opening the file, which truncates it.
        const String text = root.dump(2);

        std::ofstream file(m_fileSubsystem.Resolve(path));
        file << text << '\n';
        if(!file)
        {
            Log::Error("Could not write scene file: {}", path);
            return false;
        }

        return true;
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

    void WorldSubsystem::NewScene()
    {
        m_activeScene->Clear();
        m_activeScenePath.clear();
    }

    bool WorldSubsystem::LoadScene(const String& path)
    {
        if(!m_sceneLoader->Load(path))
        {
            return false;
        }
        m_activeScenePath = path;
        return true;
    }

    bool WorldSubsystem::SaveScene(const String& path)
    {
        if(!m_sceneWriter->Save(path))
        {
            return false;
        }
        m_activeScenePath = path;
        return true;
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
        m_sceneWriter = MakeUnique<SceneWriter>(*m_activeScene, resources, fileSystemModule, reflection);

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
