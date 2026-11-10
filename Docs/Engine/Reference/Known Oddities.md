---
tags: [engine, issues]
updated: 2026-11-09
---

# Known Oddities

Stale, buggy, inconsistent or surprising spots. **Verify before acting**, because the code may have moved on.

## Likely bugs

| What | Where | Why it matters |
|---|---|---|
| The Game's camera component pointer is never assigned, but it's used on resize | `Game/src/Sandbox.*` | probable crash when resizing the Game window |
| The Game submits **no Views** | `Game/src/Sandbox.*` | the Game shows only ImGui over black; nothing turns a camera component into a View |
| Material textures always bind to slot 0, while the sampler is told another slot | material resource | any texture not on slot 0 samples the wrong unit |
| The device viewport ignores x / y | OpenGL device | breaks the first time something renders into a sub-rectangle |
| Window size getters return startup values after a resize | GLFW window | stale sizes |
| A viewport submits its View **before** resizing in the same frame | viewport panel | possible one-frame glitch on resize |
| The camera component deserializes with a fixed 16:9 aspect | camera component | wrong aspect until something sets it |
| Scene components tick in the editor | `WorldSubsystem` | anything that reads input will react while you edit, and Save keeps the result. `main.scene.json` no longer has a `CameraController`, so nothing shows this today; adding one brings it back. Play mode should end this |
| Valid JSON with a wrong value type throws during load, after the old scene is cleared | `SceneLoader` | a hand-edited file can crash the editor |
| `Application` has no virtual destructor but is deleted through a base-class pointer | `EntryPoint.h`, `Application` | undefined behaviour; `Editor`'s and `Sandbox`'s own members are never destroyed. Fixing it starts running those destructors at exit, after the window is closed, so it needs its own change |

## Half-finished work

| What | Note |
|---|---|
| Shadow-map setup is inline in the per-view render function, with TODOs | meant to move into the shadow pass itself |
| HUD shows "Custom mask draw calls" | it counts flagged **commands**, not draws |
| The outline ignores occlusion (x-ray) | planned fix in [Post-Processing](../Rendering/Post-Processing.md) |
| The frame-order contract for editor submissions is implicit | see [Frame Lifecycle](../Core/Frame%20Lifecycle.md); refactor wanted |
| Render stats are global, not per view | two viewports' draw counts are summed |
| A directional light with no transform counts as no light and logs an error every frame | temporary, until E2c blocks removing a required component and the assert comes back |

## Leftovers and naming

- `deltaTime` is in milliseconds.
- `GetMaterial(path)` takes a **shader** path and wraps it; it doesn't load a `.mat`.
- The shadow projection's parameter is named "view" but receives the view-projection (which is the correct input).
- `PoolEvents` (should be Poll); `Register*Modules` hooks (the concept is "subsystems" now).
- Unused: `ObjectHandle`, `RenderMesh`, `VertexArray`, `RenderProxyID`.
- `Vertex.cpp` includes its own header twice.

## Design consequences

| What | Consequence |
|---|---|
| Unordered entity / component maps | Outliner and draw order are arbitrary between runs. The Inspector sorts components by type name to stay stable |
| A component can cache a sibling pointer in its initialize hook | removing a transform from under a camera controller leaves its pointer dangling, and the next WASD crashes. `main.scene.json` has no controller today. E2c ends it |
| Loading doesn't update the scene's name counters | a new "Entity" can share a name with a loaded one |
| Per-draw uniforms are written into shared materials | hazard for a render thread |
| GPU caches keyed by raw pointers | stale hits once anything unloads |
| Path resolution lowercases the whole path | Windows-only assumption. Loader lookups depend on it too, so a loader registered with an upper-case extension never matches |
| The resource listing skips a file name that doesn't fit the narrow code page, without logging it | such a file is silently missing from the picker |
| Shader includes always resolve from the engine's include folder | project shaders can't have their own includes |
| The renderer includes ImGui for its HUD | the renderer depends on the "temporary" UI (parked fix: stats as data, drawn by the editor) |
| `ImGuiSubsystem` includes GLFW and the ImGui backends directly | GLFW knowledge outside `Platform/`, in the same `Graphics` library, so the build doesn't catch it |
| Docked panels' rounded corners are painted over in the dock-gap colour | a panel that ends with `ImGui::End()` instead of `EditorTheme::EndPanel()` stays square, and the corners don't follow a hovered divider's highlight |
| Tab and menu bar height follow the field height | ImGui has no separate setting, so they are 29px against the mockup's 33px |
| The theme test includes the icon header by a relative path | the header's include path is private to the Editor library |

## Where to next

- [Where Things Go](Where%20Things%20Go.md): where a fix should go.
- [Decision Log](Decision%20Log.md): whether an oddity was a deliberate trade-off.
- [Frame Lifecycle](../Core/Frame%20Lifecycle.md): the source of most timing surprises.
- Back to the [Engine Map](../Engine%20Map.md).
