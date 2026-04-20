#pragma once
#include <cmath>

namespace axiom
{
    constexpr float PI = 3.14159265358979323846f;

    constexpr float ToRadians(float degrees) { return degrees * (PI / 180.0f); }
    constexpr float ToDegrees(float radians) { return radians * (180.0f / PI); }
    constexpr float MillisecondsToSeconds(float ms) { return ms / 1000.0f; }
}
