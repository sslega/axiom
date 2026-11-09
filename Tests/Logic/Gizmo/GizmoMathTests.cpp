#include "TestHelpers.h"
#include "Gizmo/GizmoMath.h"
#include "Math/EulerAngles.h"
#include "Scene/TransformComponent.h"

using namespace axiom;
using namespace axiom::tests;

TEST_SUITE("logic")
{
    namespace
    {
        // A gizmo rotating about world Y: M' = R * M.
        void CheckWorldRotationDrag(Vec3 euler)
        {
            const Matrix4 composed = ComposeEulerRotation(euler);
            const Matrix4 rotated = Matrix4::Rotate({0, 1, 0}, 0.2f) * composed;
            const Vec3 afterDrag = RotationFromGizmo(rotated, Vec3(1.0f), euler);

            INFO("euler ", Describe(euler), " after drag ", Describe(afterDrag));
            CHECK(IsFinite(afterDrag));
            CHECK(MaxDifference3x3(ComposeEulerRotation(afterDrag), rotated) <= 1e-3f);
        }

        void CheckScale(Vec3 euler, Vec3 scale, Vec3 ratio)
        {
            TransformComponent transform;
            transform.rotation = euler;
            transform.scale = scale;

            const Vec3 feed = GizmoFeedScale(scale);
            const Matrix4 fed = ComposeGizmoMatrix(transform);

            const Vec3 rotationOut = RotationFromGizmo(fed, feed, euler);
            INFO("euler ", Describe(euler), ", scale ", Describe(scale), ", ratio ", Describe(ratio));
            INFO("rotation out ", Describe(rotationOut));
            CHECK(IsFinite(rotationOut));
            CHECK(MaxDifference3x3(ComposeEulerRotation(rotationOut), ComposeEulerRotation(euler)) <= 1e-3f);

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
            INFO("scale out ", Describe(scaleOut), ", expected ", Describe(expected));
            CHECK(IsFinite(scaleOut));
            CHECK(MaxDifference(scaleOut, expected) <= 1e-3f);
        }

        const Vec3 kScaleRotations[] = { Vec3(0.0f), Vec3(0.5f, 0.7f, 0.3f), Vec3(-1.0f, kHalfPi, 2.0f) };
    }

    TEST_CASE("World-axis rotation drag keeps the rotation")
    {
        ForEachEuler(kYAngles, CheckWorldRotationDrag);
        ForEachEuler(kNearGimbalYAngles, CheckWorldRotationDrag);
    }

    TEST_CASE("Scale drag on a mirrored entity keeps the sign")
    {
        for (const Vec3& euler : kScaleRotations)
        {
            CheckScale(euler, Vec3(-2.0f, 0.5f, 3.0f), Vec3(1.5f, 1.0f, 0.25f));
        }
    }

    TEST_CASE("Scale drag leaves a zero axis at zero")
    {
        for (const Vec3& euler : kScaleRotations)
        {
            CheckScale(euler, Vec3(0.0f, 1.0f, 2.0f), Vec3(2.0f, 1.0f, 1.0f));
        }
    }

    TEST_CASE("Scale drag with every axis at zero stays finite")
    {
        for (const Vec3& euler : kScaleRotations)
        {
            CheckScale(euler, Vec3(0.0f), Vec3(3.0f));
        }
    }
}
