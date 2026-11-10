# editor/theme Specification

## Purpose
The Editor's single visual theme: palette, sizes, fonts and icons, set in one place and shared by every panel.

## Requirements

### Requirement: One source for colours and style
At startup every ImGui colour entry and style variable SHALL come from the editor theme. Panels SHALL NOT set colours or style variables inline; variants are named theme helpers.

#### Scenario: Panels use theme helpers
- **WHEN** a panel needs a non-default colour, such as the accent on an active tool
- **THEN** it takes it from a named theme helper rather than setting it inline

### Requirement: Palette
The palette SHALL be: accent (checkmark, slider grab, focused field, active tool) `#DA7D14`; selected row and header `#774B21`; active tool background `#49321F`; window and panel background `#252528`; menu bar `#242328`; title bar, toolbar strip and dock gap `#17181B`; tab bar `#1F1F22`; inactive tab `#222326`; active tab `#2E2E32`; collapsing and component headers `#313235`; frame background `#1D1D20`; button `#414145`; text `#E8E8EA`; disabled text `#585B5D`.

#### Scenario: Selected Outliner row
- **WHEN** an entity is selected
- **THEN** its Outliner row background is `#774B21`

### Requirement: Hover and active states
Hover and active states not listed in the palette SHALL be lighter steps of the same greys, or of the accent where the base is the accent.

#### Scenario: Hovered button
- **WHEN** the user hovers a button
- **THEN** it shows a lighter grey than `#414145`

### Requirement: Sizes and rounding
Frames, buttons and tabs SHALL be 29px tall, component headers 31px, Add Component 30px, Outliner rows 26px apart and dock gaps 4px. Panels (docked or floating) and tabs SHALL be rounded 5px; frames, buttons, popups and grabs 3px; child windows 0; borders 1px.

#### Scenario: Floating panel
- **WHEN** the user floats a panel
- **THEN** its corners are rounded 5px

### Requirement: Tab bars
Tab bars SHALL have no drop-down arrow (so no Hide Tab Bar or tab list menu) and no accent overline. A selected tab SHALL be shown by its lighter background, and each panel's tab SHALL have an icon.

#### Scenario: Selected tab
- **WHEN** a panel's tab is selected
- **THEN** it is shown by a lighter background, with its icon, and no overline

### Requirement: Selection outline colour
The selected entity's viewport outline SHALL use the theme's accent colour.

#### Scenario: Select an entity
- **WHEN** the user selects an entity with a mesh
- **THEN** its outline in every viewport is `#DA7D14`

### Requirement: Fonts and icons
The UI font SHALL be Inter at 15px with Font Awesome 6 Solid merged into it, so an icon can sit in any label. If a font file is missing, the Editor SHALL log an error naming the path and keep running on ImGui's default font.

#### Scenario: Icon in a label
- **WHEN** a label contains a Font Awesome glyph
- **THEN** the icon renders inline with the Inter text

#### Scenario: Missing font
- **WHEN** the Inter font file is missing at startup
- **THEN** an error names the path and the Editor runs with ImGui's default font
