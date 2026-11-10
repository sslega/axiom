#include "EditorTheme.h"
#include "AxiomEngine.h"

#include <imgui.h>

#include <cmath>
#include <filesystem>

namespace axiom
{
    namespace
    {
        constexpr ImVec4 Hex(uint32 rgb, float alpha = 1.0f)
        {
            return ImVec4(((rgb >> 16) & 0xFF) / 255.0f, ((rgb >> 8) & 0xFF) / 255.0f, (rgb & 0xFF) / 255.0f, alpha);
        }

        constexpr ImVec4 kAccent = Hex(0xDA7D14);
        constexpr ImVec4 kAccentHovered = Hex(0xE8923A);
        constexpr ImVec4 kAccentPressed = Hex(0xF2A65A);
        constexpr ImVec4 kSelected = Hex(0x774B21);
        constexpr ImVec4 kSelectedHovered = Hex(0x8A5A2C);
        constexpr ImVec4 kSelectedPressed = Hex(0x9C683A);
        constexpr ImVec4 kActiveToolBg = Hex(0x49321F);
        constexpr ImVec4 kActiveToolBgHovered = Hex(0x5A3E26);
        constexpr ImVec4 kActiveToolBgPressed = Hex(0x6B4A2D);
        constexpr ImVec4 kToolBg = Hex(0x2B2B2E);
        constexpr ImVec4 kToolBgHovered = Hex(0x343438);
        constexpr ImVec4 kToolBgPressed = Hex(0x3E3E42);
        constexpr ImVec4 kComponentHeader = Hex(0x313235);
        constexpr ImVec4 kComponentHeaderHovered = Hex(0x3A3B3F);
        constexpr ImVec4 kComponentHeaderPressed = Hex(0x44454A);

        constexpr ImVec4 kWindowBg = Hex(0x252528);
        constexpr ImVec4 kMenuBar = Hex(0x242328);
        constexpr ImVec4 kStrip = Hex(0x17181B);
        constexpr ImVec4 kTabBar = Hex(0x1F1F22);
        constexpr ImVec4 kTabInactive = Hex(0x222326);
        constexpr ImVec4 kTabHovered = Hex(0x2A2B2E);
        constexpr ImVec4 kTabActive = Hex(0x2E2E32);
        constexpr ImVec4 kFrameBg = Hex(0x1D1D20);
        constexpr ImVec4 kFrameBgHovered = Hex(0x2A2A2E);
        constexpr ImVec4 kButton = Hex(0x414145);
        constexpr ImVec4 kButtonHovered = Hex(0x4E4E53);
        constexpr ImVec4 kButtonPressed = Hex(0x5C5C61);
        constexpr ImVec4 kText = Hex(0xE8E8EA);
        constexpr ImVec4 kTextDisabled = Hex(0x585B5D);
        constexpr ImVec4 kTransparent = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

        constexpr float kPi = 3.14159265f;
        constexpr float kPanelRounding = 5.0f;
        constexpr float kFontSize = 15.0f;
        constexpr float kToolIconSize = 18.0f;
        constexpr float kComponentHeaderPaddingY = 8.0f;
        constexpr float kHeaderArrowWidthFactor = 0.87f; // where ImGui's arrow visibly ends in its font-sized box
        constexpr float kCheckboxPaddingY = 1.0f;
        constexpr const char* kTextFontPath = "editor://Fonts/Inter-Regular.ttf";
        constexpr const char* kIconFontPath = "editor://Fonts/fa-solid-900.ttf";

