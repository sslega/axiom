#include "RuntimeTest.h"

#include "../../Editor/external/IconFontCppHeaders/IconsFontAwesome6.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace axiom::tests
{
    namespace
    {
        ImU32 AsBytes(ImGuiCol colorSlot)
        {
            return ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[colorSlot]);
        }

        ImWchar FirstCodepoint(const char* utf8)
        {
            unsigned int codepoint = 0;
            ImTextCharFromUtf8(&codepoint, utf8, nullptr);
            return static_cast<ImWchar>(codepoint);
        }
    }

    TEST_SUITE("runtime")
    {
        TEST_CASE_FIXTURE(RuntimeTest, "the theme's palette is applied after boot")
        {
            RunFrames(1);

            CHECK(AsBytes(ImGuiCol_CheckMark) == IM_COL32(0xDA, 0x7D, 0x14, 0xFF));
            CHECK(AsBytes(ImGuiCol_WindowBg) == IM_COL32(0x25, 0x25, 0x28, 0xFF));
        }

        TEST_CASE_FIXTURE(RuntimeTest, "tab bars have no drop-down arrow or accent overline")
        {
            RunFrames(1);

            CHECK(ImGui::GetStyle().WindowMenuButtonPosition == ImGuiDir_None);
            CHECK(ImGui::GetStyle().TabBarOverlineSize == 0.0f);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "the default font holds both text and icon glyphs")
        {
            ImFont* font = nullptr;
            DuringNextFrame([&] { font = ImGui::GetFont(); });

            REQUIRE(font != nullptr);
            CHECK(font->IsGlyphInFont(static_cast<ImWchar>('A')));
            CHECK(font->IsGlyphInFont(FirstCodepoint(ICON_FA_ROTATE)));
        }
    }
}
