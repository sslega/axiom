#pragma once

#include "Core/Types.h"
#include "Core/ApplicationModule.h"

namespace axiom
{
    class FileSystemModule : public ApplicationModule
    {
    public:
        FileSystemModule(Application& application);
        ~FileSystemModule() = default;

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