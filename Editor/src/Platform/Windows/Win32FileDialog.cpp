#define NOMINMAX
#include "Platform/FileDialog.h"
#include "windows.h"
#include "commdlg.h"

namespace axiom::FileDialog
{
    std::optional<Path> OpenFile(const Path& initialDir)
    {
        wchar_t buffer[MAX_PATH] = {};
        OPENFILENAMEW ofn = {};
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = GetActiveWindow();
        ofn.lpstrFilter = L"Scene files (*.scene.json)\0*.scene.json\0";
        ofn.lpstrFile = buffer;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrInitialDir = initialDir.c_str();
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
        if (!GetOpenFileNameW(&ofn)) return std::nullopt;
        return Path(buffer);

    }
}