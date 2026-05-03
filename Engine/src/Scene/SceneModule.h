#pragma once
#include "Core/Application.h"
#include "Core/ApplicationModule.h"
#include "Core/Types.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "nlohmann/json.hpp"

namespace axiom
{
    class ResourceModule;
    class RenderModule;
    class Material;

    // using ComponentFactory = std::function<void(Entity&, const nlohmann::json&)>;

    class SceneLoader
    {
    public:
        SceneLoader(Scene& scene, ResourceModule& resourceModule, RenderModule& renderModule, FileSystemModule& fileSystemModule);

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
        ResourceModule& m_resourceModule;
        RenderModule& m_renderModule;
        FileSystemModule& m_fileSystemModule;
        // StringMap<ComponentFactory> m_factories;
        StringMap<SharedPtr<Material>> m_materials;
    };

    class SceneModule: public ApplicationModule
    {
    public:
        SceneModule(Application& application);

        Scene& GetActiveScene() const;
        inline void LoadScene(const String& path) { m_sceneLoader->Load(path); }

        // template<typename T>
        // void RegisterComponent(const String& key)
        // {
        //     m_sceneLoader->Register<T>(key);
        // }
    
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