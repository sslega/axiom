#pragma once

#include "Core/Types.h"
#include "Math/Vector.h"

namespace axiom
{
    class FileSubsystem;

    /// The Editor's one source of colours, style vars and fonts.
    ///
    /// Apply() runs once at startup, before the first frame. A panel that needs a different look for one
    /// widget calls a named helper here instead of pushing ImGui colours itself.
    ///
    /// @code{.cpp}
    /// EditorTheme::PushToolButton(isActive);
    /// ImGui::Button(ICON_FA_ROTATE "##Rotate", size);
    /// EditorTheme::PopToolButton();
    /// @endcode
    ///
    /// Traps: fonts can only be added before the first frame's atlas use, so call Apply() from startup and
    /// never again. If a font file is missing it logs an error and the Editor runs on ImGui's default font,
    /// where icon labels draw as placeholder boxes. Every push helper needs its matching pop. A panel must end
    /// with EndPanel(), not ImGui::End(), or its corners stay square when docked.
    class EditorTheme
    {
    public:
        static constexpr float kListRowHeight = 22.0f;
        static constexpr float kActionButtonHeight = 30.0f;

        static void Apply(const FileSubsystem& files);

        /// The accent, for the outline drawn around the selected entity in a viewport.
        static Vec4 SelectionOutlineColor();

        /// A window title with an icon on its tab. The ID stays the plain name, so renaming the icon keeps the layout.
        static String PanelTitle(const char* icon, const String& name);

        /// Ends a panel's window, rounding its corners if it's docked. Use it in place of ImGui::End().
        static void EndPanel();

        /// A collapsing header with an accent icon and the header colour, open by default. Returns whether it's open.
        static bool ComponentHeader(const char* icon, const char* title);

        /// Styles a flat button sitting on a ComponentHeader. Pair with PopHeaderButton().
        static void PushHeaderButton();
        static void PopHeaderButton();

        /// Keeps a checkbox small and centred in a row of full-height fields.
        static bool CompactCheckbox(const char* label, bool* value);

        /// Lets a viewport's image fill its panel. Call around ImGui::Begin() only.
        static void PushViewportWindow();
        static void PopViewportWindow();

        /// Styles a viewport tool button; the active tool takes the accent. Pair with PopToolButton().
        static void PushToolButton(bool active);
        static void PopToolButton();
    };
}
