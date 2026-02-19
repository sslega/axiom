#pragma once

#include <stdexcept>

namespace ax
{
    inline void ensure(bool condition, const char* message)
    {
        if (!condition)
            throw std::runtime_error(message);
    }

     inline void ensure(bool condition)
     {
        ensure(condition, "Assertion failed");
     }
}