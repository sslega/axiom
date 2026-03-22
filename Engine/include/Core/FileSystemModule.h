#pragma once

#include "Core/Types.h"
#include "Core/EngineModule.h"

namespace axiom
{
    class FileSystemModule : public EngineModule
    {
    public:
        FileSystemModule(Application& engine);
        ~FileSystemModule() = default;

        void Mount(const String& virtualPath, const Path& physicalPath);
        bool MountExists(const String &mountPoint) const;
        Path Resolve(const String& virtualPath) const;

        virtual bool Initialize() override;
    private:
        StringMap<Path> m_mountPoints;
        String ToLower(const String& str) const;
    };
}