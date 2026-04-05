#include "Core/Application.h"
#include "Platform/ApplicationWindow.h"
#include "Rendering/RenderModule.h"
#include "Resources/ResourceModule.h"
#include "Resources/GLShaderLoader.h"
#include "Core/FileSystemModule.h"
#include "Scene/SceneModule.h"

namespace axiom
{

    Application::Application(AppConfig appConfig)
    : m_appConfig(appConfig)
    {
        m_appConfig = appConfig;
        // TODO: Make proper initialization based on config.backend
        m_applicationWindow = MakeUnique<ApplicationWindow>(m_appConfig.windowConfig);
    }

    int Application::Run()
    {
        printf("Starting Axiom Application...\n");
        
        RegisterModules();
        OnRegisterModules();
        InitializeModules();
        OnInitializeModules();

        OnApplicationRun();
        
        while (!m_applicationWindow->ShouldClose())
        {
            PoolEvents();
            Update();
            Render(); 
        }

        m_applicationWindow->CloseWindow();

        return 0;    
    }

    void Application::OnApplicationRun()
    {
    }

    void Application::PoolEvents()
    {
        m_applicationWindow->PoolEvents();
    }

    void Application::Update()
    {
        m_applicationWindow->Update();

        for (auto& [type, module] : m_engineModules)
        {
            module->Update();
        }
        
    }

    void Application::Render()
    {
        m_applicationWindow->Render();

        for (auto& [type, module] : m_engineModules)
        {
            module->Render();
        }

        OnRender();
    }

    void Application::OnRender()
    {
    }

    void Application::RegisterModules()
    {   
        RegisterModule<FileSystemModule>();
        RegisterModule<RenderModule>();
        // RegisterModule<SceneModule>();

        ResourceModule* resourceModule = RegisterModule<ResourceModule>();
        resourceModule->RegisterLoader(".glsl", MakeUnique<GLShaderLoader>());
    }

    void Application::OnRegisterModules()
    {
    }

    void Application::InitializeModules()
    {
        printf("Initializing engine modules...\n");
        for (auto& [type, module] : m_engineModules)
        {
            printf("Initializing module: %s\n", type.name());
            module->Initialize();
        }
    }

    void Application::OnInitializeModules()
    {
    }

    const GraphicsDevice::API Application::GetRenderAPI() const
    {
        return m_appConfig.renderAPI;
    }

    const ApplicationWindow& Application::GetApplicationWindow() const
    {
        return *m_applicationWindow;
    }
}