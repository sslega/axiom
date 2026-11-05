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
    class ReflectionSubsystem;

    /// Reads a scene JSON file into a Scene. Usually reached through WorldSubsystem::LoadScene().
    ///
    /// Load() takes a virtual path, and adds to whatever the scene already holds rather than replacing it.
    ///
    /// File shape: `{ "entities": [ { "name": "...", "components": [ { "type": "MeshComponent", ...fields } ] } ] }`
    class SceneLoader
    {
    public:
        SceneLoader(Scene& scene, ResourceSubsystem& resourceModule, RenderSubsystem& renderModule, FileSubsystem& fileSystemModule, ReflectionSubsystem& reflectionSubsystem);

        void Load(const String& path);
    
    private:
        Scene& m_scene;
        ResourceSubsystem& m_resourceSubsystem;
        RenderSubsystem& m_renderSubsystem;
        FileSubsystem& m_fileSubsystem;
        ReflectionSubsystem& m_reflectionSubsystem;
    };

    class SceneWriter
    {
    public:
        SceneWriter(Scene& scene, ResourceSubsystem& resourceSubsystem, FileSubsystem& fileSystemModule, ReflectionSubsystem& reflectionSubsystem);

        void Save(const String& path);
    
    private:
        Scene& m_scene;
        ResourceSubsystem& m_resourceSubsystem;
        FileSubsystem& m_fileSubsystem;
        ReflectionSubsystem& m_reflectionSubsystem;
    };

    /// Owns the active Scene and drives its frame hooks.
    ///
    /// @code{.cpp}
    /// Scene& scene = world.GetActiveScene();
    /// world.LoadScene("project://Scenes/main.scene.json");
    /// @endcode
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
