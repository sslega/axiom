#pragma once
#include "Core/ApplicationSubsystem.h"
#include "Core/Types.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "nlohmann/json.hpp"

namespace axiom
{
    class FileSubsystem;
    class ResourceSubsystem;
    class ReflectionSubsystem;

    /// Reads a scene JSON file into a Scene. Usually reached through WorldSubsystem::LoadScene().
    ///
    /// Load() takes a virtual path and replaces the scene's contents. If the file can't be read or
    /// isn't a scene, it logs an error, returns false and leaves the scene alone.
    ///
    /// File shape: `{ "entities": [ { "id": "...", "name": "...", "components": [ { "type": "MeshComponent", "id": "...", ...fields } ] } ] }`
    /// A missing `id` gets a new one, so hand-written scenes can leave it out.
    class SceneLoader
    {
    public:
        SceneLoader(Scene& scene, ResourceSubsystem& resourceModule, FileSubsystem& fileSystemModule, ReflectionSubsystem& reflectionSubsystem);

        bool Load(const String& path);

    private:
        Scene& m_scene;
        ResourceSubsystem& m_resourceSubsystem;
        FileSubsystem& m_fileSubsystem;
        ReflectionSubsystem& m_reflectionSubsystem;
    };

    /// Writes a Scene to a JSON file in the shape SceneLoader reads. Usually reached through WorldSubsystem::SaveScene().
    ///
    /// Entities are sorted by ID and components by type only so saving is repeatable; loading doesn't depend on order.
    /// A component type that isn't reflected can't be saved; it's logged and skipped.
    class SceneWriter
    {
    public:
        SceneWriter(Scene& scene, ResourceSubsystem& resourceSubsystem, FileSubsystem& fileSystemModule, ReflectionSubsystem& reflectionSubsystem);

        bool Save(const String& path);

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
    /// world.SaveScene(world.GetActiveScenePath());
    /// @endcode
    ///
    /// NewScene() and a successful LoadScene() destroy the old entities, so anything holding an Entity* must drop it.
    class WorldSubsystem : public ApplicationSubsystem
    {
    public:
        WorldSubsystem(SubsystemHost& host);

        Scene& GetActiveScene() const;

        void NewScene();
        bool LoadScene(const String& path);
        bool SaveScene(const String& path);

        /// Virtual path of the file the active scene came from or was last saved to. Empty if it has none.
        const String& GetActiveScenePath() const { return m_activeScenePath; }

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
        UniquePtr<SceneWriter> m_sceneWriter;
        String m_activeScenePath;
    };
}
