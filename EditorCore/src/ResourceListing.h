#pragma once

#include "Core/Types.h"

namespace axiom
{
    /// One folder to search, and the virtual path prefix its files get.
    struct ResourceRoot
    {
        Path folder;
        String virtualPrefix; // e.g. "engine://"
    };

    /// Lists the files under `roots` whose extension is one of `extensions` (with the dot).
    /// Returns virtual paths with forward slashes, sorted by name without regard to case.
    ///
    /// It takes plain data so it needs no subsystem; the Editor supplies the roots and extensions.
    ///
    /// @code{.cpp}
    /// Vector<String> meshes = ListResourceFiles({ { "C:/Game/Assets", "project://" } }, { ".obj" });
    /// // "project://Meshes/bunny.obj"
    /// @endcode
    ///
    /// The extension match ignores case, because FileSubsystem lowercases a path before the loader sees it.
    /// A root that doesn't exist is skipped. It reads the disk on every call, so don't call it each frame.
    Vector<String> ListResourceFiles(const Vector<ResourceRoot>& roots, const Vector<String>& extensions);
}
