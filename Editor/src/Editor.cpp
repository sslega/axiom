#include "Editor.h"
#include "EntryPoint.h"
#include "Renderer/FrameBuffer.h"
#include <imgui.h>

UniquePtr<Application> CreateApplication()
{

    AppConfig appConfig;
    appConfig.renderAPI = GraphicsDevice::API::OpenGL;
    
    AppWindowConfig windowConfig;
    windowConfig.width = 1280;
    windowConfig.height = 720;
    windowConfig.title = "Axiom Editor";
    windowConfig.vsync = true;
    windowConfig.fullscreen = false;
    windowConfig.backend = ApplicationWindowBackend::GLFW;

    appConfig.windowConfig = windowConfig;
    return MakeUnique<Editor>(appConfig);
};

namespace axiom
{
    Editor::Editor(AppConfig appConfig)
    : Application(appConfig)
    {
    }

    void Editor::OnResize(const WindowResizeEvent &event)
    {
    }
    void Editor::OnRegisterModules()
    {
        FileSubsystem& fileSystemModule = GetSubsystem<FileSubsystem>();

        fileSystemModule.Mount(MountPoints::Editor, AX_EDITOR_DIR);
        fileSystemModule.Mount(MountPoints::Project, AX_DEV_PROJECT_DIR); // read that from ini or "Open Project" dialog
    }

    void Editor::OnInitializeModules()
    {
        axiom::FramebufferSpec frameBufferSpec;
        frameBufferSpec.width = GetApplicationWindow().GetWidth();
        frameBufferSpec.height = GetApplicationWindow().GetHeight();
        frameBufferSpec.depthOnly = false;

        m_viewportFrameBuffer = GetSubsystem<RenderSubsystem>().GetGraphicsDevice().CreateFrameBuffer(frameBufferSpec);
        m_viewportFrameBuffer2 = GetSubsystem<RenderSubsystem>().GetGraphicsDevice().CreateFrameBuffer(frameBufferSpec);

    }

    void Editor::OnApplicationRun()
    {
        GetSubsystem<WorldSubsystem>().LoadScene("project://Scenes/main.scene.json");
    }

    void Editor::OnRender()
    {
        m_viewportFocused = false;
        m_viewport2Focused = false;

        {
            View view;
            view.cameraPosition = m_viewportCamera.GetCamera().GetPosition();
            view.clearColor = Vec4(0.1f, 0.1f, 0.1f, 1.0f);
            view.debugName = "Viewport";
            view.width = m_viewportFrameBuffer->GetWidth();
            view.height = m_viewportFrameBuffer->GetHeight();
            view.renderTarget = m_viewportFrameBuffer;
            view.viewProjection = m_viewportCamera.GetCamera().GetViewProjectionMatrix();
            
            GetSubsystem<RenderSubsystem>().SubmitView(view);


            ImGui::Begin("Viewport");
            m_viewportFocused = ImGui::IsWindowFocused();
            ImVec2 available = ImGui::GetContentRegionAvail();

            Vec2 windowSize = Vec2(available.x, available.y);
            if(windowSize.x != 0 && windowSize.y != 0 && windowSize != m_lastViewportSize)
            {
                m_viewportFrameBuffer->Resize(windowSize.x, windowSize.y);
                float aspectRatio = windowSize.x / windowSize.y;
                m_viewportCamera.SetAspectRatio(aspectRatio);
                m_lastViewportSize = windowSize;
            }

            uint32 colorId = m_viewportFrameBuffer->GetColorAttachmentID();
            ImGui::Image((ImTextureID)(uintptr_t)colorId,
                ImVec2((float)view.width, (float)view.height),
                ImVec2(0,1), ImVec2(1,0));  // flip UVs — OpenGL framebuffers are upside down
            ImGui::End();
        }

        ///////////

        {
            View view;
            view.cameraPosition = m_viewportCamera2.GetCamera().GetPosition();
            view.clearColor = Vec4(0.1f, 0.1f, 0.1f, 1.0f);
            view.debugName = "Viewport2";
            view.width = m_viewportFrameBuffer2->GetWidth();
            view.height = m_viewportFrameBuffer2->GetHeight();
            view.renderTarget = m_viewportFrameBuffer2;
            view.viewProjection = m_viewportCamera2.GetCamera().GetViewProjectionMatrix();
            
            GetSubsystem<RenderSubsystem>().SubmitView(view);


            ImGui::Begin("Viewport2");
            m_viewport2Focused = ImGui::IsWindowFocused();

            ImVec2 available = ImGui::GetContentRegionAvail();

            Vec2 windowSize = Vec2(available.x, available.y);
            if(windowSize.x != 0 && windowSize.y != 0 && windowSize != m_lastViewportSize2)
            {
                m_viewportFrameBuffer2->Resize(windowSize.x, windowSize.y);
                float aspectRatio = windowSize.x / windowSize.y;
                m_viewportCamera2.SetAspectRatio(aspectRatio);
                m_lastViewportSize2 = windowSize;
            }

            uint32 colorId = m_viewportFrameBuffer2->GetColorAttachmentID();
            ImGui::Image((ImTextureID)(uintptr_t)colorId,
                ImVec2((float)view.width, (float)view.height),
                ImVec2(0,1), ImVec2(1,0));  // flip UVs — OpenGL framebuffers are upside down
            ImGui::End();
        }

    }

    void Editor::OnUpdate(float deltaTime)
    {
        if(m_viewportFocused)
            m_viewportCamera.OnUpdate(deltaTime);
        
        if(m_viewport2Focused)
            m_viewportCamera2.OnUpdate(deltaTime);
    }
}