        void ApplyColors(ImGuiStyle& style)
        {
            // A new ImGui version can add entries, and those would silently keep ImGui's own colour.
            static_assert(ImGuiCol_COUNT == 63, "ImGuiCol_ changed: give the new entries a colour here");

            ImVec4* colors = style.Colors;
            colors[ImGuiCol_Text] = kText;
            colors[ImGuiCol_TextDisabled] = kTextDisabled;
            colors[ImGuiCol_WindowBg] = kWindowBg;
            colors[ImGuiCol_ChildBg] = kWindowBg;
            colors[ImGuiCol_PopupBg] = kWindowBg;
            colors[ImGuiCol_Border] = kStrip;
            colors[ImGuiCol_BorderShadow] = kTransparent;
            colors[ImGuiCol_FrameBg] = kFrameBg;
            colors[ImGuiCol_FrameBgHovered] = kFrameBgHovered;
            colors[ImGuiCol_FrameBgActive] = kAccent;
            // Docked windows draw their tab bar with the title bar colours.
            colors[ImGuiCol_TitleBg] = kTabBar;
            colors[ImGuiCol_TitleBgActive] = kTabBar;
            colors[ImGuiCol_TitleBgCollapsed] = kTabBar;
            colors[ImGuiCol_MenuBarBg] = kMenuBar;
            colors[ImGuiCol_ScrollbarBg] = kTabBar;
            colors[ImGuiCol_ScrollbarGrab] = kButton;
            colors[ImGuiCol_ScrollbarGrabHovered] = kButtonHovered;
            colors[ImGuiCol_ScrollbarGrabActive] = kButtonPressed;
            colors[ImGuiCol_CheckMark] = kAccent;
            colors[ImGuiCol_CheckboxSelectedBg] = kFrameBg;
            colors[ImGuiCol_SliderGrab] = kAccent;
            colors[ImGuiCol_SliderGrabActive] = kAccentHovered;
            colors[ImGuiCol_Button] = kButton;
            colors[ImGuiCol_ButtonHovered] = kButtonHovered;
            colors[ImGuiCol_ButtonActive] = kButtonPressed;
            colors[ImGuiCol_Header] = kSelected;
            colors[ImGuiCol_HeaderHovered] = kSelectedHovered;
            colors[ImGuiCol_HeaderActive] = kSelectedPressed;
            colors[ImGuiCol_Separator] = kStrip;
            colors[ImGuiCol_SeparatorHovered] = kButtonHovered;
            colors[ImGuiCol_SeparatorActive] = kButtonPressed;
            colors[ImGuiCol_ResizeGrip] = kTabActive;
            colors[ImGuiCol_ResizeGripHovered] = kButton;
            colors[ImGuiCol_ResizeGripActive] = kButtonPressed;
            colors[ImGuiCol_InputTextCursor] = kText;
            colors[ImGuiCol_TabHovered] = kTabHovered;
            colors[ImGuiCol_Tab] = kTabInactive;
            colors[ImGuiCol_TabSelected] = kTabActive;
            colors[ImGuiCol_TabSelectedOverline] = kAccent;
            colors[ImGuiCol_TabDimmed] = kTabInactive;
            colors[ImGuiCol_TabDimmedSelected] = kTabActive;
            colors[ImGuiCol_TabDimmedSelectedOverline] = kAccent;
            colors[ImGuiCol_DockingPreview] = Hex(0xDA7D14, 0.5f);
            colors[ImGuiCol_DockingEmptyBg] = kStrip;
            colors[ImGuiCol_PlotLines] = kButtonPressed;
            colors[ImGuiCol_PlotLinesHovered] = kAccent;
            colors[ImGuiCol_PlotHistogram] = kAccent;
            colors[ImGuiCol_PlotHistogramHovered] = kAccentHovered;
            colors[ImGuiCol_TableHeaderBg] = kComponentHeader;
            colors[ImGuiCol_TableBorderStrong] = kStrip;
            colors[ImGuiCol_TableBorderLight] = kTabActive;
            colors[ImGuiCol_TableRowBg] = kTransparent;
            colors[ImGuiCol_TableRowBgAlt] = Hex(0xFFFFFF, 0.03f);
            colors[ImGuiCol_TextLink] = kAccent;
            colors[ImGuiCol_TextSelectedBg] = Hex(0xDA7D14, 0.35f);
            colors[ImGuiCol_TreeLines] = kButton;
            colors[ImGuiCol_DragDropTarget] = kAccent;
            colors[ImGuiCol_DragDropTargetBg] = kTransparent;
            colors[ImGuiCol_UnsavedMarker] = kText;
            colors[ImGuiCol_NavCursor] = kAccent;
            colors[ImGuiCol_NavWindowingHighlight] = Hex(0xE8E8EA, 0.7f);
            colors[ImGuiCol_NavWindowingDimBg] = Hex(0x000000, 0.2f);
            colors[ImGuiCol_ModalWindowDimBg] = Hex(0x000000, 0.45f);
        }

