#pragma once

#include "Core/Types.h"
#include "Core/ApplicationSubsystem.h"

namespace axiom
{
    namespace MountPoints
    {
        constexpr const char* Engine  = "engine";
        constexpr const char* Project = "project";
        constexpr const char* Editor  = "editor";  // Editor target only
    }

    class FileSubsystem : public ApplicationSubsystem
    {
    public:
        FileSubsystem(Application& application);
        ~FileSubsystem() = default;

        void Mount(const String& virtualPath, const Path& physicalPath);
        bool MountExists(const String &mountPoint) const;
        Path Resolve(const String& virtualPath) const;

    protected:
        void OnInitialize() override;
    private:
        StringMap<Path> m_mountPoints;
        String ToLower(const String& str) const;
    };
}