#include "RuntimeTest.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>
#include <optional>

namespace axiom::tests
{
    namespace
    {
        // Dock node IDs from DefaultLayout.ini.
        constexpr ImGuiID kOutlinerNode = 0x00000001;
        constexpr ImGuiID kSceneNode = 0x00000002;
        constexpr ImGuiID kInspectorNode = 0x00000003;

        ImGuiID DockNodeOf(const char* windowName)
        {
            ImGuiWindow* window = ImGui::FindWindowByName(windowName);
            return window && window->DockNode ? window->DockNode->ID : 0;
        }

        std::optional<std::filesystem::file_time_type> LastWriteTime(const String& path)
        {
            std::error_code error;
            std::filesystem::file_time_type time = std::filesystem::last_write_time(path, error);
            if (error)
            {
                return std::nullopt;
            }
            return time;
        }
    }

    TEST_SUITE("runtime")
    {
        TEST_CASE_FIXTURE(RuntimeTest, "a test run starts from the default layout and never writes an ini")
        {
            const String& userLayoutPath = GetUI().GetUserLayoutPath();
            const std::optional<std::filesystem::file_time_type> before = LastWriteTime(userLayoutPath);

            RunFrames(3);
            GetUI().RequestLayoutReset();
            RunFrames(3);

            CHECK(ImGui::GetIO().IniFilename == nullptr);
            CHECK(LastWriteTime(userLayoutPath) == before);
            CHECK(DockNodeOf("Outliner") == kOutlinerNode);
            CHECK(DockNodeOf("Scene") == kSceneNode);
            CHECK(DockNodeOf("Inspector") == kInspectorNode);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "resetting the layout re-docks a floating panel")
        {
            DuringNextFrame([]
            {
                if (ImGuiWindow* inspector = ImGui::FindWindowByName("Inspector"))
                {
                    ImGui::DockContextQueueUndockWindow(ImGui::GetCurrentContext(), inspector);
                }
            });
            RunFrames(1);
            REQUIRE(DockNodeOf("Inspector") == 0);

            GetUI().RequestLayoutReset();
            RunFrames(1);

            CHECK(DockNodeOf("Outliner") == kOutlinerNode);
            CHECK(DockNodeOf("Scene") == kSceneNode);
            CHECK(DockNodeOf("Inspector") == kInspectorNode);
        }
    }
}