        void ApplyStyleVars(ImGuiStyle& style)
        {
            // Measured from the mockup: fields are 29px tall and rows 33px apart.
            style.WindowPadding = ImVec2(8.0f, 8.0f);
            style.FramePadding = ImVec2(8.0f, 7.0f);
            style.CellPadding = ImVec2(4.0f, 2.0f);
            style.ItemSpacing = ImVec2(6.0f, 4.0f);
            style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
            style.IndentSpacing = 14.0f;
            style.SelectableTextAlign = ImVec2(0.0f, 0.5f);
            style.DockingSeparatorSize = 4.0f;

            // Off: the drop-down arrow on every tab bar.
            style.WindowMenuButtonPosition = ImGuiDir_None;
            // Off: on rounded tabs the 1px overline shows up as an orange speck in the tab's corner.
            style.TabBarOverlineSize = 0.0f;

            style.ScrollbarSize = 12.0f;
            style.GrabMinSize = 10.0f;

            // Only floating windows use this; docked ones are rounded by EndPanel().
            style.WindowRounding = kPanelRounding;
            style.ChildRounding = 0.0f;
            style.FrameRounding = 3.0f;
            style.PopupRounding = 3.0f;
            style.ScrollbarRounding = 3.0f;
            style.GrabRounding = 3.0f;
            style.TabRounding = kPanelRounding;

            style.WindowBorderSize = 1.0f;
            style.ChildBorderSize = 1.0f;
            style.PopupBorderSize = 1.0f;
            style.FrameBorderSize = 1.0f;
            style.TabBorderSize = 1.0f;
            // Stays at 0: a border would inset the viewport image, and the gizmo is aligned to its rect.
            style.ImageBorderSize = 0.0f;
        }

        void LoadFonts(const FileSubsystem& files)
        {
            const Path textPath = files.Resolve(kTextFontPath);
            const Path iconPath = files.Resolve(kIconFontPath);

            bool allPresent = true;
            for (const Path& path : { textPath, iconPath })
            {
                if (!std::filesystem::exists(path))
                {
                    Log::Error("Editor font is missing: {}", path.string());
                    allPresent = false;
                }
            }
            if (!allPresent)
            {
                return;
            }

            ImFontAtlas* atlas = ImGui::GetIO().Fonts;

            // The flag makes ImGui return null for an unreadable file instead of asserting.
            ImFontConfig textConfig;
            textConfig.Flags |= ImFontFlags_NoLoadError;
            ImFont* font = atlas->AddFontFromFileTTF(textPath.string().c_str(), kFontSize, &textConfig);
            if (!font)
            {
                Log::Error("Editor font could not be loaded: {}", textPath.string());
                return;
            }

            ImFontConfig iconConfig;
            iconConfig.Flags |= ImFontFlags_NoLoadError;
            iconConfig.MergeMode = true;
            iconConfig.GlyphMinAdvanceX = kFontSize; // keeps icons of different widths in line
            if (!atlas->AddFontFromFileTTF(iconPath.string().c_str(), kFontSize, &iconConfig))
            {
                Log::Error("Editor font could not be loaded: {}", iconPath.string());
                atlas->RemoveFont(font); // text without its icons would not be the default font either
            }
        }
    }

