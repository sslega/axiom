#include "Core/FileSubsystem.h"
#include "Core/Assert.h"
#include <filesystem>

namespace axiom
{
    FileSubsystem::FileSubsystem(Application& application)
    :ApplicationSubsystem(application)
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

    void FileSubsystem::OnInitialize()
    {
        AX_ASSERT(MountExists("engine"), "Engine folder not mount.");
        AX_ASSERT(MountExists("project"), "Project folder not mount.");
    }

    String FileSubsystem::ToLower(const String& str) const
    {
        String result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;

    }
}
