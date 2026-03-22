#include "EntryPoint.h"
#include "Core/Application.h"
#include "Core/FileSystemModule.h"
#include "Core/Types.h"
#include "Rendering/GLShader.h"
#include "Resources/ResourceModule.h"
#include "Scene/Entity.h"
#include "Scene/MeshComponent.h"

#include "Sandbox.h"
#include "TriangleMesh.h"
#include "Scene/Scene.h"

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
    // ResourceModule* resourceModule = GetModule<ResourceModule>();
    // SharedPtr<GLShader> shader = resourceModule->Load<GLShader>("Engine://Shaders/VertexColor.glsl");

    SharedPtr<TriangleMesh> triangle = MakeShared<TriangleMesh>();

    UniquePtr<Scene> scene = MakeUnique<Scene>();
    Entity* entity = scene->CreateEntity();
    MeshComponent* meshComponent = entity->CreateComponent<MeshComponent>();
    meshComponent->SetMesh(triangle);
    

    // TriangleMesh triangle = TriangleMesh();
    // Entity testEntity = Entity();
    // MeshComponent& meshComponent = testEntity.CreateComponent<MeshComponent>();
}