    void EditorTheme::Apply(const FileSubsystem& files)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ApplyColors(style);
        ApplyStyleVars(style);
        LoadFonts(files);
    }

    Vec4 EditorTheme::SelectionOutlineColor()
    {
        return Vec4(kAccent.x, kAccent.y, kAccent.z, kAccent.w);
    }

    String EditorTheme::PanelTitle(const char* icon, const String& name)
    {
        return String(icon) + "  " + name + "###" + name;
    }

    void EditorTheme::EndPanel()
    {
        // ImGui forces 0 rounding on anything docked in a viewport-wide dockspace, so each corner is
        // painted over in the dock gap's colour. It runs last so it also covers the content and tab bar.
        if (ImGui::IsWindowDocked())
        {
            const ImVec2 min = ImGui::GetWindowPos();
            const ImVec2 max(min.x + ImGui::GetWindowSize().x, min.y + ImGui::GetWindowSize().y);
            const float r = kPanelRounding;

            // Each arc runs backwards so the shape winds clockwise, which ImGui's anti-aliasing needs.
            struct Corner { ImVec2 point; ImVec2 arcCentre; float startAngle; float endAngle; };
            const Corner corners[] = {
                { min, ImVec2(min.x + r, min.y + r), kPi * 1.5f, kPi },
                { ImVec2(max.x, min.y), ImVec2(max.x - r, min.y + r), kPi * 2.0f, kPi * 1.5f },
                { max, ImVec2(max.x - r, max.y - r), kPi * 0.5f, 0.0f },
                { ImVec2(min.x, max.y), ImVec2(min.x + r, max.y - r), kPi, kPi * 0.5f },
            };

            ImDrawList* drawList = ImGui::GetWindowDrawList();
            drawList->PushClipRect(min, max, false); // the window's own clip rect leaves out the tab bar
            for (const Corner& corner : corners)
            {
                drawList->PathLineTo(corner.point);
                drawList->PathArcTo(corner.arcCentre, r, corner.startAngle, corner.endAngle);
                drawList->PathFillConcave(ImGui::GetColorU32(kStrip));
            }
            drawList->PopClipRect();
        }

        ImGui::End();
    }

    bool EditorTheme::ComponentHeader(const char* icon, const char* title)
    {
        const ImGuiStyle& style = ImGui::GetStyle();
        const ImVec2 contentStart = ImGui::GetCursorScreenPos();

        ImGui::PushStyleColor(ImGuiCol_Header, kComponentHeader);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, kComponentHeaderHovered);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, kComponentHeaderPressed);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, kComponentHeaderPaddingY));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        const String id = String("##") + title;
        const bool open = ImGui::CollapsingHeader(id.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);

        // A label has one text colour, so the icon and title are drawn over the empty header.
        const float fontSize = ImGui::GetFontSize();
        const float gap = style.ItemSpacing.x;
        const float arrowEnd = contentStart.x + style.FramePadding.x + fontSize * kHeaderArrowWidthFactor;
        const float iconSlotX = arrowEnd + gap;
        // Icons differ in width, so each is centred in a fixed slot and every title starts at the same x.
        const float iconX = std::floor(iconSlotX + (fontSize - ImGui::CalcTextSize(icon).x) * 0.5f);
        const float titleX = std::floor(iconSlotX + fontSize + gap);
        const float textY = (ImGui::GetItemRectMin().y + ImGui::GetItemRectMax().y - fontSize) * 0.5f;
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddText(ImVec2(iconX, textY), ImGui::GetColorU32(kAccent), icon);
        drawList->AddText(ImVec2(titleX, textY), ImGui::GetColorU32(kText), title);
        return open;
    }

    void EditorTheme::PushHeaderButton()
    {
        ImGui::PushStyleColor(ImGuiCol_Button, kTransparent);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, kComponentHeaderPressed);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, kButtonPressed);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    }

    void EditorTheme::PopHeaderButton()
    {
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
    }

    bool EditorTheme::CompactCheckbox(const char* label, bool* value)
    {
        const float rowHeight = ImGui::GetFrameHeight();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, kCheckboxPaddingY));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (rowHeight - ImGui::GetFrameHeight()) * 0.5f);
        const bool changed = ImGui::Checkbox(label, value);
        ImGui::PopStyleVar();
        return changed;
    }

    void EditorTheme::PushViewportWindow()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    }

    void EditorTheme::PopViewportWindow()
    {
        ImGui::PopStyleVar();
    }

    void EditorTheme::PushToolButton(bool active)
    {
        ImGui::PushFont(nullptr, kToolIconSize);
        ImGui::PushStyleColor(ImGuiCol_Button, active ? kActiveToolBg : kToolBg);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, active ? kActiveToolBgHovered : kToolBgHovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, active ? kActiveToolBgPressed : kToolBgPressed);
        ImGui::PushStyleColor(ImGuiCol_Border, active ? kAccent : kStrip);
        ImGui::PushStyleColor(ImGuiCol_Text, active ? kAccent : kText);
    }

    void EditorTheme::PopToolButton()
    {
        ImGui::PopStyleColor(5);
        ImGui::PopFont();
    }
}
