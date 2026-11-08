#include "SmokeChecks.h"
#include "UI/GizmoMath.h"
#include "Scene/TransformComponent.h"
#include "Core/Log.h"
#include "Math/Math.h"

#include <algorithm>
#include <cmath>
#include <format>

namespace axiom
{
    namespace
    {
        constexpr float kHalfPi = PI * 0.5f;

        float MaxDifference3x3(const Matrix4& a, const Matrix4& b)
        {
            float largest = 0.0f;
            for (int row = 0; row < 3; ++row)
            {
                for (int column = 0; column < 3; ++column)
                {
                    largest = std::max(largest, std::abs(a(row, column) - b(row, column)));
                }
            }
            return largest;
        }

        float MaxDifference(Vec3 a, Vec3 b)
        {
            return std::max({ std::abs(a.x - b.x), std::abs(a.y - b.y), std::abs(a.z - b.z) });
        }

        String Describe(Vec3 euler)
        {
            return std::format("({}, {}, {})", euler.x, euler.y, euler.z);
        }

        // Gizmo drags go through ComposeGizmoMatrix, RotationFromGizmo and ScaleFromGizmo, and the stored
        // values must survive them. These cases stand in for what a drag produces.
        void CheckEuler(Vec3 euler, int& failures)
        {
            TransformComponent transform;
            transform.rotation = euler;
            const Matrix4 composed = transform.GetTransform();

            if (MaxDifference3x3(composed, ComposeEulerRotation(euler)) > 1e-4f)
            {
                Log::Error("Gizmo math: ComposeEulerRotation differs from TransformComponent for {}", Describe(euler));
                ++failures;
            }

            const Vec3 decomposed = DecomposeEulerRotation(composed, euler);
            if (!IsFinite(decomposed) || MaxDifference3x3(ComposeEulerRotation(decomposed), composed) > 1e-3f)
            {
                Log::Error("Gizmo math: round trip failed for {}, got {}", Describe(euler), Describe(decomposed));
                ++failures;
            }

            // Away from gimbal lock, the value closest to the stored one is the stored one: no pop at drag start.
            if (std::abs(composed(0, 2)) < 0.999f && MaxDifference(decomposed, euler) > 2e-3f)
            {
                Log::Error("Gizmo math: decomposition drifted for {}, got {}", Describe(euler), Describe(decomposed));
                ++failures;
            }

            // A gizmo rotating about world Y: M' = R * M.
            const Matrix4 rotated = Matrix4::Rotate({0, 1, 0}, 0.2f) * composed;
            const Vec3 afterDrag = RotationFromGizmo(rotated, Vec3(1.0f), euler);
            if (!IsFinite(afterDrag) || MaxDifference3x3(ComposeEulerRotation(afterDrag), rotated) > 1e-3f)
            {
                Log::Error("Gizmo math: world rotation failed for {}, got {}", Describe(euler), Describe(afterDrag));
                ++failures;
            }
        }

        void CheckScale(Vec3 euler, Vec3 scale, Vec3 ratio, int& failures)
        {
            TransformComponent transform;
            transform.rotation = euler;
            transform.scale = scale;

            const Vec3 feed = GizmoFeedScale(scale);
            const Matrix4 fed = ComposeGizmoMatrix(transform);

            const Vec3 rotationOut = RotationFromGizmo(fed, feed, euler);
            if (!IsFinite(rotationOut) || MaxDifference3x3(ComposeEulerRotation(rotationOut), ComposeEulerRotation(euler)) > 1e-3f)
            {
                Log::Error("Gizmo math: rotation lost with scale {}", Describe(scale));
                ++failures;
            }

            // A scale drag multiplies each column of the fed matrix.
            Matrix4 dragged = fed;
            const float ratios[3] = { ratio.x, ratio.y, ratio.z };
            for (int column = 0; column < 3; ++column)
            {
                for (int row = 0; row < 3; ++row)
                {
                    dragged(row, column) *= ratios[column];
                }
            }

            const Vec3 expected(scale.x * ratio.x, scale.y * ratio.y, scale.z * ratio.z);
            const Vec3 scaleOut = ScaleFromGizmo(dragged, ComposeEulerRotation(euler), scale, feed);
            if (!IsFinite(scaleOut) || MaxDifference(scaleOut, expected) > 1e-3f)
            {
                Log::Error("Gizmo math: scale {} with ratio {} gave {}", Describe(scale), Describe(ratio), Describe(scaleOut));
                ++failures;
            }
        }
    }

    void RunGizmoMathChecks()
    {
        const float angles[] = { -3.0f, -2.0f, -1.0f, -0.3f, 0.0f, 0.3f, 0.7f, 1.2f, 2.5f, 3.0f };
        const float yAngles[] = {
            -2.0f, -0.5f, 0.0f, 0.7f, 2.0f,
            kHalfPi, -kHalfPi,
            kHalfPi - 1e-3f, -kHalfPi + 1e-3f,
            kHalfPi + 1e-3f, -kHalfPi - 1e-3f,
            kHalfPi - 1e-4f, -kHalfPi + 1e-4f,
        };

        int failures = 0;
        int cases = 0;

        for (float x : angles)
        {
            for (float y : yAngles)
            {
                for (float z : angles)
                {
                    CheckEuler(Vec3(x, y, z), failures);
                    ++cases;
                }
            }
        }

        const Vec3 rotations[] = { Vec3(0.0f), Vec3(0.5f, 0.7f, 0.3f), Vec3(-1.0f, kHalfPi, 2.0f) };
        for (const Vec3& euler : rotations)
        {
            CheckScale(euler, Vec3(-2.0f, 0.5f, 3.0f), Vec3(1.5f, 1.0f, 0.25f), failures);   // mirrored
            CheckScale(euler, Vec3(0.0f, 1.0f, 2.0f), Vec3(2.0f, 1.0f, 1.0f), failures);     // zero axis
            CheckScale(euler, Vec3(0.0f), Vec3(3.0f), failures);                             // all zero
            cases += 3;
        }

        if (failures == 0)
        {
            Log::Info("Smoke: gizmo math checks passed ({} cases)", cases);
        }
    }
}
