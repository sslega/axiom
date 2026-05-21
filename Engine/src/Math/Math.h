#pragma once
#include <cmath>

#include "Math/Vector.h"

namespace axiom
{
    constexpr float PI = 3.14159265358979323846f;

    inline float ToRadians(float degrees) { return degrees * (PI / 180.0f); }
    inline Vec3 ToRadians(Vec3 degrees) { return Vec3(ToRadians(degrees.x), ToRadians(degrees.y), ToRadians(degrees.z)); }
    inline float ToDegrees(float radians) { return radians * (180.0f / PI); }
    inline Vec3 ToDegrees(Vec3 radians) { return Vec3(ToDegrees(radians.x), ToDegrees(radians.y), ToDegrees(radians.z)); }
    inline float MillisecondsToSeconds(float ms) { return ms / 1000.0f; }
}
