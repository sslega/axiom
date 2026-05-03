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
            String name = jsonMaterial["name"].get<String>();
            String shaderPath = jsonMaterial["shader"].get<String>();
            SharedPtr<Shader> shader = m_renderModule.GetShader(shaderPath);
            SharedPtr<Material> material = MakeShared<Material>(shader);
            m_materials[name] = material;
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
        for (auto& entityJson : jsonData["entities"])
        {
            Entity& entity = m_scene.CreateEntity(entityJson["name"]);
            for (auto& componentJson : entityJson["components"])
            {
                String type = componentJson["type"].get<std::string>();
                // auto it = m_factories.find(type);
                // AX_ASSERT(it != m_factories.end(), "Class factory not found for loaded scene!");
                // it->second(entity, componentJson);
                if(type == "transform")
                {
                    TransformComponent& component = entity.CreateComponent<TransformComponent>();
                    if (componentJson.contains("position"))
                    {
                        auto& p = componentJson["position"];
                        Vec3 position = { p[0].get<float>(), p[1].get<float>(), p[2].get<float>() };
                        component.position = position;
                    }

                    if (componentJson.contains("scale"))
                    {
                        auto& p = componentJson["scale"];
                        Vec3 scale = { p[0].get<float>(), p[1].get<float>(), p[2].get<float>() };
                        component.scale = scale;
                    }

                    if (componentJson.contains("rotation"))
                    {
                        auto& p = componentJson["rotation"];
                        Vec3 rotation = { p[0].get<float>(), p[1].get<float>(), p[2].get<float>() };
                        rotation.x = ToRadians(rotation.x);
                        rotation.y = ToRadians(rotation.y);
                        rotation.z = ToRadians(rotation.z);
                        component.rotation = rotation;
                    }
                }
                if(type == "camera")
                {
                    float near = componentJson["near"].get<float>();
                    float far = componentJson["far"].get<float>();
                    float fov = componentJson["fov"].get<float>();
                    float aspectRatio = 800.0f / 600.0f; // REMOVE THIS
                    CameraComponent& component = entity.CreateComponent<CameraComponent>(ToRadians(fov), aspectRatio, near, far);
                    // if (componentJson.contains("projection"))
                    // {
                    //     String projectionType = componentJson["projection"].get<String>();
                    //     component.m_camera.SetProjectionType(projectionType == "perspective" ? Camera::ProjectionType::Perspective : Camera::ProjectionType::Orthographic);
                    // }
                    // if (componentJson.contains("fov"))
                    // {
                    //     component.m_camera.SetFoV(componentJson["fov"].get<float>());
                    // }
                    // if (componentJson.contains("near"))
                    // {
                    //     component.m_camera.SetFoV(componentJson["near"].get<float>());
                    // }
                    // if (componentJson.contains("far"))
                    // {
                    //     component.m_camera.SetFoV(componentJson["far"].get<float>());
                    // }
                }
                if(type == "camera_controller")
                {
                    CameraController& component = entity.CreateComponent<CameraController>();
                    if (componentJson.contains("move_speed"))
                    {
                        component.moveSpeed = componentJson["move_speed"].get<float>();
                    }
                    if (componentJson.contains("look_sensitivity"))
                    {
                        component.lookSensitivity = componentJson["look_sensitivity"].get<float>();
                    }
                }
                if(type == "mesh")
                {
                    MeshComponent& component = entity.CreateComponent<MeshComponent>();
                    if (componentJson.contains("path"))
                    {
                        String path = componentJson["path"].get<String>();
                        SharedPtr<MeshResource> mesh;
                        if(path == "builtin://Quad") mesh = MakeShared<Quad>();
                        else if(path == "builtin://Cube") mesh = MakeShared<Cube>();
                        else mesh = m_resourceModule.Load<MeshResource>(path);
                        component.SetMesh(mesh);
                    }
                    if (componentJson.contains("material"))
                    {
                        String materialName = componentJson["material"].get<String>();  
                        component.SetMaterial(m_materials[materialName]);
                    }
                }
            }
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
