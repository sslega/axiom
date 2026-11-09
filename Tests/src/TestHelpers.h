#pragma once

#include "Core/Types.h"
#include "Math/Math.h"
#include "Math/Matrix4.h"
#include "Math/Vector.h"

#include <doctest/doctest.h>

#include <algorithm>
#include <cmath>
#include <format>
#include <span>

namespace axiom::tests
{
    inline constexpr float kHalfPi = PI * 0.5f;

    inline constexpr float kAngles[] = { -3.0f, -2.0f, -1.0f, -0.3f, 0.0f, 0.3f, 0.7f, 1.2f, 2.5f, 3.0f };
    inline constexpr float kYAngles[] = { -2.0f, -0.5f, 0.0f, 0.7f, 2.0f };
    inline constexpr float kNearGimbalYAngles[] = {
        kHalfPi, -kHalfPi,
        kHalfPi - 1e-3f, -kHalfPi + 1e-3f,
        kHalfPi + 1e-3f, -kHalfPi - 1e-3f,
        kHalfPi - 1e-4f, -kHalfPi + 1e-4f,
    };

    // Runs fn for every X and Z in kAngles combined with every Y in yAngles.
    template <typename Fn>
    void ForEachEuler(std::span<const float> yAngles, Fn&& fn)
    {
        for (float x : kAngles)
        {
            for (float y : yAngles)
            {
                for (float z : kAngles)
                {
                    fn(Vec3(x, y, z));
                }
            }
        }
    }

    inline float MaxDifference3x3(const Matrix4& a, const Matrix4& b)
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

    inline float MaxDifference(Vec3 a, Vec3 b)
    {
        return std::max({ std::abs(a.x - b.x), std::abs(a.y - b.y), std::abs(a.z - b.z) });
    }

    // doctest::String, because doctest prints a plain const char* as an address in INFO.
    inline doctest::String Describe(Vec3 value)
    {
        return std::format("({}, {}, {})", value.x, value.y, value.z).c_str();
    }
}
