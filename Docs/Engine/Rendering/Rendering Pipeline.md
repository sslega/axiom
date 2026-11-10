---
tags: [engine, rendering]
updated: 2026-11-08
---

# Rendering Pipeline

**In one sentence:** every frame the Render subsystem turns mesh components into render commands, then renders each submitted View: shadows, depth pre-pass, color pass, custom mask, then the view's post-process chain.

## Per-frame data and its lifetime

| Data | Built by | Lives | Carries |
|---|---|---|---|
| **View** | whoever owns a camera (editor viewport) | one frame | camera matrices, target framebuffer, size, effects to run (borrowed) |
| **Render command** | renderer, from each visible mesh component | one frame | mesh, material, transform, render flags. **No entity pointer.** |
| **Scene render data** | renderer | refreshed each frame | the directional light, time, transient flags per entity UUID |

The entity → command step is the **only** place that touches the ECS. Everything after it works on commands. That boundary is what a render thread would cut along later.

## A frame inside the renderer

```mermaid
flowchart TB
    G["Gather: light + mesh components<br/>→ render commands (flags baked in)"] --> V
    subgraph V["for each View"]
        S["Shadow map<br/>(fitted to this view)"] --> P["Depth pre-pass"] --> C["Color pass<br/>instanced / batched / single"]
        C --> M{"has effects AND<br/>any CustomMask object?"}
        M -- yes --> MK["Custom mask pass"] --> PP
        M -- no --> PP["Post-process chain"]
    end
    V --> UI["clear window, debug HUD"]
```

## Passes: the ideas

- **Shadows:** one shared shadow map, **re-rendered per view**, because its frustum is fitted to *that* camera. Its center is snapped to shadow-map texels so shadows don't shimmer when the camera moves.
- **Depth pre-pass:** depth only, then the color pass tests "equal or nearer" without writing depth. Each pixel's expensive lighting runs once, for the closest surface.
- **Each pass sets the GPU state it needs.** Nothing is restored afterwards.

## Instancing vs batching vs single

```mermaid
flowchart TB
    G["group by (mesh, material)"] --> Q{"same mesh many times?"}
    Q -- yes --> I["Instancing<br/>1 draw, N transforms"]
    Q -- no --> Q2{"different meshes,<br/>same material?"}
    Q2 -- yes --> B["Batching<br/>CPU pre-transforms vertices<br/>into one buffer, 1 draw"]
    Q2 -- no --> S["one draw each"]
```

Instancing costs a per-frame transform upload; batching costs CPU vertex work every frame. Both can be toggled to compare draw counts.

## Render flags

Per-entity, per-frame bits requested from outside the renderer (the editor marks the selection). They're stored by **entity UUID** in a side table, cleared every frame, and baked into each command during the gather.

> [!important] The rule for a new bit
> Each bit means **"include this object in pass/buffer X"**. A per-object *draw style* (wireframe-only, tint) is **not** a flag; it belongs on the material. A pass filters by **its own bit**, never by "has any flags".

Today: one bit, **CustomMask**, which feeds the custom mask. See [Post-Processing](Post-Processing.md).

## GPU caches

The renderer keeps its own caches: compiled shaders by path, GPU buffers per mesh, instance and batch buffers, temp framebuffers (pooled). Most are keyed by **raw pointers** and never evicted. That's fine while nothing unloads, and a stale-hit hazard once something does.

## Where to next

- [Shaders and Materials](Shaders%20and%20Materials.md): what each pass compiles and binds.
- [Post-Processing](Post-Processing.md): what runs after the color pass.
- [Frame Lifecycle](../Core/Frame%20Lifecycle.md): where the renderer sits inside a frame.
- [Platform Layer](../Core/Platform%20Layer.md): how the renderer stays free of OpenGL.
- Back to the [Engine Map](../Engine%20Map.md).
