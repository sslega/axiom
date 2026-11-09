#include "Gizmo/GizmoMath.h"
#include "Math/EulerAngles.h"
#include "Scene/TransformComponent.h"

#include <algorithm>
#include <cmath>

namespace axiom
{
    namespace
    {
        // Smaller than any change a pixel of mouse movement produces, larger than float noise.
        constexpr float kUnmovedEpsilon = 1e-5f;

        Vec3 Column(const Matrix4& matrix, int column)
        {
            return Vec3(matrix(0, column), matrix(1, column), matrix(2, column));
        }

        float ResolveAxis(float value, float start, float step, bool keepNonZero)
        {
            // Relative, because a drag's float error grows with the size of the value.
            if (std::abs(value - start) < kUnmovedEpsilon * std::max(1.0f, std::abs(start)))
            {
                return start;
            }
            if (step <= 0.0f)
            {
                return value;
            }

            float snapped = std::round(value / step) * step + 0.0f; // + 0.0f turns -0 into 0
            if (keepNonZero && snapped == 0.0f)
            {
                snapped = std::copysign(step, value);
            }
            return snapped;
        }
    }

    Vec3 GizmoFeedScale(Vec3 scale)
    {
        const auto feed = [](float axis) { return std::abs(axis) < 1e-5f ? 1.0f : axis; };
        return Vec3(feed(scale.x), feed(scale.y), feed(scale.z));
    }

    Matrix4 ComposeGizmoMatrix(const TransformComponent& transform)
    {
        return Matrix4::Translate(transform.position) * ComposeEulerRotation(transform.rotation) * Matrix4::Scale(GizmoFeedScale(transform.scale));
    }

    Vec3 RotationFromGizmo(const Matrix4& gizmoMatrix, Vec3 feedScale, Vec3 reference)
    {
        const float feed[3] = { feedScale.x, feedScale.y, feedScale.z };

        Matrix4 rotation = Matrix4::Identity();
        for (int column = 0; column < 3; ++column)
        {
            for (int row = 0; row < 3; ++row)
            {
                rotation(row, column) = gizmoMatrix(row, column) / feed[column];
            }
        }
        return DecomposeEulerRotation(rotation, reference);
    }

    Vec3 ScaleFromGizmo(const Matrix4& gizmoMatrix, const Matrix4& rotation, Vec3 startScale, Vec3 startFeedScale)
    {
        const float start[3] = { startScale.x, startScale.y, startScale.z };
        const float feed[3] = { startFeedScale.x, startFeedScale.y, startFeedScale.z };

        float result[3];
        for (int axis = 0; axis < 3; ++axis)
        {
            const float ratio = Dot(Column(gizmoMatrix, axis), Column(rotation, axis)) / feed[axis];
            result[axis] = start[axis] * ratio;
        }
        return Vec3(result[0], result[1], result[2]);
    }

    Vec3 ResolveDraggedAxes(Vec3 value, Vec3 start, float step, bool keepNonZero)
    {
        return Vec3(
            ResolveAxis(value.x, start.x, step, keepNonZero),
            ResolveAxis(value.y, start.y, step, keepNonZero),
            ResolveAxis(value.z, start.z, step, keepNonZero)
        );
    }

    float ScaleMultiplierSnap(float startScale, float step)
    {
        return 1.0f / std::max(1.0f, std::round(std::abs(startScale) / step));
    }
}
