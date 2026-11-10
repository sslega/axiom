# Editor UX Design

Framework-independent UX design for the engine editor. Records the decisions made
2026-08-31 via `/grill-me`. The near-term editor is built on ImGui (docking), but
nothing here depends on ImGui — these are UX and interaction-model decisions.

**North star:** Blender-tier *systemic coherency* — consistent visual language,
one shared selection, a predictable tool/shortcut logic. Not a clone of Blender's
keymap or its area-type system.

**Non-goal:** writing a UI library. ImGui stays (see
`editor_ui_architecture` memory). Revisit only if explicitly raised.

---

## 1. Interaction paradigm

**Hybrid, mouse-first spine.** Gizmo-primary manipulation (Unity/Unreal/Hazel):
the gizmo is always on the selected object, drag to transform, `W/E/R` swap gizmo
mode. Keyboard shortcuts are menu accelerators, not a modal grammar.

The Blender-style modal grammar (`G/R/S` + axis constraint typing) is a
**deferred power-user layer** — it is layered on later, on top of the operator
system (§17), without reworking the mouse-first path.

**Principle carried from Blender now:** every scene mutation is a **named,
undoable command/operator** (§9, §17). Honor this incrementally — do not scatter
ad-hoc mutation or `ImGui::IsKeyPressed` checks through panels.

---

## 2. Scene hierarchy (engine data model)

**Godot model.** The parent/child link lives on **`Entity`** (the ID / tree
layer), *not* on `TransformComponent`.

- `Entity` stays an ID container that *may* have components. It gains parent +
  ordered-children links.
- `TransformComponent` adds **spatial** meaning only: stores **local** TRS; world
  matrix is derived by walking up to the **nearest transform-bearing ancestor**
  (ancestors without a transform still structure the tree; they are skipped for
  the matrix math).
- **Global / persistent non-world logic → `Subsystem` / `ApplicationModule`, not
  an entity.** A "game manager" is a subsystem. This keeps transform-less
  entities rare; the Outliner is almost always a clean spatial tree.
- Transform-less entities are legal and nestable — they serve as grouping nodes
  ("folders"). A folder is an entity with no Transform plus an editor-only
  "is folder" display hint (deferred until the icon/behavior is wanted).
- `Scene` currently stores `StringMap<SharedPtr<Entity>>` (unordered). Hierarchy
  work must add an **ordered root list** and an **ordered children vector** per
  entity. **Sibling order becomes serialized scene state.**
- Future archetype-ECS migration is not blocked by this and is not being
  optimized for now.

### Reparenting behavior

**Keep world position.** On drag-reparent, back-solve the child's new local TRS
so it stays visually put (`A.world x B.local_new == B.world_old`). The reparent
operator takes a `keepWorldTransform` bool so a "snap / keep local" modifier
(e.g. Shift-drop) can be added later.

---

## 3. Selection model

- **Multi-select set + explicit active entity.** The active entity (last-clicked)
  drives the Property panel in v1. The set receives bulk ops (move, delete,
  reparent, multi-edit).
- **Editor-side `Selection` service owned by `EditorUI`. Never serialized.**
  Selection is not a scene fact. This is the cross-panel shared state that
  `editor_ui_architecture` deferred `GetPanel<T>()` for: the Outliner writes it;
  Property panel, viewports, and gizmo read it.
- **Global within the editor**, not per-viewport. Clicking in one viewport
  highlights in all; the one Property panel follows the one active entity.
  Per-viewport scope is **input/camera focus only** (camera nav affects only the
  hovered viewport — already how the code works).
- **v1 selection source:** Outliner click only (viewport raycast deferred).
  Plain click replaces the set + sets active; `Ctrl`+click toggles one;
  `Shift`+click selects the contiguous range; click empty space deselects all.

### Editor contexts — rejected

No Unreal-style "asset editor" contexts with isolated selection/outliner. **One
editor window, one shared scene selection.** Specialized editors (material graph,
animation timeline, mesh inspector) are just **panels** — a "big" panel owns its
internal sub-state (e.g. selected graph nodes) as plain member variables, and is
driven either by the scene selection or by "what asset did you open." Adding a
tool = one new `EditorPanel` subclass. An optional Godot-style main-area mode
switch is possible polish later, not architecture.

---

## 4. Layout & panels

- **One OS window. Context switch, if ever, via workspace tabs** — no ImGui
  multi-viewport (real OS windows) for now. Tear-off windows are a possible later
  addition; nothing forbids it.
