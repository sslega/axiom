#pragma once
#include "AxiomEngine.h"
#include "Renderer/View.h"
#include "EditorCamera.h"
namespace axiom
{
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
    private:
        View m_view;
        EditorCamera m_camera;
        SharedPtr<FrameBuffer> m_frameBuffer;
        uint32 m_width {32};
        uint32 m_height {32};
        String m_name;
        bool m_isActive {false};
    };
}