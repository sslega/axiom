#pragma once

#include "Core/Types.h"

#include <algorithm>
#include <cctype>

namespace axiom
{
    /// Returns `text` in lower case.
    ///
    /// Virtual paths ignore case, so use this wherever a path is compared or used as a key.
    /// Keep the original string for storing and showing a path, so a saved scene keeps its casing.
    ///
    /// @code{.cpp}
    /// bool same = ToLower("engine://Meshes/Cube.obj") == ToLower("engine://meshes/cube.obj"); // true
    /// @endcode
    inline String ToLower(StringView text)
    {
        String result(text);
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return result;
    }
}
