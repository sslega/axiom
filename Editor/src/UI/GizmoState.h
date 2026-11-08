#pragma once

namespace axiom
{
    enum class GizmoMode { None, Translate, Rotate, Scale };

    /// Transform gizmo state shared by every viewport. EditorUI owns it and passes it to the viewport panels.
    ///
    /// `dragging` is true for as long as any viewport is dragging a handle, so shortcuts and the other
    /// viewports can stay out of the way until the drag ends.
    struct GizmoState
    {
        GizmoMode mode = GizmoMode::Translate;
        bool dragging = false;
    };
}
