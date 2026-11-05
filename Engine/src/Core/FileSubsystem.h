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

    /// Maps virtual paths such as `project://Scenes/main.scene.json` to physical paths on disk.
    ///
    /// @code{.cpp}
    /// Path physical = file.Resolve("project://Scenes/main.scene.json");
    /// file.Mount(MountPoints::Editor, AX_EDITOR_DIR); // the mount name has no "://"
    /// @endcode
    ///
    /// Mount during Application::OnRegisterModules(). `engine` and `project` must be mounted before initialization.
    ///
    /// Resolve() asserts on a path with no `://` or an unknown mount, and lowercases the whole path.
    /// It only goes from virtual to physical; there is no way back.
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