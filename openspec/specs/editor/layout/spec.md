# editor/layout Specification

## Purpose
The Editor's dock layout: a committed default for fresh checkouts, a personal layout kept per machine, and a way to return to the default.

## Requirements

### Requirement: Personal layout file
The Editor SHALL save its layout to `Editor/UserSettings/imgui.ini` (gitignored), creating the folder if needed. Debug and Release SHALL share it, and it SHALL survive deleting `build/`.

#### Scenario: Layout persists across configurations
- **WHEN** the user rearranges panels in a Debug build, closes it and starts a Release build
- **THEN** the Release build opens with the same arrangement

### Requirement: Default layout on first run
When `Editor/UserSettings/imgui.ini` does not exist at startup, the Editor SHALL open with `editor://Layouts/DefaultLayout.ini` (Outliner left, Scene in the centre, Inspector right). Otherwise the personal file SHALL be used.

#### Scenario: Fresh checkout
- **WHEN** the Editor starts with no personal layout file
- **THEN** it shows the Outliner on the left, the Scene viewport in the centre and the Inspector on the right

### Requirement: Reset Layout to Default
**View → Reset Layout to Default** (the menu after Edit) SHALL replace the layout with the default at the start of the next frame, without a restart. Floating or moved panels SHALL re-dock, and the result SHALL be saved on exit.

#### Scenario: Reset after moving panels
- **WHEN** the user floats the Inspector and chooses Reset Layout to Default
- **THEN** the Inspector docks back on the right without a restart, and the reset layout is what is saved on exit

### Requirement: Default layout is read-only
The Editor SHALL NOT write `DefaultLayout.ini`.

#### Scenario: Closing never touches the default
- **WHEN** the user rearranges panels and closes the Editor
- **THEN** `DefaultLayout.ini` is unchanged

### Requirement: Missing default layout
If `DefaultLayout.ini` is missing, the Editor SHALL log an error naming the path, run on ImGui's own layout, and disable the reset menu item.

#### Scenario: Default file deleted
- **WHEN** the Editor starts with no personal layout and no `DefaultLayout.ini`
- **THEN** an error names the missing path, the Editor keeps running, and Reset Layout to Default is disabled

### Requirement: Deterministic layout in automated tests
An automated runtime test boot SHALL start from `DefaultLayout.ini` and SHALL NOT write any layout file.

#### Scenario: Test run
- **WHEN** the runtime test suite boots the Editor
- **THEN** each boot starts from the default layout and no ini file is written