- **Hybrid flexibility:** side panels are fixed types (Outliner, Properties).
  The **center / large dock area** hosts multiple "main editors" — perspective
  viewport, ortho view, material graph, mesh viewer — as tabs. Full Blender
  "any area becomes any editor" is not built; nothing blocks evolving to it.
- **One persisted dock layout** (ImGui writes `imgui.ini` for free). Named
  workspaces deferred until there are enough tools to warrant them.

---

## 5. Undo / redo

**Command pattern, built as a foundation before the Outliner.**

- Each mutation is a command object with `Do()` / `Undo()`; `EditorUI` owns an
  `UndoStack`.
- **Structural ops** (create / delete / reparent / reorder / add-remove
  component) get explicit command classes.
- **Property edits** are auto-generated from the reflection system: record
  `(entity, field-path, old, new)` — no hand-written command per field type.
- **Coalescing:** a drag / numeric scrub is **one** undo entry — begin on
  interaction start (`IsItemActivated`), commit on release
  (`IsItemDeactivatedAfterEdit`).
- **Dirty state = undo-stack head != last-saved marker.** No separate dirty
  tracking.

---

## 6. Scene Outliner

- **Row:** type icon (derived from components: mesh / light / camera) + name +
  an **eye toggle**. The eye maps to an **editor-only "hidden in viewport" flag**
  (editor metadata, not gameplay state). A runtime enable/disable flag is a
  separate engine feature — deferred.
- **Rename:** via the Property panel name field for v1; inline double-click / `F2`
  later.
- **Drag and drop:** drop *on* a row = reparent (keep world, §2); drop *between*
  rows = reorder siblings. Both in v1 (sibling order is serialized, so it needs a
  UI).
- **Context menu:** Add Child > (templates), Add Sibling > (templates),
  Duplicate, Delete. All go through undo commands. **Add Component is a Property
  panel concern, not the Outliner.**
- **Search:** substring filter showing matches + their ancestor chain. Regex
  later.
- Tree reflects the entity hierarchy 1:1. Expand/collapse; persist expansion
  state per session (nice-to-have).

---

## 7. Property panel

- **Reflection-driven layout:** active entity -> one collapsible header per
  component -> fields in declaration order -> widget by `FieldType`
  (Vec3 -> 3 drag-floats, float -> drag-float, bool -> checkbox,
  String -> text input, `SharedPtr<Resource>` -> asset field, §8). Reflection
  model: hand-written external registration, component headers untouched —
  see [Reflection and Serialization](../Data/Reflection%20and%20Serialization.md).
- **Component header:** collapsible + a per-component menu with **Remove
  Component**. Per-component enable/disable toggle **deferred** (needs engine
  state that does not exist yet).
- **Add Component:** button at panel bottom -> searchable popup listing
  `TypeRegistry` factories not already on the entity.
- **Numeric editing:** ImGui drag widgets — drag to scrub, `Ctrl`+click to type.
  One coalesced undo entry per drag.
- **Multi-edit:** show only components common to **all** selected entities
  (intersection). Per field: all agree -> show value; differ -> blank / `—`
  "mixed" indicator (bool differ -> dash / indeterminate). Editing any field
  writes to **all selected** as a single command.
- **Read-only fields** (e.g. `m_id`): shown but disabled. **Reset-to-default:**
  deferred until reflection carries default values.

---

## 8. Asset reference fields

- **v1:** a type-filtered dropdown of the matching files on disk under `engine://`
  and `project://`, scanned when it opens, plus `None`. It replaced the planned
  raw text field, which was never built.
- **Later:** drag-and-drop of a **type-validated** asset from the Asset Browser
  panel. The Asset Browser (scan mount dirs for known extensions, searchable) is
  a separate larger feature.
- Typing a path and a native file dialog are skipped unless the dropdown proves
  too painful before the browser lands.

---

## 9. Entity creation

- **Templates:** Empty, Mesh Object, Directional Light, Camera — each spawns an
  entity with the right components pre-attached. "Add Component" is the secondary
  path.
- **Entry points:** Outliner context menu + a menu-bar Create/Entity menu with
  the same list + optionally a "+" button in the Outliner toolbar. No
  Blender-style viewport `Shift+A` until the operator layer exists.
- **Spawn location:** world origin, at scene root when invoked from the menu bar;
  as a child of the right-clicked entity when invoked from Outliner "Add Child".
  New entity is auto-selected and becomes active. ("Spawn at camera look-at" is a
  later refinement.)
- **Definition:** hardcoded C++ list (name + builder lambda) for now.
  Data-driven templates fold into the deferred prefab feature.
