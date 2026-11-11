---
tags: [engine, decisions]
updated: 2026-11-11
---

# Decision Log

Choices that **can't be recovered from the code**: what was picked, what it beat, why, and when to revisit. Newest first. Add to this page whenever a design discussion ends. From 2026-11-05, new tables also name the **principle** behind each decision (a pattern or rule with a checkable source), or say it's a judgment call.

## Licensing and provenance (2026-11-11)

| Decision | Instead of | Why | Principle | Revisit when |
|---|---|---|---|---|
| Axiom is MIT-licensed | Apache-2.0; source-available; no licence | simplest permissive licence; the author keeps every right to use the engine in closed products, and a repo with no licence is legally unclear to everyone else | judgment call | patent concerns or a commercial engine licence come up |
| Every third-party component is listed with its licence in `THIRD_PARTY_NOTICES.md`; copyleft and non-commercial sources are never used | tracking licences ad hoc | the engine must stay usable in closed-source products, which copyleft or NC code would block | judgment call | — |
| Snippet-level licence scan with SCANOSS on changed files at every CL, and on all files before a release | no scanning; a commercial scanner; scanning on every build | catches copied or regurgitated code, including old tutorial-derived code; only fingerprints leave the machine. Per CL, not per build, because it needs the network | shift checks left, but to the cheapest point that still catches the problem | scan noise outweighs findings, or a CI pipeline exists |
| Implementing a published algorithm or adapting external code needs explicit approval of the source and its licence first | a provenance comment written after the fact | a comment records a licence problem; a stop prevents one | judgment call | — |
| Commits with AI-written code carry the standard `Co-Authored-By` trailer; commits without it are human-written code | no marking; a custom tag; a separate spec commit before the code | the boundary between human and AI-written code can be checked from history alone, with the convention git hosts already show. A spec commit minutes before the code proves nothing a timestamp can't be edited to say | judgment call | mixed commits become common |

## Editor layout (2026-11-10)

