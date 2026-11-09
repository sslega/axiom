#include "Core/FileSubsystem.h"
#include "Core/Assert.h"
#include <filesystem>

namespace axiom
{
    FileSubsystem::FileSubsystem(SubsystemHost& host)
    :ApplicationSubsystem(host)
    {

    };

    void FileSubsystem::Mount(const String& virtualPath, const Path& physicalPath)
    {
        AX_ASSERT(!virtualPath.empty(), "Mount point name is empty");
        AX_ASSERT(std::filesystem::exists(physicalPath), "Physical path does not exist");
        String virtualPathLowerCase = ToLower(virtualPath);
        m_mountPoints[virtualPathLowerCase] = physicalPath;
    }

    bool FileSubsystem::MountExists(const String& mountPoint) const
    {
        String mountPointLowerCase = ToLower(mountPoint);
        auto it = m_mountPoints.find(mountPointLowerCase);
        return it != m_mountPoints.end();
    }

    Path FileSubsystem::Resolve(const String& virtualPath) const
    {
        String virtualPathLowerCase = ToLower(virtualPath);

        auto separatorPos = virtualPathLowerCase.find("://");
        AX_ASSERT(separatorPos != String::npos, "Invalid virtual path, missing ://");

        String mountPoint   = virtualPathLowerCase.substr(0, separatorPos);
        String relativePath = virtualPathLowerCase.substr(separatorPos + 3);

        auto it = m_mountPoints.find(mountPoint);
        AX_ASSERT(it != m_mountPoints.end(), "Mount point not registered");

        return it->second / relativePath;
    }

    std::optional<String> FileSubsystem::ToVirtual(const Path& physicalPath) const
    {
        // Canonical forms, because the same folder can arrive as z:\Axiom and Z:/Axiom.
        std::error_code error;
        const Path target = std::filesystem::weakly_canonical(physicalPath, error);
        if(error)
        {
            return std::nullopt;
        }

        for(const auto& [mountPoint, mountPath] : m_mountPoints)
        {
            const Path root = std::filesystem::weakly_canonical(mountPath, error);
            if(error)
            {
                continue;
            }

            const Path relative = target.lexically_relative(root);
            if(relative.empty() || *relative.begin() == "..")
            {
                continue;
            }
            return mountPoint + "://" + relative.generic_string();
        }

        return std::nullopt;
    }

    void FileSubsystem::OnInitialize()
    {
        AX_ASSERT(MountExists(MountPoints::Engine), "Engine folder not mount.");
        AX_ASSERT(MountExists(MountPoints::Project), "Project folder not mount.");
    }

    String FileSubsystem::ToLower(const String& str) const
    {
        String result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;

    }
}
