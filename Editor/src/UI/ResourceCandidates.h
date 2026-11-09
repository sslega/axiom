#pragma once

#include "Core/Types.h"

namespace axiom
{
    class FileSubsystem;
    class ResourceSubsystem;

    /// Every file under `engine://` and `project://` that loads as the given resource class, as sorted virtual paths.
    /// It reads the disk, so call it when a list opens rather than each frame.
    Vector<String> ListResourceCandidates(const FileSubsystem& files, const ResourceSubsystem& resources, std::type_index resourceType);
}