| Decision | Instead of | Why | Principle | Revisit when |
|---|---|---|---|---|
| The default layout is a hand-authored `DefaultLayout.ini` asset | building it in code with DockBuilder | the author sets the layout by dragging panels, not by writing split ratios | data over code; keep config apart from code | a window ID changes and the default stops matching it (`PanelTitle`'s `###name` keeps IDs stable) |
| The default is read-only; nothing in the Editor writes it | a "Save Layout as Default" menu item | it's the author's opinion of an efficient layout, not user state | never write back to the source of defaults; YAGNI | named user layouts arrive |
| User layout lives in gitignored `Editor/UserSettings/` | next to the exe; `%APPDATA%` | shared by Debug and Release, survives deleting `build/`; AppData only pays off for an installed editor | judgment call | the editor is installed, or layouts become per project |
| `EditorUI` owns the layout policy | paths and logic in `ImGuiSubsystem` | the Game uses `ImGuiSubsystem` too; layout is an Editor concern | dependencies point downward: engine code never knows about the editor | named layouts justify their own small class |
| Reset is a request flag applied at the start of the next `OnUpdate` | loading inside the menu handler | ImGui can't load settings inside a frame, and the Editor's update runs before ImGui's frame starts | command deferral: queue the request, apply it at a safe point in the frame | the Application's update/begin-frame order changes |

## Editor theme (2026-11-10)

| Decision | Instead of | Why | Principle | Revisit when |
|---|---|---|---|---|
| The UI mockup outranks `EditorUX.md` on look: orange accent, rounded panels | desaturated blue accent, square corners | the mockup was drawn as the target. A written rule that disagrees with the picture was the older, vaguer intent | judgment call | a second theme or a style guide is wanted |
| The selection outline uses the UI accent | its own colour, separate from the accent (Blender's split) | the mockup shows one orange for both, and one source keeps them from drifting. The Editor passes the colour in, so the engine effect has no theme knowledge | dependency inversion (Martin, SOLID): the high-level Editor supplies policy to the low-level effect | outline and selected rows become hard to tell apart |
| One `EditorTheme` class with named helpers; panels never push style | inline `PushStyleColor` in panels | one place to change the look, and helpers name intent ("component header") instead of slots | single source of truth / DRY (Hunt & Thomas, *The Pragmatic Programmer*) | an external theme file is wanted |
| Docked panels get rounded corners by painting them over in the dock-gap colour | patching ImGui; leaving them square | ImGui forces windows docked in a viewport-wide dockspace to 0 rounding, and patching a fetched dependency costs every update | judgment call | ImGui gains rounding for docked windows, or the gap colour stops being flat |
| Fonts and the icon header are committed in `Editor/`, not fetched | FetchContent at configure time | the TTFs are runtime files read through `editor://`, so any checkout runs without a download or copy step | judgment call | the font files grow large, or versions start changing often |
| Panel tabs show icons through `###` IDs | plain window names | tab labels need the icon, and the ID stays the plain name so later icon changes keep the layout. The rename cost one re-dock | judgment call | — |

## Test framework (2026-11-09)

| Decision | Instead of | Why | Principle | Revisit when |
|---|---|---|---|---|
| One `AxiomTests` runner replaces `AxiomTests` plus `EditorSmoke` | two executables; tests inside `Editor.exe` and `Game.exe` behind `-test` | the two shared nothing. Hosting in the product would let a broken test block the Editor build, stop tests relinking while the Editor is open, and lose tests silently in static libraries. The cost is that logic-only changes relink against the whole Editor | the product never depends on test code; the structure is a judgment call | relink time hurts, or a Test Runner panel inside the Editor is wanted |
| Tests live in `Tests/`, not beside the code | `TEST_CASE` inside product `.cpp` files; a `tests/` folder per library | much of the code is header-only, runtime scenarios span several systems so no single file owns them, and `DOCTEST_CONFIG_DISABLE` doesn't strip fakes. Production stays clean by never compiling these files | judgment call | — |
| `Tests/Logic` and `Tests/Runtime` are separate object libraries | one source list | each folder's include path is its dependency boundary, so `Logic` reaching for Graphics is a compile error. Object libraries also link whole, which keeps self-registering tests | levelization (Lakos, *Large-Scale C++ Software Design*) | — |
| Isolation is a fresh scene per test in one booted Editor | one process per test | `Application` is a singleton, and booting per test is slow. Long-lived caches stay shared, which is what stability tests should stress. A crash ends the run but is reported against the test that caused it | judgment call | a crash ending the run becomes annoying |
| Runtime tests use a hidden window with a real GPU | a no-op renderer | "change something, then keep rendering" only proves anything if a GPU really draws, and a no-op backend would need every graphics interface stubbed | judgment call | tests must run on a machine with no GPU |
| `Selection` stores an entity ID and looks it up in the `Scene` | a raw `Entity*` cleared by whoever removes it; clearing after undo | every removal path had to remember to clear it, and undo didn't, which was a use-after-free. Lookup makes dangling impossible and lets undo restore the selection. It costs one hash lookup per query | make illegal states unrepresentable (Yaron Minsky) | selection becomes multi-select (E7) |

## Resource picker and remove component (2026-11-09)

| Decision | Instead of | Why | Principle | Revisit when |
|---|---|---|---|---|
| A resource is an ordinary `FieldValue`: a path plus a type-erased resource, read and written through `get` and `set` | going through `serialize` / `deserialize` with a throwaway JSON node; a dedicated `setResource` closure | the first version left resource fields the only kind with empty `get` and `set`, and needed its own command. Now `PropertyEditCommand` covers them. It costs a `void` cast inside `set`, and the undo stack holds old resources alive | judgment call: remove the special case rather than tidy it | resources get evicted while the undo stack holds them |
| Loaders state their resource type, and `ResourceSubsystem` answers which extensions load it | a type → extensions table in the Editor | the Editor's copy would drift when a loader is added | single source of truth (DRY, *The Pragmatic Programmer*) | — |
| The picker is a dropdown of files on disk, scanned when it opens | a text field (`EditorUX.md` v1); only loaded resources; a cached list | a scene has to be able to pick a mesh it doesn't use yet, and a few dozen files need no cache | judgment call | the asset browser (E9) needs the same list |
| The file listing takes plain folders and extensions, in `EditorCore` | moving it to `EditorApp` beside the subsystems | it stays reachable from `AxiomTests`, which can't see `Simulation` | levelization (Lakos, *Large-Scale C++ Software Design*) | — |
| Virtual paths ignore case through one `ToLower`, used for lookups and cache keys only; stored paths keep their casing | comparing without case at each call site; lowercasing stored paths | the file lookup already ignored case while the caches didn't, so one file could load twice. Lowercasing stored paths would rewrite saved scenes | one owner for a convention | the engine targets a case-sensitive file system |
| The Inspector sorts components by type name | ordered storage in `Entity`, with undo restoring the position | scene saving already sorts by that name, so the panel matches the file and needs no Engine change. There's no user-chosen order | judgment call | components need a user-chosen order |
| Temporary: a directional light with no transform counts as no light, and logs an error every frame | keeping the assert, which aborts the Editor; blocking the removal in this change | Remove Component made the assert reachable, and blocking needs components to declare what they require. Marked temporary so it can't become lasting error handling | judgment call | E2c blocks the removal and restores the assert |

## Engine layers and tests (2026-11-09)

| Decision | Instead of | Why | Principle | Revisit when |
|---|---|---|---|---|
| Engine is four libraries in separate folders: `EngineCore`, `Simulation`, `Graphics`, `Runtime` | one library with layering by convention; only a small graphics-free math library | anything testable dragged in the window and GL, and the split is needed for the DX12 backend and simulation threading anyway. Separate include roots make an upward include a compile error | levelization (Lakos, *Large-Scale C++ Software Design*) | a layer keeps needing workarounds to stay clean |
| Graphics sits on top of Simulation, not beside it | two sibling libraries under the runtime | the renderer reads the world and resources, so siblings would need a cycle | dependencies point one way | the renderer stops reading the world directly (render thread) |
| `SubsystemHost` base class holds the subsystems; subsystems see only the host | subsystems holding `Application&`; an abstract lookup interface | `Application` includes the window and graphics device, which blocked the split. `GetSubsystem<T>` is a template, so it can't be a virtual | interface segregation (SOLID) | subsystems need more than lookup from their owner |
| `Archive` takes a small `IResourceResolver` | `Archive` holding `ResourceSubsystem&` | this one link pulled the whole app into serialization | dependency inversion (SOLID) | — |
| `Vertex` is plain data; its GPU layout is a free function in `Renderer/` | `Vertex` knowing its buffer layout | CPU meshes must not depend on graphics | CPU data vs GPU objects (Where Things Go) | — |
| doctest, in a separate `AxiomTests` executable | Catch2; GoogleTest; a hand-written check macro | single header and fast to compile; math and logic tests need no mocks | judgment call | tests need mocks or matchers |
| Code belongs in `EditorCore` only if it means something without the UI | moving everything ImGui-free; keeping only what the first tests needed | selection and undo make sense with no UI. A gizmo drag only exists during an ImGuizmo interaction, so `GizmoState` and the drag stay in the viewport panel, and a `GizmoDragSession` class would have wrapped one caller | judgment call | a second kind of gizmo or a scripted drag appears |
| `Editor` is the `EditorApp` library plus a thin executable; the smoke run is `EditorSmoke` under `Tests/` | a `--smoke` flag and frame counter inside `Editor`; a subclass in the Editor project | test code depends on the product, and the product never depends on test code. `SmokeEditor` uses the same hooks as any app | Template Method (GoF) for the hooks; the structure is a judgment call | — |

## Transform gizmos (2026-11-08)

| Decision | Instead of | Why | Principle | Revisit when |
|---|---|---|---|---|
| Rotation is decomposed by our own code, in `TransformComponent`'s order and radians | ImGuizmo's decomposition | it assumes the opposite rotation order and returns degrees, so multi-axis rotations would jump | keep one owner for a convention | rotations move to quaternions |
| Each drag writes one reflected field and reuses `PropertyEditCommand` | a dedicated transform command; writing all three fields | one field per mode stops float drift reaching the others, and needs no new class | reuse the existing command | multi-select needs a combined command (E7) |
| Translate and scale snap to absolute multiples in our code, with a matching step given to ImGuizmo for the handles | snapping only in ImGuizmo; snapping only in our code | ImGuizmo snaps a multiplier from the drag's start, so alone it gives offsets rather than multiples; its handles only step if it knows about the snap | judgment call | a custom readout or gizmo replaces ImGuizmo's |
| Rotation snaps by offset from the drag's start | absolute multiples of 15 degrees | a world-axis drag changes all three Euler angles, so rounding them would break the rotation | judgment call | rotations move to quaternions, or Local mode lands (E8) |
| `CameraController` removed from `main.scene.json` | leaving it until play mode; stopping scene ticks now | W/E/Q gizmo keys would move the camera and Save would keep it; stopping ticks is an Engine change | smallest change that removes the conflict | Arc 2.2 brings camera control back as a behaviour |
| A drag released outside the viewport is ended through ImGuizmo's public `Enable` calls | leaving the drag stuck; patching ImGuizmo | one drag must stay one undo step, and the pinned library stays unmodified | judgment call | ImGuizmo fixes this upstream |
| Camera input requires hover as well as focus | focus alone | a focused viewport would otherwise lock the cursor on a right-click in another panel | judgment call | — |

## Spec-driven development (2026-11-08, revised 2026-11-10)

| Decision | Instead of | Why | Principle | Revisit when |
|---|---|---|---|---|
| Every change is an [OpenSpec](https://github.com/Fission-AI/OpenSpec) change: a proposal, a design, spec deltas and tasks, archived into living specs in `openspec/specs/` | GitHub Spec Kit; a hand-rolled process | built for changing existing code; light per change; archiving keeps the living specs current with no separate sync step | spec-anchored level, from Birgitta Böckeler's "Understanding Spec-Driven Development" | the specs cost more to read than the diffs |
| Specs are public, next to the code | a private docs repo tied to code by covered paths and a synced commit | specs and code change in the same commit, so they can't drift | one-way dependency: specs reference code, never the reverse | — |
| AI agents may build and run only when implementing a change end to end, in the shared `build/` | a separate worktree; a human building every time | an agent without a compiler is blind; the shared folder avoids a second full build | judgment call | waiting out runs or file locks becomes the felt pain |
| An agent run stops at an Engine change, a lifecycle or reflection change, a new dependency or a workaround | letting the agent decide | the first spec-driven change grew into an Engine lifecycle change and picked up a hack that hid a load-order flaw | judgment call | stops fire so often that the specs aren't saving attention |

## Scene files and component lifecycle (2026-11-08)

| Decision | Instead of | Why | Principle | Revisit when |
|---|---|---|---|---|
| Register when a component joins its entity; initialize once the entity is complete and joins a scene | register and initialize together at add time | file order shouldn't decide whether a scene loads; undoing a delete now restarts components | two-phase initialization (Unreal's `OnRegister` / `InitializeComponent`, Godot's `_enter_tree` / `_ready`) | game code needs "the whole scene is loaded": add a start hook in the play-mode arc |
| Entity and component IDs are saved in the scene file | regenerating IDs on load; sorting and matching by name | reference properties need a stable target, and names aren't unique | identity is separate from display name | — |
| Save sorts entities by ID and components by type, only to keep files repeatable | relying on unordered map order; writing the transform first | load must not depend on order; a repeatable file diffs cleanly | judgment call | the file should follow Outliner order |
| `LoadScene` replaces the scene, and parses the file first | adding to the scene | Open needs a replace, and a bad file shouldn't cost the current scene | validate before mutating | additive loading is needed |
| Future `EntityRef`: UUID on disk, cached pointer plus scene version in memory (designed, not built) | a resolve pass after load; `SharedPtr` handles; string lookup on every use | a resolve pass goes stale after any delete, `SharedPtr` keeps dead objects alive, and string lookups are too slow at scale | identity separate from address (Andre Weissflog, "Handles are the better pointers") | SoA ECS: same `Resolve`, index + generation inside |
| Scene path lives in `WorldSubsystem`; New is empty; Save without a path shows a dialog; unsaved changes aren't guarded | keeping the path in `EditorUI`; default camera and light; disabling Save; a prompt before New or Open | smallest thing that makes the editor's work survive a restart | judgment call | losing unsaved work costs real time |

## Working method (2026-11-05)

| Decision | Instead of | Why | Principle | Revisit when |
|---|---|---|---|---|
| Docs have two homes: header comments and Doxygen for the code API; Obsidian for how things fit, why, and where new things go | everything in Doxygen; everything in Obsidian | nothing to migrate; Obsidian keeps live pages, the canvas and offline diagrams; generated API notes would swamp the vault | judgment call | a public site becomes worthwhile |
| Wiki pages live in topic folders, without numbers, and end with links to related pages | one flat numbered list | the numbers had stopped matching the topic order; reading should follow related ideas | judgment call | — |
| The wiki is updated only when ownership, frame order or module boundaries change. Who Does What is folded into Where Things Go and Application and Subsystems | updating the affected pages and an index for every feature | generated records were piling up unread; per-class facts already live in the header comments | judgment call | the wiki visibly falls behind the code |
| Three doc layers: the wiki for concepts, a usage comment at the top of each main class, Doxygen for raw signatures | a hand-written API page per area; Doxygen alone | signatures alone don't show how to use a type, and comments next to the code go stale more slowly and appear on hover | judgment call | a public site becomes worthwhile; graphs get wanted (Graphviz) |

## Direction (2026-10-16)

| Decision | Instead of | Why | Revisit when |
|---|---|---|---|
| The editor reaches a defined **finish line** (a usable scene builder), then grows only when a concrete need asks | an open-ended editor | an editor with no finish line kept growing sideways | — |
| **Play mode inside the editor viewport** (scene snapshot → play copy → discard on Stop) | a separate game window | Unity-style iteration; reuses serialization | — |
| Game logic in **C++**; numerous things = **data components + systems**; one-offs = **`BehaviourComponent`** with its own update | everything as component updates; a scripting language | systems survive the SoA rewrite and become jobs for threading; behaviours stay simple where there are few | — |
| Behaviours and systems tick **only in the play scene**; editing-time ticking is opt-in | always ticking | the edit scene stays inert data | a preview needs live logic |
| Game code is a **static library** linked into Editor and Game | a DLL now | play mode with the least setup; a DLL needs Engine as a DLL first (otherwise two copies of every global) | restarting to test code changes really hurts → hot-reload arc |
| Game code reaches the engine through **one entry point** and owns no globals | — | keeps the later DLL switch a packaging change | — |
| **ImGui is THE editor UI** until engine v1; editor logic stays ImGui-free | ImGui as temporary, a separate "real" editor later | a second editor app doubles the UI work for no new capability | after v1 |
| **One window, dockable panels** (Unity model) | separate per-asset editors (Unreal model) | simpler; ImGui docking already fits | — |
| Viewport picking via a **GPU ID buffer** | CPU raycast | pixel-perfect; reuses the custom-mask pass machinery | — (CPU raycast still comes for terrain picking) |
| **FrameGraph v1 on OpenGL**, once there are enough real passes to design from | waiting for DX12; building it now | ordering, culling and transient memory all work on GL; designed from ~10 real passes | — |
| **DirectX 12** after simulation threading, **before the render thread** | Vulkan; the render thread on OpenGL | Windows/Steam target, industry value; an OpenGL render thread is a dead end | — |

## Rendering and post-processing (2026-10)

| Decision | Instead of | Why | Revisit when |
|---|---|---|---|
| **Custom mask is an engine scene texture**, produced by the renderer | each effect draws its own mask | like scene color / depth, any effect can read it; the renderer already owns the commands | many effects want different filters |
| Mask produced only if the view has effects **and** something is flagged | always | no consumer or no content = no cost | an effect needs a mask every frame anyway |
| Effects **write back themselves**; early-out = untouched | the chain loop copies after every effect | doing nothing costs nothing; no stale-buffer bug | moving to ping-pong (touches every effect) |
| Temp buffers from a **per-frame pool** | per-view persistent buffers | views have no stable identity; the pool returns the same texture anyway | temporal effects need last frame's data |
| Per-view **context object** for effects | growing the parameter list | stable signature as inputs grow | becomes a render-graph pass input |
| Outline via **Jump Flood** | edge detection / inverted hull | constant pixel width, any thickness, a few passes | — |
| Camera data stays on the CPU context; a per-view **constant buffer** comes later | constants only | CPU code needs the view too (sizes, buffers); hidden "current view" state breaks deferral | doing the constant-buffer feature |
| **Render features** + pass list after the outline; a real **render graph** later | building the graph now | design from real passes | done: RenderFeatures after the ID buffer, FrameGraph later (see Direction above) |

## Selection → renderer (2026-10)

| Decision | Instead of | Why | Revisit when |
|---|---|---|---|
| **Generic render flags** ("include in pass X") | a selection-specific API | the same mechanism serves games (highlight pickups) | — |
| Selection is **scene-level**, in the renderer's per-frame scene data | per-View list | viewports can't disagree about selection; flags baked once per frame | multiple render scenes (mesh editor) |
| Flags stored **by entity UUID** in a side table | `Entity*`, or a flag on the component | UUIDs are never reused, so no wrong-object (ABA) hits; selection isn't scene data, so no serialization or undo | ECS moves to generational handles |
| Gather bakes flags into commands; **no entity pointer leaves the gather** | passes look up entities | the boundary a render thread will cut along | Phase 3 |
| Editor-only code excluded **by build target** | `#ifdef WITH_EDITOR` | one static engine lib would need two builds; struct layouts differing per define = silent ODR corruption | engine needs editor-only data (then: two engine targets) |

## Reflection (2026-08-31)

| Decision | Instead of | Why | Revisit when |
|---|---|---|---|
| Types are described in **hand-written registration files**; component headers stay plain C++ | the RTTR library; macros in headers; a Python scanner now | RTTR is unmaintained since 2018, its own variant and type system would leak into the Inspector and serialization, and it skips the learning the project exists for. Macros would clutter headers that should stay plain. A scanner adds a permanent Python build dependency and build-rule friction under the Visual Studio generator | hand-writing gets painful (roughly 40+ components); a scanner could emit the same file |
| Registration through **explicit startup calls** | self-registering statics | the linker discards unreferenced files in the static `Engine` library, which would need `/WHOLEARCHIVE` | — |

## Earlier foundations

| Decision | Why |
|---|---|
| OpenGL first, behind a device abstraction | fast iteration now; DirectX 12 later as a new platform folder (Vulkan dropped 2026-10-15: Windows/Steam target, DX12 more valuable for a tech artist) |
| Single-threaded by design | threading is a deliberate later learning phase |
| Object-style ECS for now | simple to learn; data-oriented ECS is a Phase 3 measurement target |
| Reflection with closures, registered in one file | one description drives serialization, Inspector, undo, Add Component |
| Static handle only if it passes the four-question test | avoid hidden globals; owner ≠ accessor |
| ~~ImGui as a temporary tool~~ | superseded 2026-10-16: ImGui is the editor UI until v1 (see Direction above) |

## Where to next

- [Where Things Go](Where%20Things%20Go.md): the rules these decisions add up to.
- [Known Oddities](Known%20Oddities.md): what the trade-offs cost so far.
- [Roadmap](../../Roadmap.md): what gets built next.
- Back to the [Engine Map](../Engine%20Map.md).
