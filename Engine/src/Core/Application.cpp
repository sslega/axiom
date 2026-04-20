#include "Core/Application.h"
#include "Platform/ApplicationWindow.h"
#include "Renderer/RenderModule.h"
#include "Resources/ResourceModule.h"
#include "Resources/GLShaderLoader.h"
#include "Resources/Texture2DLoader.h"
#include "Core/FileSystemModule.h"
#include "Scene/SceneModule.h"
#include "ImGui/ImGuiModule.h"
#include "Input/Input.h"
#include "Core/Log.h"
#include <imgui.h>
#include "Application.h"

namespace axiom
{

    Application* Application::s_instance = nullptr;

    Application::Application(AppConfig appConfig)
    : m_appConfig(appConfig)
    {
        if(!s_instance)
        {
            s_instance = this;
        }
        else
        {
            AX_ASSERT(false, "Application instance already exists!");
        }

        // forces each printf to flush immediately.
        // TODO: Remove after adding proper logging
        setvbuf(stdout, NULL, _IONBF, 0);
        m_appConfig = appConfig;
        m_applicationWindow = ApplicationWindow::Create(m_appConfig.windowConfig);

        m_input = Input::Create(*m_applicationWindow);
        m_log = MakeUnique<ConsoleLog>();
        m_lastRenderTime = std::chrono::steady_clock::now();
        m_lastUpdateTime = m_lastRenderTime;
    }

    Application::~Application()
    {
        s_instance = nullptr;
    }

    int Application::Run()
    {
        Log::Info("Starting Axiom Application...");

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

        ShutdownModules();
        m_applicationWindow->CloseWindow();

        return 0;
    }

    void Application::PoolEvents()
    {
        m_applicationWindow->PollEvents();
    }

    void Application::Update()
    {
        auto now = std::chrono::steady_clock::now();
        Timestep ts = std::chrono::duration<float>(now - m_lastUpdateTime).count();
        float deltaTime = ts.Milliseconds();
        m_lastUpdateTime = now;

        m_applicationWindow->Update();

        for (auto& id : m_moduleOrder)
        {
            m_engineModules[id]->Update(deltaTime);
        }
        OnUpdate(deltaTime);
    }

    void Application::Render()
    {
        auto now = std::chrono::steady_clock::now();
        m_dt = std::chrono::duration<float>(now - m_lastRenderTime).count();
        m_lastRenderTime = now;

        for (auto& id : m_moduleOrder)
        {
            m_engineModules[id]->BeginFrame();
        }
            

        for (auto& id : m_moduleOrder)
        {
            m_engineModules[id]->Render();
        }

        OnRender();
        DebugRender();

        for (auto& id : m_moduleOrder)
        {
            m_engineModules[id]->EndFrame();
        }

        GetModule<RenderModule>()->GetGraphicsDevice().Present();
    }

    void Application::DebugRender()
    {
        uint8 fps = 1.0f / m_dt;
        auto Render = GetModule<RenderModule>();

        ImDrawList* dl = ImGui::GetForegroundDrawList();
        dl->AddText(ImVec2(10, 10), IM_COL32(255, 255, 255, 255), std::string("FPS: " + std::to_string(fps)).c_str());
        dl->AddText(ImVec2(10, 22), IM_COL32(255, 255, 255, 255), std::string("Draw Calls: " + std::to_string(Render->GetDrawCallCount())).c_str());
        dl->AddText(ImVec2(10, 34), IM_COL32(255, 255, 255, 255), std::string("Instanced calls: " + std::to_string(Render->GetInstanceCallCount())).c_str());
        dl->AddText(ImVec2(10, 46), IM_COL32(255, 255, 255, 255), std::string("Instanced objects drawn: " + std::to_string(Render->GetInstanceObjectCount())).c_str());
        dl->AddText(ImVec2(10, 58), IM_COL32(255, 255, 255, 255), std::string("Batched calls: " + std::to_string(Render->GetBatchCallCount())).c_str());
        dl->AddText(ImVec2(10, 70), IM_COL32(255, 255, 255, 255), std::string("Batched objects drawn: " + std::to_string(Render->GetBatchObjectCount())).c_str());
    }

    void Application::RegisterModules()
    {
        RegisterModule<FileSystemModule>();
        
        ResourceModule* resourceModule = RegisterModule<ResourceModule>();
        resourceModule->RegisterLoader<GLShaderLoader>(".glsl");
        resourceModule->RegisterLoader<Texture2DLoader>(".png");
        resourceModule->RegisterLoader<Texture2DLoader>(".jpg");

        RegisterModule<SceneModule>();
        RegisterModule<RenderModule>();
        RegisterModule<ImGuiModule>();
    }

    void Application::InitializeModules()
    {
        Log::Info("Initializing engine modules...");
        for (auto& id : m_moduleOrder)
        {
            Log::Info("Initializing module: {}", id.name());
            m_engineModules[id]->Initialize();
        }
    }

    void Application::ShutdownModules()
    {
        Log::Info("Shutting down engine modules...");
        for (auto it = m_moduleOrder.rbegin(); it != m_moduleOrder.rend(); ++it)
        {
            Log::Info("Shutting down module: {}", it->name());
            m_engineModules[*it]->Shutdown();
        }
    }

    const GraphicsDevice::API Application::GetRenderAPI() const
    {
        return m_appConfig.renderAPI;
    }

    ApplicationWindow& Application::GetApplicationWindow()
    {
        return *m_applicationWindow;
    }
}
