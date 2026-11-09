#include "Math/EulerAngles.h"
#include "Math/Math.h"

#include <algorithm>
#include <cmath>

namespace axiom
{
    namespace
    {
        constexpr float kTwoPi = 2.0f * PI;

        // Below this distance from +/-1, cos(y) is too small for the usual formulas to be reliable.
        constexpr float kGimbalThreshold = 0.9999998f;

        float WrapNear(float angle, float reference)
        {
            return angle + kTwoPi * std::round((reference - angle) / kTwoPi);
        }

        Vec3 WrapNear(Vec3 angles, Vec3 reference)
        {
            return Vec3(WrapNear(angles.x, reference.x), WrapNear(angles.y, reference.y), WrapNear(angles.z, reference.z));
        }

        float ManhattanDistance(Vec3 a, Vec3 b)
        {
            return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
        }
    }

    Matrix4 ComposeEulerRotation(Vec3 eulerRadians)
    {
        return Matrix4::Rotate({1, 0, 0}, eulerRadians.x)
            * Matrix4::Rotate({0, 1, 0}, eulerRadians.y)
            * Matrix4::Rotate({0, 0, 1}, eulerRadians.z);
    }

    Vec3 DecomposeEulerRotation(const Matrix4& rotation, Vec3 reference)
    {
        const float sinY = std::clamp(rotation(0, 2), -1.0f, 1.0f);
        const float y = std::asin(sinY);

        if (std::abs(sinY) > kGimbalThreshold)
        {
            // Gimbal lock: X and Z turn about the same axis, so only their sum (or difference) is known.
            // Keep the X we had and give the rest to Z.
            const float combined = std::atan2(rotation(1, 0), rotation(1, 1));
            const float z = sinY > 0.0f ? combined - reference.x : combined + reference.x;
            return Vec3(reference.x, WrapNear(y, reference.y), WrapNear(z, reference.z));
        }

        const Vec3 first(std::atan2(-rotation(1, 2), rotation(2, 2)), y, std::atan2(-rotation(0, 1), rotation(0, 0)));
        const Vec3 second(first.x + PI, PI - first.y, first.z + PI);

        const Vec3 firstWrapped = WrapNear(first, reference);
        const Vec3 secondWrapped = WrapNear(second, reference);
        return ManhattanDistance(firstWrapped, reference) <= ManhattanDistance(secondWrapped, reference) ? firstWrapped : secondWrapped;
    }
}
