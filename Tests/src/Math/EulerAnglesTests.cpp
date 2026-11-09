#include "TestHelpers.h"
#include "Math/EulerAngles.h"

using namespace axiom;
using namespace axiom::tests;

namespace
{
    void CheckRoundTrip(Vec3 euler)
    {
        const Matrix4 composed = ComposeEulerRotation(euler);
        const Vec3 decomposed = DecomposeEulerRotation(composed, euler);

        INFO("euler ", Describe(euler), " decomposed to ", Describe(decomposed));
        CHECK(IsFinite(decomposed));
        CHECK(MaxDifference3x3(ComposeEulerRotation(decomposed), composed) <= 1e-3f);
    }

    // Away from gimbal lock, the value closest to the stored one is the stored one: no pop at drag start.
    void CheckNoDrift(Vec3 euler)
    {
        const Matrix4 composed = ComposeEulerRotation(euler);
        if (std::abs(composed(0, 2)) >= 0.999f)
        {
            return;
        }

        const Vec3 decomposed = DecomposeEulerRotation(composed, euler);

        INFO("euler ", Describe(euler), " decomposed to ", Describe(decomposed));
        CHECK(MaxDifference(decomposed, euler) <= 2e-3f);
    }
}

TEST_CASE("Euler round trip recovers the rotation")
{
    ForEachEuler(kYAngles, CheckRoundTrip);
}

TEST_CASE("Euler round trip near +/-90 degrees Y")
{
    ForEachEuler(kNearGimbalYAngles, CheckRoundTrip);
}

TEST_CASE("Decomposing keeps the stored angles away from gimbal lock")
{
    ForEachEuler(kYAngles, CheckNoDrift);
    ForEachEuler(kNearGimbalYAngles, CheckNoDrift);
}
