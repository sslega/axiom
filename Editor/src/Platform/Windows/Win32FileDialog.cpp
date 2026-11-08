#define NOMINMAX
#include "Platform/FileDialog.h"
#include "windows.h"
#include "commdlg.h"

namespace axiom::FileDialog
{
    namespace
    {
        const wchar_t* const SceneExtension = L".scene.json";

        std::optional<Path> ShowSceneDialog(BOOL (APIENTRY* showDialog)(LPOPENFILENAMEW), const Path& initialDir, DWORD flags)
        {
            // Win32 wants backslashes, and mount paths can hold forward slashes.
            const std::wstring preferredDir = Path(initialDir).make_preferred().wstring();

            wchar_t buffer[MAX_PATH] = {};
            OPENFILENAMEW ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = GetActiveWindow();
            ofn.lpstrFilter = L"Scene files (*.scene.json)\0*.scene.json\0";
            ofn.lpstrFile = buffer;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrInitialDir = preferredDir.c_str();
            ofn.Flags = flags | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
            if (!showDialog(&ofn)) return std::nullopt;
            return Path(buffer);
        }
    }

    std::optional<Path> OpenFile(const Path& initialDir)
    {
        return ShowSceneDialog(GetOpenFileNameW, initialDir, OFN_FILEMUSTEXIST);
    }

    std::optional<Path> SaveFile(const Path& initialDir)
    {
        std::optional<Path> path = ShowSceneDialog(GetSaveFileNameW, initialDir, OFN_OVERWRITEPROMPT);
        if (path && !path->wstring().ends_with(SceneExtension))
        {
            *path += SceneExtension;
        }
        return path;
    }
}
