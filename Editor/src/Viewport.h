#pragma once
#include "AxiomEngine.h"
#include "Renderer/View.h"
#include "EditorCamera.h"
namespace axiom
{
    class PostProcessBase;
    class Viewport
    {
    public:
        Viewport(SharedPtr<FrameBuffer> frameBuffer, String name = "");
        inline const View& GetView() { return m_view; }
        inline uint32 GetWidth() { return m_width; }
        inline uint32 GetHeight() { return m_height; }
        inline const EditorCamera& GetCamera() { return m_camera; }
        inline const String& GetName() { return m_name; }
        inline const SharedPtr<FrameBuffer> GetFrameBuffer() { return m_frameBuffer; }
        inline void SetActive(bool isActive) { m_isActive = isActive; }
        void OnUpdate(float deltaTime);
        void OnRender();
        void Resize(uint32 width, uint32 height);
        
        template <typename T, typename... Args>
        T& AddPostProcess(Args&&... args)
        {
            static_assert(std::is_base_of_v<PostProcessBase, T>, "T must derive from PostProcessBase");
            auto& pp = m_postProcesses.emplace_back(MakeUnique<T>(std::forward<Args>(args)...));
            return static_cast<T&>(*pp);
        }
    private:
        View m_view;
        EditorCamera m_camera;
        SharedPtr<FrameBuffer> m_frameBuffer;
        uint32 m_width {32};
        uint32 m_height {32};
        String m_name;
        bool m_isActive {false};
        Vector<UniquePtr<PostProcessBase>> m_postProcesses;
    };
}