- **Naming:** template name, auto-suffixed on collision (`Empty`, `Empty (1)`).

---

## 10. Save / load

- **Explicit save** (`Ctrl+S`), not autosave. Dirty = undo-head != saved marker
  (§5). Title bar: `SceneName* — Editor`.
- **File menu:** New Scene · Open Scene… · Save (`Ctrl+S`) ·
  Save As… (`Ctrl+Shift+S`) · Recent (later).
- **Dialogs:** native Win32 open / save (Windows-only project).
- **Dirty guard:** New / Open / Quit while dirty -> modal "Save / Don't Save /
  Cancel".
- **One scene open at a time** (no multi-scene tabs).
- Autosave / crash recovery: deferred.
- Depends on `SceneWriter` (planned), which now also serializes hierarchy +
  sibling order.

---

## 11. Viewport navigation

Unity/Unreal hybrid. Flycam stays primary:

| Input | Action |
|---|---|
| RMB-hold + `WASD` + `Q/E` | Flycam move (cursor locked) |
| MMB-drag | Pan |
| `Alt`+LMB-drag | Orbit around selection / pivot |
| Wheel | Dolly (always) |
| RMB-hold + wheel | Adjust fly speed (+ numeric control in viewport header) |
| `F` | Focus / frame selection (reframes pivot on selected bounds) |

Ortho + standard views (front / top / side, persp<->ortho toggle) are a
**per-viewport header control** (§13), not numpad bindings — deferred.

---

## 12. Transform gizmo (ImGuizmo)

- **Mode keys:** `W` / `E` / `R` = translate / rotate / scale; `Q` or `Esc` =
  select-only (gizmo hidden).
- **Space & pivot:** viewport-header toggle buttons for Local/World and
  Pivot/Center (median). Defaults: **World, Pivot**. No keybinds in v1.
- **Snapping:** hold `Ctrl` = snap (grid step / angle step / scale ratio); steps
  set in a small header popup.
- **Multi-select:** one gizmo at selection center (Center mode) or active
  entity's origin (Pivot mode); drag delta applies to all selected as one
  coalesced undo command.
- **Placement:** mode + space/pivot/snap controls in a viewport header toolbar,
  **top-left**, icon buttons.
- Use `ImGuizmo::IsOver()` / `IsUsing()` so a gizmo drag does not also trigger
  selection or camera nav.

---

## 13. Multi-viewport

- **Purpose:** simultaneous camera angles (persp + ortho) and shading modes
  (lit / unlit / wireframe / normals / overdraw).
- **Per-viewport state** (on `ViewportPanel`): its `EditorCamera`, camera-type
  enum (persp/ortho), standard-view setter, shading-mode enum, toggles for
  grid / gizmos / overlays, a maximize toggle.
- **Controls:** viewport header, **right-aligned** (opposite the transform
  tools) — shading dropdown, camera dropdown, overlay toggles, maximize.
- **User-addable** (Window > New Viewport). Independent state, **shared
  selection**. Layout persisted by ImGui.
- **v1 wires only what exists:** perspective + lit. Ortho projection and debug
  shading modes are renderer / `Camera` work, threaded through the `View` struct
  via a `shadingMode` field. Quad-view / standard-view hotkeys deferred to the
  operator layer.

---

## 14. Visual design language

- **One central `EditorTheme`** applied at startup — sets *all* ImGui style vars
  and colors. No inline `PushStyleColor` in panels; variants are named helpers on
  `EditorTheme`. Values in code for v1; external theme file deferred.
- **Visual target:** a UI mockup (kept outside the repo). Where this section and
  the mockup disagree, the mockup wins (decided 2026-11-09).
- **Palette:** dark, flat, low-contrast greys, subtle 1px borders, **a single
  orange accent** for checkmarks, focused fields, the active tool and type icons.
  Selected rows use a muted brown. **Lightly rounded** panels and tabs.
- The **viewport selection outline** uses the same accent, passed in by
  `EditorTheme` (this replaced the earlier rule of a separate outline colour).
- **Density:** sizes measured from the mockup, roomier than ImGui's defaults:
  29px fields, 31px component headers, 26px list rows.
