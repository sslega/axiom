#include "UI/ResourceCandidates.h"
#include "ResourceListing.h"
#include "Core/FileSubsystem.h"
#include "Resources/ResourceSubsystem.h"

namespace axiom
{
    Vector<String> ListResourceCandidates(const FileSubsystem& files, const ResourceSubsystem& resources, std::type_index resourceType)
    {
        Vector<ResourceRoot> roots;
        for (const char* mountPoint : { MountPoints::Engine, MountPoints::Project })
        {
            const String prefix = String(mountPoint) + "://";
            roots.push_back({ files.Resolve(prefix), prefix });
        }
        return ListResourceFiles(roots, resources.GetExtensions(resourceType));
    }
}
