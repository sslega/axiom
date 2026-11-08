#pragma once

#include "UI/EditorPanel.h"
#include "UI/GizmoState.h"
#include "Math/Vector.h"
#include <optional>

namespace axiom
{
    class RenderSubsystem;
    class Viewport;
    class FrameBuffer;
    class Selection;
    class UndoStack;
    class TransformComponent;
    struct FieldDescriptor;
    struct Matrix4;

    /// A dockable 3D view: its Viewport image, plus the transform gizmo and mode toolbar for the selected entity.
    ///
    /// The gizmo mode is shared by all viewports through GizmoState. Dragging a handle edits one reflected field
    /// of TransformComponent live, and becomes one undo step when the drag ends.
    class ViewportPanel : public EditorPanel
    {
    public:
        ViewportPanel(const String& name, RenderSubsystem& renderSubsystem, Selection& selection, UndoStack& undoStack, GizmoState& gizmoState);
        ViewportPanel(const String& name, uint32 width, uint32 height, RenderSubsystem& renderSubsystem, Selection& selection, UndoStack& undoStack, GizmoState& gizmoState);
        ~ViewportPanel();

        virtual void OnRender() override;
        virtual void OnUpdate(float deltaTime) override;

        Viewport* GetViewport() { return m_viewport.get(); }

    private:
        struct GizmoDrag
        {
            TransformComponent* transform;
            const FieldDescriptor* field;
            GizmoMode mode;
            Vec3 startValue;
        };

        RenderSubsystem& m_renderSubsystem;
        Selection& m_selection;
        UndoStack& m_undoStack;
        GizmoState& m_gizmoState;
        UniquePtr<Viewport> m_viewport;

        std::optional<GizmoDrag> m_drag;
        bool m_gizmoWantsMouse = false;
        bool m_cameraActive = false;
        bool m_looking = false;

        void UpdateCameraActive();
        void HandleModeKeys();
        void DrawGizmo(const ImVec2& imagePos, const ImVec2& imageSize);
        void DrawToolbar(const ImVec2& imagePos);

        void BeginDrag(TransformComponent& transform, GizmoMode mode);
        void ApplyDrag(const Matrix4& gizmoMatrix, bool snapping);
        void EndDrag();
    };
}