- **Fonts:** real TTF UI font — **Inter** (Blender's own font), ~15–16px,
  oversampled — plus a merged **icon font** (Font Awesome 6 free via
  IconFontCppHeaders) in the same atlas.
- This is **v1 work, not "later"** — it is the difference between "debug HUD" and
  "editor," which is the stated quality bar.

---

## 15. Viewport overlays

- Infinite ground-plane grid, world-axis indicator, small stats overlay
  (FPS / draw calls — counters already exist).
- All toggleable per-viewport (§13).
- Grid + axis are **v1** (cheap, high impact on "looks like an editor").

---

## 16. Play mode

**None in v1** — nothing to simulate (no physics, no scripting). The editor edits
the **live scene** directly. When play mode arrives: snapshot via `SceneWriter`
on Play, restore via `SceneLoader` on Stop; edits during play are discarded
(Unity-style). Documented, nothing built.

---

## 17. Shortcut & command system (deferred target)

Not built for the Outliner / Property v1. The documented north star:

- An **operator registry** as the single source of truth: `id`, `label`,
  `Execute(context)`, `IsEnabled(context)`, optional `defaultShortcut`, optional
  `menuPath`. The undo commands (§5) are the *mutating* subset; non-mutating
  operators also exist (focus-selected, toggle gizmo mode, frame-all).
- **Context object** supplied by the **hovered panel** (active selection,
  viewport ref, …). Dispatch: keypress -> hovered panel's keymap -> global
  keymap -> operator `id` -> `Execute`.
- **Menus auto-generate** from `menuPath`; each item shows its `defaultShortcut`.
  A **command palette** (`Ctrl+Shift+P` / `F3`) lists operators filtered by
  `IsEnabled(context)`.
- **Rebinding:** keymap is an indirection (key -> operator id) from day one,
  stored in a config file; v1 ships defaults only, rebinding UI comes "for free"
  later.

**Near-term guardrail:** funnel each panel's few v1 shortcuts (`Ctrl+S`,
`Ctrl+Z/Y`, `F`, `W/E/R`, `Delete`) through one `HandleShortcuts()`-style method
so migration onto the registry is mechanical.

---

## 18. Keymap principle

Where the interaction spine is mouse-first, keybindings follow **Unity/Unreal
conventions, not Blender's**. The Blender reference is for *visual and systemic
coherency*, not the keymap — mixing Blender's `A`-to-select-all into an otherwise
Unity-like editor creates exactly the inconsistency this design is avoiding.

---

## Deferred / future sections

- Operator registry + contextual keymaps + command palette + rebinding (§17)
- Prefab / instance system (data-driven entity templates fold in here)
- Ortho camera projection + debug shading modes (renderer / `Camera` work)
- Named workspaces / saved layouts
- Tear-off / multi-window panels (ImGui multi-viewport)
- Play / simulate mode (§16)
- Asset Browser panel + drag-to-field + reverse path resolution
- Inline Outliner rename; runtime entity enable/disable flag
- Blender-style modal transform grammar (`G/R/S` + axis typing)
- Full "any area becomes any editor" panel-type system
- Viewport raycast selection (needs spatial query system)
- Python codegen scanner for reflection registration (manual `EngineReflection.cpp`
  is the drop-in target; see [Reflection and Serialization](../Data/Reflection%20and%20Serialization.md))

---

## Build order

New prerequisites (hierarchy, undo + selection, theme) folded into the
`editor_design` build order:

1. **`EditorTheme`** — font, icons, style. Small, independent, immediate payoff.
2. **Reflection registration + `TypeRegistry`** — hand-written `EngineReflection.cpp`
   (member pointers, component headers untouched), `Reflect<T>().Field(...)` builder,
   `type_index` runtime lookup, string factory. Python scanner deferred. See
   [Reflection and Serialization](../Data/Reflection%20and%20Serialization.md).
3. **Hierarchy engine work** (`/workshop`) — parent link on `Entity`,
   ordered roots + children, local/world transform composition, serialize parent
   + sibling order. *Critical-path unblocker for the Outliner.*
4. **Undo stack + `Selection` service** scaffolding in `EditorUI`.
5. **`SceneWriter`** — save; also serializes hierarchy + sibling order.
6. **Scene Outliner** panel — needs 3 + 4.
7. **Property panel** — needs 2 + 4.
8. **Entity templates + Create menu.**
9. **Save / Load workflow** — native dialogs, dirty guard.
10. **Gizmos** — ImGuizmo modes, snap, space/pivot, header toolbar.
11. **Viewport nav additions** — orbit / pan / wheel / focus.
12. **Viewport overlays** — grid / axis / stats + per-viewport header controls.

Steps 1–2 are independent and can interleave with renderer work. Step 3 is the
critical path.

## Where to next

- [Editor](Editor.md): what is actually built so far.
- [Reflection and Serialization](../Data/Reflection%20and%20Serialization.md): how the Inspector and scene files work today.
- Back to the [Engine Map](../Engine%20Map.md).
