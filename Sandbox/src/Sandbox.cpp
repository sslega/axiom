#include "EntryPoint.h"
#include "Core/Application.h"
#include "Core/FileSystemModule.h"
#include "Core/Types.h"
// #include "Rendering/MeshComponent.h"
// #include "Rendering/Material.h"
// #include "Resources/ResourceModule.h"
// #include "Scene/SceneModule.h"
// #include "Scene/Entity.h"

#include "Sandbox.h"
// #include "TriangleMesh.h"
// #include "Scene/Scene.h"

#include "Renderer/Buffer.h"

using namespace axiom;


UniquePtr<Application> CreateApplication()
{
    AppConfig appConfig;
    appConfig.renderAPI = axiom::RenderAPI::OpenGL;
    
    AppWindowConfig windowConfig;
    windowConfig.width = 800;
    windowConfig.height = 600;
    windowConfig.title = "Yet Another Game Engine";
    windowConfig.vsync = true;
    windowConfig.fullscreen = false;
    windowConfig.backend = ApplicationWindowBackend::GLFW;

    appConfig.windowConfig = windowConfig;
    
    return MakeUnique<Sandbox>(appConfig);
};

Sandbox::Sandbox(AppConfig appConfig)
: Application(appConfig)
{
}

void Sandbox::OnRegisterModules()
{
    FileSystemModule* fileSystemModule = GetModule<FileSystemModule>();
    fileSystemModule->Mount("Engine", AX_ENGINE_DIR);
    fileSystemModule->Mount("Project", AX_PROJECT_DIR);
}

void Sandbox::OnApplicationRun()
{

    // SceneModule* sceneModule = GetModule<SceneModule>();
    // ResourceModule* resourceModule = GetModule<ResourceModule>();
    
    // SharedPtr<ShaderResource> shaderResource = resourceModule->Load<ShaderResource>("Engine://Shaders/VertexColor.glsl");
    // SharedPtr<Material> material = MakeShared<Material>();
    // material->shader = shaderResource;

    // SharedPtr<TriangleMesh> triangle = MakeShared<TriangleMesh>();

    // Scene* scene = sceneModule->CreateScene("SandboxScene");
    // Entity* entity = scene->CreateEntity();
    // MeshComponent* meshComponent = entity->CreateComponent<MeshComponent>();
    // meshComponent->SetMesh(triangle);
    // meshComponent->SetMaterial(material);
    

    // TriangleMesh triangle = TriangleMesh();
    // Entity testEntity = Entity();
    // MeshComponent& meshComponent = testEntity.CreateComponent<MeshComponent>();
}


