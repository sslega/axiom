#pragma once
#include "Core/Types.h"
#include <optional>

namespace axiom::FileDialog
{
    std::optional<Path> OpenFile(const Path& initialDir);
}