---
tags: [roadmap]
updated: 2026-11-10
---

# Roadmap

Axiom is a C++ game engine with an ImGui editor, built on OpenGL today and heading for DirectX 12. Work is grouped into themed arcs of related systems, and each arc builds on the one before it.

Each change is specified as an [OpenSpec](https://github.com/Fission-AI/OpenSpec) change in `openspec/changes/`. Finished changes are archived into the living specs in `openspec/specs/`. Design decisions are in the [Decision Log](Engine/Reference/Decision%20Log.md).

## Arc E: editor finish line

The editor becomes a complete scene builder.

| # | Topic | Status |
|---|---|---|
| E1 | Save / New / Open scene, with repeatable scene files | done |
| E2 | Resource picker and remove component | done |
| E2b | Fix: the Outliner rename field stays open after clicking away | done |
| E2c | Components declare the components they require, so a required one can't be removed | planned |
| E3 | Duplicate entity (Ctrl+D) | planned |
| E4 | Editor theme: palette, Inter font, icons, rounded panels | done |
| E4b | Default editor layout and View → Reset Layout to Default | done |
| E5 | Transform gizmos, one undo step per drag | done |
| E5b | Engine split into layered libraries, with test targets | done |
| E5c | One test runner with isolated logic and runtime suites | done |
| E6 | Click to select through a GPU ID buffer | planned |
| E6b | Reusable render features extracted from the mask and ID passes | planned |
| E6c | Outline for occluded selected objects | planned |
| E7 | Multi-select and multi-edit | planned |
| E8 | Hierarchy: parenting, transform composition, visibility toggle, local/world gizmo | planned |
| E9 | Asset browser with thumbnails | planned |

## Arc 2: play mode and gameplay code

| # | Topic | Status |
|---|---|---|
| 2.1 | Gameplay code as a static library with one entry point and no globals | planned |
| 2.2 | Systems and `BehaviourComponent` | planned |
| 2.3 | The sample app renders through a camera component | planned |
| 2.4 | Play / Stop in the viewport: scene snapshot, play copy, play camera | planned |
| 2.5 | Explicit submit phase in the frame | planned |

## Later arcs

Topics in these arcs are detailed when each arc starts.

| Arc | Theme | Contents |
|---|---|---|
| 3 | Terrain and picking | Terrain, grid, an orbit/pan camera, CPU raycast picking |
| 4 | Data-driven content | Nested reflected data, in-viewport placement tools |
| 5 | Navigation | Graph-based pathfinding (A*) and agents that follow paths |
| 6 | Frame graph and visuals | FrameGraph on OpenGL, then terrain shading, water, overlays and cascaded shadows |
| 7 | Simulation clock | Fixed timestep, pause and speed-up, simulation separated from rendering, determinism |
| 8 | Scale | Thousands of agents, then a data-oriented (SoA) ECS, measured before and after |
| 9 | Simulation threading | Job system and parallel updates |
| 10 | DirectX 12 backend | Resource states and barriers in the FrameGraph, PIX; OpenGL stays runnable |
| 11 | Render thread | A render thread and parallel command recording on DirectX 12 |

**Added when needed:** a log panel, viewport debug modes, per-view stats, hot reload of gameplay code.
