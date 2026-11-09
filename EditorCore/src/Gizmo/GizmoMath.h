#pragma once

#include "Math/Matrix4.h"
#include "Math/Vector.h"

namespace axiom
{
    class TransformComponent;

    // Helpers between a gizmo's matrix and TransformComponent's fields. ImGui-free.

    /// The scale a gizmo's matrix should be built with: the stored scale, with zero axes replaced by 1
    /// so the matrix stays invertible. The stored scale itself is never changed by this.
    Vec3 GizmoFeedScale(Vec3 scale);

    Matrix4 ComposeGizmoMatrix(const TransformComponent& transform);

    /// New rotation after a drag, from the matrix the gizmo returned. Scale comes from `feedScale`,
    /// not from column lengths, so a mirrored entity keeps its sign.
    Vec3 RotationFromGizmo(const Matrix4& gizmoMatrix, Vec3 feedScale, Vec3 reference);

    /// New scale after a drag, from the matrix the gizmo returned. Measured along the entity's own axes
    /// (`rotation`) as a ratio of the scale the drag started from, so negative scales keep their sign
    /// and a zero axis stays zero.
    Vec3 ScaleFromGizmo(const Matrix4& gizmoMatrix, const Matrix4& rotation, Vec3 startScale, Vec3 startFeedScale);

    /// Applies a drag's result per axis: axes that didn't move keep their exact start value, the others
    /// snap to a multiple of `step` (no snapping when `step` is 0). `keepNonZero` stops snapping from
    /// collapsing an axis to 0.
    Vec3 ResolveDraggedAxes(Vec3 value, Vec3 start, float step, bool keepNonZero);

    /// Step for ImGuizmo's scale snap, so its handles move in steps of about `step` on an axis starting at
    /// `startScale`. ImGuizmo snaps the drag's multiplier, using one step for all axes, so this always keeps
    /// a multiplier of 1 on the grid and axes that aren't being dragged stay put.
    float ScaleMultiplierSnap(float startScale, float step);
}
