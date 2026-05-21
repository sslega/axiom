#pragma once
#include "Core/Application.h"
#include "Core/ApplicationSubsystem.h"
#include "Core/Types.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "nlohmann/json.hpp"

namespace axiom
{
    class ResourceSubsystem;
    class RenderSubsystem;

    // using ComponentFactory = std::function<void(Entity&, const nlohmann::json&)>;

    class SceneLoader
    {
    public:
        SceneLoader(Scene& scene, ResourceSubsystem& resourceModule, RenderSubsystem& renderModule, FileSubsystem& fileSystemModule);

        void Load(const String& path);

        // template<typename T>
        // void Register(const String& key)
        // {
        //     m_factories[key] = [](Entity& e, const nlohmann::json& j) {
        //         e.CreateComponent<T>().Load(j);
        //     };
        // }

    private:
        Scene& m_scene;
        ResourceSubsystem& m_resourceModule;
        RenderSubsystem& m_renderModule;
        FileSubsystem& m_fileSystemModule;
        // StringMap<ComponentFactory> m_factories;
    };

    class WorldSubsystem : public ApplicationSubsystem
    {
    public:
        WorldSubsystem(Application& application);

        Scene& GetActiveScene() const;
        inline void LoadScene(const String& path) { m_sceneLoader->Load(path); }

    protected:
        virtual void OnRegister()  override;
        virtual void OnInitialize() override;
        virtual void OnShutdown()  override;
        virtual void OnUpdate(float deltaTime) override;
        virtual void OnBeginFrame() override;
        virtual void OnRender() override;
        virtual void OnEndFrame() override;

    private:
        UniquePtr<Scene>  m_activeScene;
        UniquePtr<SceneLoader> m_sceneLoader;
    };
}
